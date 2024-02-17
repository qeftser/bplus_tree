
#ifndef BXTREE

#define BXTREE 1
#include "b+definitions.h"

void start_bx_tree(struct bx_tree *);
void close_bx_tree(struct bx_tree *);
int add_bx_tree(long int, int, struct bx_tree *);
int get_bx_tree(long int, struct bx_tree *);
int rem_bx_tree(long int, struct bx_tree *);

#endif
