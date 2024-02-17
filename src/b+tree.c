
#include "b+auxiliary.h"
#include "b+tree.h"

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
   int curr_pointer, pos;
   
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
   int val, curr_pointer, pos;
   
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

