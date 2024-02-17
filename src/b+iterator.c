
#include "b+auxiliary.h"
#include "b+tree.h"
#include "b+iterator.h"

void basic_next_bx_iterator(struct bx_iterator *);
int basic_has_next_bx_iterator(struct bx_iterator *);

void invalidate_bx_iterators(struct bx_tree *bx) {
   bx->invalidation = bx->total_iterators;
}

void start_bx_iterator(struct bx_tree *bx, struct bx_iterator *bxi) {
   static int pointer;
   static int val;

   /* write changes before using */
   write_block(bx->cached,bx->cache_pos,bx);

   /* set nessesary bookeeping variables */
   bxi->id = ++(bx->total_iterators);
   bxi->key = 0;
   bxi->val = 0;
   bxi->idx = 0;
   bxi->bx = bx;

   /* decend to lowest layer */
   memcpy(bxi->cache,bx->root,BLOCKSIZE);
   while (is_internal(bxi->cache)) {
      get_pointer(&pointer,bxi->cache,0);
      load_block(bxi->cache,pointer,bxi->bx);
   }

   /* pointer to next block */
   get_pointer(&pointer,bxi->cache,MAX_ELEMENTS);
   bxi->next = pointer;

   /* iterate out to the first actual value */
   get_val(&val,bxi->cache,bxi->idx);
   while (has_next_bx_iterator(bxi) && val == INT_MIN) {
      basic_next_bx_iterator(bxi);
      get_val(&val,bxi->cache,bxi->idx);
   }
   basic_next_bx_iterator(bxi);
}

int valid_bx_iterator(struct bx_iterator *bxi) {
   return bxi->id > bxi->bx->invalidation;
}

int has_next_bx_iterator(struct bx_iterator *bxi) {
   return !((!bxi->next_key || bxi->idx == MAX_ELEMENTS) && bxi->next == 0);
}

void basic_next_bx_iterator(struct bx_iterator *bxi) {
   if (bxi->idx == MAX_ELEMENTS) {
      load_block(bxi->cache,bxi->next,bxi->bx);
      get_pointer(&bxi->next,bxi->cache,MAX_ELEMENTS);
      bxi->idx = 0;
   }
   bxi->idx++;

   /* set values and skip removed values - recursive */
   get_val(&bxi->next_val,bxi->cache,bxi->idx-1);
   get_key(&bxi->next_key,bxi->cache,bxi->idx-1);
}

void next_bx_iterator(struct bx_iterator *bxi) {
   bxi->key = bxi->next_key;
   bxi->val = bxi->next_val;
   do {
      basic_next_bx_iterator(bxi);
   } while (has_next_bx_iterator(bxi) && bxi->next_key == 0 || bxi->next_val == INT_MIN);
}

/* next is really the only one that does much */
int get_val_bx_iterator(struct bx_iterator *bxi) {
   return bxi->val;
}

long int get_key_bx_iterator(struct bx_iterator *bxi) {
   return bxi->key;
}

void rem_bx_iterator(struct bx_iterator *bxi) {
   rem_bx_tree(bxi->key,bxi->bx);
}

void set_bx_iterator(int val, struct bx_iterator *bxi) {
   add_bx_tree(bxi->key,val,bxi->bx);
}


