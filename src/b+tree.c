
#include "b+tree.h"

/* local functions */
void load_block(char *storage, long int block_num, struct bx_tree *bx) {
   static char null_block[BLOCKSIZE] = { 0 };
   if (block_num >= bx->total_blocks) {
      lseek(bx->file,0,SEEK_END);
      int res = write(bx->file,null_block,BLOCKSIZE);
      bx->total_blocks++;
   }
   lseek(bx->file,BLOCKSIZE*block_num,SEEK_SET);
   read(bx->file, storage, BLOCKSIZE);
}

void write_block(char *block, int pointer, struct bx_tree *bx) {
   lseek(bx->file,pointer*BLOCKSIZE,SEEK_SET);
   write(bx->file,block,BLOCKSIZE);
}

void safe_load_cache(long int block_num, struct bx_tree *bx) {
   if (bx->cache_written) {
      write_block(bx->cached,bx->cache_pos,bx);
      bx->cache_written = 0;
   }
   bx->cache_pos = block_num;
   load_block(bx->cached,block_num,bx);
}

void get_key(long int *key, char *block, int idx) {
   int pos = idx*16 + 4;
   memcpy(key,block+pos,sizeof(long int));
}

void get_val(int *val, char *block, int idx) {
   int pos = idx*16 + 12;
   memcpy(val,block+pos,sizeof(int));
}

void get_pointer(int *pointer, char *block, int idx) {
   int pos = idx*16;
   memcpy(pointer,block+pos,sizeof(int));
}

void set_slot(long int key, int val, char *block, int idx) {
   int pos = idx*16+4;
   memcpy(block+pos,&key,sizeof(long int));
   memcpy(block+pos+8,&val,sizeof(int));
}

void set_val(int val, char *block, int idx) {
   int pos = idx*16+12;
   memcpy(block+pos,&val,sizeof(int));
}

void set_key(long int key, char *block, int idx) {
   int pos = idx*16+4;
   memcpy(block+pos,&key,sizeof(long int));
}

void set_pointer(int pointer, char *block, int idx) {
   int pos = idx*16;
   memcpy(block+pos,&pointer,sizeof(int));
}

void inc_elements(char *block) {
   static int val_buf;
   get_val(&val_buf,block,MAX_ELEMENTS);
   set_val(val_buf+1,block,MAX_ELEMENTS);
}

int get_elements(char *block) {
   static int val_buf;
   get_val(&val_buf,block,MAX_ELEMENTS);
   return val_buf;
}

int get_parent(struct bx_tree *bx) {
   return bx->parent_stack[bx->parents];
}

void add_parent(int pointer, struct bx_tree *bx) {
   bx->parent_stack[++(bx->parents)] = pointer;
}

int rem_parent(struct bx_tree *bx) {
   int ret = get_parent(bx);
   bx->parents--;
   return ret;
}

void clear_parents(struct bx_tree *bx) {
   bx->parents = 0;
}

int is_full(char *block) {
   long int last;
   get_key(&last,block,MAX_ELEMENTS-1);
   return last != 0;
}

void shift(char *block, int from) {
   int pos = from*16;
   int has_next_pointer,pointer;
   /* saving and replacing the pointer is nessesary to preserve ordering */
   get_pointer(&pointer,block,from);
   get_pointer(&has_next_pointer,block,from);

   /* move values over by one slot */
   if (has_next_pointer) {
      memmove(block+(pos+16),block+pos,BLOCKSIZE-(from*16+28));
   }
   else {
      memmove(block+(pos+16),block+pos,BLOCKSIZE-(from*16+32));
   }

   /* clear slot */
   memset(block+pos,0,16);

   set_pointer(pointer,block,from);
}

int find_slot(long int key, char *block) {
   long int curr_key;
   int idx = 0;
   get_key(&curr_key,block,0);
   while (curr_key) {
      if (key <= curr_key) break;
      idx++;
      get_key(&curr_key,block,idx);
   }
   return idx;
}

void add_slot_x_split_pointer(int val, long int key, int pointer, char *block, int block_num, struct bx_tree *bx) {
   /* if block if full we need to split it */
   if (is_full(block)) {
      if (block_num == 0) {
         long int mid_key = split_root(bx);
         /* now we need to add to one of the new children 
          *  - we will determine this by going to the left or right of the middle key */
         if (key < mid_key) {
            safe_load_cache(bx->total_blocks-2,bx);
         }
         else {
            safe_load_cache(bx->total_blocks-1,bx);
         }
         block = bx->cached;
      }
      else {
         split(block,bx);
      }
   }

   /* get position */
   int idx = find_slot(key,block);
   
   /* oepn the position */
   shift(block,idx);

   /* add key, value, and pointer */
   set_slot(key,val,block,idx);
   set_pointer(pointer,block,idx+1);
   
   /* fin */
}

