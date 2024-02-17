
#ifndef BX_AUXILIARY

#define BX_AUXILIARY
#include "b+definitions.h"

void load_block(char *, long int, struct bx_tree *);
void write_block(char *, int, struct bx_tree *);
void safe_load_cache(long int, struct bx_tree *);

void get_key(long int *, char *, int);
void get_val(int *, char *, int);
void get_pointer(int *, char *, int);

void set_slot(long int, int, char *, int);
void set_key(long int, char *, int);
void set_val(int, char *, int);
void set_pointer(int, char *, int);

int get_parent(struct bx_tree *);
void add_parent(int, struct bx_tree *);
int rem_parent(struct bx_tree *);
void clear_parents(struct bx_tree *);

int is_full(char *);
int is_internal(char *);
int find_slot(long int, char *);

void leaf_shift(char *, int);
void shift_internal(char *, int); /* lol */

void split_root_internal(int *, long int, char *, struct bx_tree *);
void split_internal(int *, long int, int, char *, struct bx_tree *);
void split_leaf(long int, struct bx_tree *);

void add_to_parent(long int, int, int, int, struct bx_tree *);
void add_to_root_parent(long int, int, int, struct bx_tree *);

void print_block(char *);


#endif
