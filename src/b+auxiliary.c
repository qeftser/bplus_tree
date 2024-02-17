
#include "b+auxiliary.h"

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

void leaf_shift(char *block, int from) {
   int pos = from*16;
   memmove(block+(pos+16),block+pos,BLOCKSIZE-(pos+32));

   /* clear slot */
   memset(block+pos,0,16);

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
   /* left points to right */
   set_pointer(right_p,left,MAX_ELEMENTS);

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

void add_to_parent(long int key, int val, int pointer, int parent, struct bx_tree *bx) {
   static int mid_val = MAX_ELEMENTS / 2;
   static int mid_pos = (MAX_ELEMENTS / 2) * 16;
   int pos;
   char block[BLOCKSIZE];
   int ret;

   /* setup */
   ret = parent;
   load_block(block,parent,bx);

   /* split if full */
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


