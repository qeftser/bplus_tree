
#ifndef BX_ITERATOR

#define BX_ITERATOR
#include "b+definitions.h"

struct bx_iterator { int id; long int key; int val; long int next_key; int next_val; 
                     int idx; int next; struct bx_tree *bx; char cache[BLOCKSIZE]; };

void invalidate_bx_iterators(struct bx_tree *); /* it is up to you to choose when to invalidate */

void start_bx_iterator(struct bx_tree *, struct bx_iterator *);
int valid_bx_iterator(struct bx_iterator *);

void next_bx_iterator(struct bx_iterator *);
int has_next_bx_iterator(struct bx_iterator *);
int get_val_bx_iterator(struct bx_iterator *);
long int get_key_bx_iterator(struct bx_iterator *);
void rem_bx_iterator(struct bx_iterator *);
void set_bx_iterator(int, struct bx_iterator *);

#endif