void split(char *block, struct bx_tree *bx) {
   static int mid = (BLOCKSIZE / 2) - 16;
   int parent;
   char new_block[BLOCKSIZE];
   int to_add_pointer = bx->total_blocks;
   int old_pointer;
   long int to_add_key;
   int to_add_val;
   
   /* create new block */
   load_block(new_block,bx->total_blocks,bx);

   /* get middle value */
   get_key(&to_add_key,block,mid/16);
   get_val(&to_add_val,block,mid/16);

   /* copy half to new block - exclude last slot */
   memcpy(new_block,block+mid,BLOCKSIZE-(mid+16));

   /* set new block to point to next block */
   get_pointer(&old_pointer,block,MAX_ELEMENTS);
   set_pointer(old_pointer,new_block,MAX_ELEMENTS);

   /* and erase the copied portion from old block */
   memset(block+mid,0,BLOCKSIZE-mid);
   bx->cache_written = 1;

   /* set block to point to new block */
   set_pointer(to_add_pointer,block,MAX_ELEMENTS); /* leaf points to next leaf */
   //set_pointer(to_add_pointer,block,mid/16); /* also as last element for shifting effects */

   /* write new block changes */
   write_block(new_block,to_add_pointer,bx);

   /* add values to parent now */
   if (get_parent(bx) == 0) {
      add_slot_x_split_pointer(to_add_val,to_add_key,to_add_pointer,bx->root,0,bx);
   }
   else {
      int parent = rem_parent(bx);
      safe_load_cache(parent,bx);
      add_slot_x_split_pointer(to_add_val,to_add_key,to_add_pointer,bx->cached,parent,bx);
   }
}

long int split_root(struct bx_tree *bx) {
   static int mid = (BLOCKSIZE / 2) - 16;
   char new_block_left[BLOCKSIZE];
   char new_block_right[BLOCKSIZE];
   int left_pointer = bx->total_blocks;
   int right_pointer = bx->total_blocks+1;
   int mid_val;
   long int mid_key;
   
   /* get middle value */
   get_val(&mid_val,bx->root,mid/16);
   get_key(&mid_key,bx->root,mid/16);

   /* create new blocks - 2 children */
   load_block(new_block_left,left_pointer,bx);
   load_block(new_block_right,right_pointer,bx);

   /* copy contents of root to blocks */
   memcpy(new_block_left,bx->root,BLOCKSIZE-(mid+28));
   memcpy(new_block_right,bx->root+mid,BLOCKSIZE-(mid));

   /* set last pointer in left block to point to right block */
   set_pointer(right_pointer,new_block_left,MAX_ELEMENTS); /* leaf pointer to next leaf */
   //set_pointer(right_pointer,new_block_left,mid/16); /* alse setting as last pointer for shifting */

   /* write changes */
   write_block(new_block_left,left_pointer,bx);
   write_block(new_block_right,right_pointer,bx);

   /* add values to root now */
   /* clear root */
   memset(bx->root,0,BLOCKSIZE);

   /* set pointers to left and right blocks */
   set_pointer(left_pointer,bx->root,0);
   set_pointer(right_pointer,bx->root,1);

   /* add mid value to root - only value in root now */
   set_slot(mid_key,mid_val,bx->root,0);

   return mid_key;
}

void print_block(char *block) {
   static int val,pointer;
   static long int key;
   for (int i = 0; i < MAX_ELEMENTS; i++) {
      get_pointer(&pointer,block,i);
      get_key(&key,block,i);
      get_val(&val,block,i);
      printf("| IDX: %4d | POINTER: %10d | KEY: %20ld | VAL: %10d |\n",i,pointer,key,val);
   }
   get_pointer(&pointer,block,MAX_ELEMENTS);
   printf("Final Pointer: %d\n",pointer);
}

/* non-local functions */

void start_bx_tree(struct bx_tree *bx) {
   bx->file = open(bx->filepath,O_CREAT|O_RDWR,S_IRWXU|S_IRWXG);
   struct stat fst;
   fstat(bx->file,&fst);
   bx->total_blocks = fst.st_size / BLOCKSIZE;
   load_block(bx->root,0,bx);
   bx->cache_pos = 0;
   bx->root_pos = 0;
   bx->parents = 0;
   bx->parent_stack[0] = 0;
}

void close_bx_tree(struct bx_tree *bx) {
   write_block(bx->root,0,bx);
   if (bx->cache_written) {
      write_block(bx->cached,bx->cache_pos,bx);
   }
   close(bx->file);
}

int add_bx_tree(long int key, int val, struct bx_tree *bx) {
   if (!key) { printf("error: key must be non-zero\n"); return -1; }

   /* first check root */
   int old_pointer, pointer, idx = find_slot(key,bx->root);
   long int curr_key;
   
   get_key(&curr_key,bx->root,idx);
   /* change existing value if found */
   if (curr_key == key) {
      set_val(val,bx->root,idx);
      return 1;
   }

   get_pointer(&pointer,bx->root,idx);
   /* if there is a pointer start descending */
   if (pointer) {
      clear_parents(bx);
      safe_load_cache(pointer,bx);
      /* start looking though the tree */
      while (1) {
         idx = find_slot(key,bx->cached);
         get_key(&curr_key,bx->cached,idx);
         /* change existing value if found */
         if (curr_key == key) {
            set_val(val,bx->cached,idx);
            return 1;
         }
         bx->cache_pos = pointer;
         get_pointer(&pointer,bx->cached,idx);
         /* if there is a pointer decend again */
         if (pointer && idx != MAX_ELEMENTS) {
            load_block(bx->cached,pointer,bx);
            add_parent(bx->cache_pos,bx);
         }
         else { /* else add at cached block */
            if (is_full(bx->cached)) {
               split(bx->cached,bx);
               bx->cache_written = 1;
               return add_bx_tree(key,val,bx);
            }
            shift(bx->cached,idx);
            set_slot(key,val,bx->cached,idx);
            bx->cache_written = 1;
            return 1;
         }
      }
   }

   /* no pointer, add to root */
   if (is_full(bx->root)) {
      split_root(bx);
      return add_bx_tree(key,val,bx);
   }
   shift(bx->root,idx);
   set_slot(key,val,bx->root,idx);

   return 1;
}


