
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

int is_internal(char *block) {
   int first;
   get_pointer(&first,block,0);
   return first != 0;
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

void leaf_shift(char *block, int from) {
   int pos = from*16;
   memmove(block+(pos+16),block+pos,BLOCKSIZE-(pos+32));

   /* clear slot */
   memset(block+pos,0,16);

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
   long int curr_key;
   int curr_pointer, curr_val, pos;
   
   /* first search the root */
   pos = find_slot(key, bx->root);
   get_key(&curr_key,bx->root,pos);
   if (curr_key == key) {
      /* if the current key is equal to our key, we replace the value */
      set_val(val,bx->root,pos);
      pos++; /* decend to this node's children */

      if (!is_internal(bx->root)) {
         /* if this is not an internal node we are done, and can exit */
         return 1;
      }
   }

   get_pointer(&curr_pointer,bx->root,pos);
   if (curr_pointer) {
      /* if there is a pointer we are going to decend */
      clear_parents(bx);
      add_parent(0,bx);

      safe_load_cache(curr_pointer,bx);

      /* now we enter the case where we are decending */
      while (1) {
         /* same as before */
         pos = find_slot(key, bx->cached);
         get_key(&curr_key,bx->cached,pos);
         if (curr_key == key) {
            /* replace old value */
            set_val(val,bx->cached,pos);
            bx->cache_written = 1;
            pos++; /* decend to this node's children */

            if (!is_internal(bx->cached)) {
               /* we are done here */
               return 1;
            }
         }

         get_pointer(&curr_pointer,bx->cached,pos);
         if (curr_pointer && is_internal(bx->cached)) {
            /* decend */
            add_parent(bx->cache_pos,bx);
            safe_load_cache(curr_pointer,bx);
            continue;
         }

         /* now if the value is not here and we can't decend we need to add the
          * value here */
         if (is_full(bx->cached)) {
            split_leaf(key,bx);
         }
         pos = find_slot(key,bx->cached);

         leaf_shift(bx->cached,pos);
         set_slot(key,val,bx->cached,pos);
         bx->cache_written = 1;
         
         /* we are done now */

         return 1;

      }
   }

   /* root is only node - add here - only call this once */
   if (is_full(bx->root)) {
      char block[BLOCKSIZE];
      int ret;
      split_root_internal(&ret,key,block,bx);
      pos = find_slot(key,block);

      leaf_shift(block,pos);
      set_slot(key,val,block,pos);

      write_block(block,ret,bx);
      return 1;

   }

   leaf_shift(bx->root,pos);
   set_slot(key,val,bx->root,pos);

   return 1;
}

int get_bx_tree(long int key, struct bx_tree *bx) {
   long int curr_key;
   int val, curr_pointer, curr_val, pos;
   
   /* first search the root */
   pos = find_slot(key, bx->root);
   get_key(&curr_key,bx->root,pos);
   if (curr_key == key) {
      /* if the current key is equal to our key, we replace the value */
      get_val(&val,bx->root,pos);
      return val;
   }

   get_pointer(&curr_pointer,bx->root,pos);
   if (curr_pointer) {
      /* if there is a pointer we are going to decend */
      clear_parents(bx);
      add_parent(0,bx);

      safe_load_cache(curr_pointer,bx);

      /* now we enter the case where we are decending */
      while (1) {
         /* same as before */
         pos = find_slot(key, bx->cached);
         get_key(&curr_key,bx->cached,pos);
         if (curr_key == key) {
            /* replace old value */
            get_val(&val,bx->cached,pos);

            return val;
         }

         get_pointer(&curr_pointer,bx->cached,pos);
         if (curr_pointer) {
            /* decend */
            add_parent(bx->cache_pos,bx);
            safe_load_cache(curr_pointer,bx);
            continue;
         }

         
         /* we are done now */

         return INT_MIN;

      }
   }

   return INT_MIN;
}

int rem_bx_tree(long int key, struct bx_tree *bx) {
   long int curr_key;
   int curr_pointer, pos;
   int mod = 0;
   
   /* first search the root */
   pos = find_slot(key, bx->root);
   get_key(&curr_key,bx->root,pos);
   if (curr_key == key) {
      /* if the current key is equal to our key, we replace the value */
      set_val(INT_MIN,bx->root,pos);
      pos++; /* decend to this node's children */
      mod = 1;

      if (!is_internal(bx->root)) {
         /* if this is not an internal node we are done, and can exit */
         return 1;
      }
   }

   get_pointer(&curr_pointer,bx->root,pos);
   if (curr_pointer) {
      /* if there is a pointer we are going to decend */
      clear_parents(bx);
      add_parent(0,bx);

      safe_load_cache(curr_pointer,bx);

      /* now we enter the case where we are decending */
      while (1) {
         /* same as before */
         pos = find_slot(key, bx->cached);
         get_key(&curr_key,bx->cached,pos);
         if (curr_key == key) {
            /* replace old value */
            set_val(INT_MIN,bx->cached,pos);
            bx->cache_written = 1;
            pos++; /* decend to this node's children */
            mod = 1;

            if (!is_internal(bx->cached)) {
               /* we are done here */
               return 1;
            }
         }

         get_pointer(&curr_pointer,bx->cached,pos);
         if (curr_pointer && is_internal(bx->cached)) {
            /* decend */
            add_parent(bx->cache_pos,bx);
            safe_load_cache(curr_pointer,bx);
            continue;
         }

         /* now if the value is not here and we can't decend we need to add the
          * value here */
         
         /* we are done now */

         return mod;

      }
   }

   /* root is only node - add here - only call this once */

   return mod;
}

void shift_internal(char *block, int pos) {
   static int pointer;

   /* get pointer to replace */
   get_pointer(&pointer,block,pos);

   /* shift like normal */
   int relative = pos*16;
   memmove(block+(relative+16),block+relative,BLOCKSIZE-(relative+16));

   /* clear slot */
   memset(block+relative,0,16);

   /* readd pointer */
   set_pointer(pointer,block,pos);
}

void split_root_internal(int *ret, long int to_add, char *block, struct bx_tree *bx) {
   static int mid_val = MAX_ELEMENTS / 2;
   static int mid_pos = (MAX_ELEMENTS / 2) * 16;
   static char left[BLOCKSIZE];
   static char right[BLOCKSIZE];
   static int left_p;
   static int right_p;
   left_p = bx->total_blocks;
   right_p = bx->total_blocks+1;

   long int key;
   int val;
   int pointer;
   get_key(&key,bx->root,mid_val);
   get_val(&val,bx->root,mid_val);
   get_pointer(&pointer,bx->root,mid_val);

   /* copy into left */
   memcpy(left,bx->root,mid_pos);
   set_pointer(pointer,left,mid_val);

   /* copy into right */
   memcpy(right,bx->root+mid_pos,BLOCKSIZE-mid_pos);

   /* zero root */
   memset(bx->root,0,BLOCKSIZE);

   /* populate root */
   set_slot(key,val,bx->root,0);
   set_pointer(left_p,bx->root,0);
   set_pointer(right_p,bx->root,1);

   /* save new blocks */
   write_block(left,left_p,bx);
   write_block(right,right_p,bx);

   /* return values */
   if (key <= to_add) {
      *ret = right_p;
      load_block(block,right_p,bx);
   }
   else {
      *ret = left_p;
      load_block(block,left_p,bx);
   }
}

void add_to_root_parent(long int key, int val, int pointer, struct bx_tree *bx) {
   static int mid_val = MAX_ELEMENTS / 2;
   static int mid_pos = (MAX_ELEMENTS / 2) * 16;
   static char block[BLOCKSIZE];
   static int pos;
   static int new_pointer;

   if (is_full(bx->root)) {
      split_root_internal(&new_pointer,key,block,bx);
      pos = find_slot(key,block);
      shift_internal(block,pos);
      set_slot(key,val,block,pos);
      set_pointer(pointer,block,pos+1);
      write_block(block,new_pointer,bx);
      return;
   }

   pos = find_slot(key,bx->root);
   shift_internal(bx->root,pos);
   set_slot(key,val,bx->root,pos);
   set_pointer(pointer,bx->root,pos+1);

}

void split_internal(int *ret, long int to_add, int pointer, char *block, struct bx_tree *bx) {
   static char new_block[BLOCKSIZE];
   static int mid_val = MAX_ELEMENTS / 2;
   static int mid_pos = (MAX_ELEMENTS / 2) * 16;

   int old_pointer;
   long int key; 
   int val;
   int parent;

   /* setup */
   *ret = pointer;

   /* create new block */
   load_block(new_block,bx->total_blocks,bx);

   /* get 'duplicate' pointer - also other values */
   parent = rem_parent(bx);
   get_key(&key,block,mid_val);
   get_val(&val,block,mid_val);
   get_pointer(&old_pointer,block,mid_val);

   /* copy after mid to new block */
   memcpy(new_block,block+mid_pos,BLOCKSIZE-mid_pos);

   /* set portion to zero */
   memset(block+mid_pos,0,BLOCKSIZE-mid_pos);

   /* read duplicate pointer */
   set_pointer(old_pointer,block,mid_val);

   write_block(block,pointer,bx);
   write_block(new_block,bx->total_blocks-1,bx);

   /* take action based on where to add */
   if (key <= to_add) {
      load_block(block,bx->total_blocks-1,bx); /* load new_block */
      *ret = bx->total_blocks-1;
   }

   /* now we need to add to our parent */
   if (parent) {
      add_to_parent(key,val,bx->total_blocks-1,parent,bx);
   }
   else {
      add_to_root_parent(key,val,bx->total_blocks-1,bx);
   }
}

void add_to_parent(long int key, int val, int pointer, int parent, struct bx_tree *bx) {
   static int mid_val = MAX_ELEMENTS / 2;
   static int mid_pos = (MAX_ELEMENTS / 2) * 16;
   int pos;
   char block[BLOCKSIZE];
   int ret;

   /* setup */
   ret = parent;

   load_block(block,parent,bx);


   if (is_full(block)) {
      split_internal(&ret,key,parent,block,bx);
   }



   /* find position */
   pos = find_slot(key,block);
   shift_internal(block,pos);

   /* set values */
   set_slot(key,val,block,pos);
   set_pointer(pointer,block,pos+1);

   /* save changes */
   write_block(block,ret,bx);

}

void split_leaf(long int to_add, struct bx_tree *bx) {
   static int mid_val = MAX_ELEMENTS / 2;
   static int mid_pos = (MAX_ELEMENTS / 2) * 16;
   static char new_block[BLOCKSIZE];

   long int key;
   int val;
   int old_pointer;
   int parent;


   /* get realavant values */
   parent = rem_parent(bx);
   get_key(&key,bx->cached,mid_val);
   get_val(&val,bx->cached,mid_val);
   get_pointer(&old_pointer,bx->cached,MAX_ELEMENTS);

   /* point old block to new one */
   set_pointer(bx->total_blocks,bx->cached,MAX_ELEMENTS);

   /* create new block */
   load_block(new_block,bx->total_blocks,bx);

   /* copy after mid to new block - exclude final node */
   memcpy(new_block,bx->cached+mid_pos,BLOCKSIZE-(mid_pos+16));

   /* now set it to zero */
   memset(bx->cached+mid_pos,0,BLOCKSIZE-(mid_pos+16));
   bx->cache_written = 1;

   /* point new block to next one */
   set_pointer(old_pointer,new_block,MAX_ELEMENTS);

   write_block(new_block,bx->total_blocks-1,bx);

   if (key <= to_add) {
      safe_load_cache(bx->total_blocks-1,bx);
   }

   /* now we need to add to the parent */
   if (parent) {
      add_to_parent(key,val,bx->total_blocks-1,parent,bx);
   }
   else {
      add_to_root_parent(key,val,bx->total_blocks-1,bx);
   }

}

