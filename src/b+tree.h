
#ifndef BXTREE

#define BXTREE 1
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>

#define BLOCKSIZE 128
#define MAX_ELEMENTS (BLOCKSIZE-16) / 16

struct bx_tree { char *filepath; int file; char root[4096]; char cached[4096]; int cache_written;
                 int root_pos; int cache_pos; int total_blocks; int parent_stack[8]; int parents; };
void start_bx_tree(struct bx_tree *);
void close_bx_tree(struct bx_tree *);
int add_bx_tree(long int, int, struct bx_tree *);
long int get_bx_tree(long int, struct bx_tree *);
int rem_bx_tree(long int, struct bx_tree *);

/* should be private - here for testing */
void load_block(char *, long int, struct bx_tree *);
void safe_load_cache(long int, struct bx_tree *);
void get_key(long int *, char *, int);
void get_val(int *, char *, int);
void get_pointer(int *, char *, int);
void set_slot(long int, int, char *, int);
void set_pointer(int, char *, int);
void inc_elements(char *);
int get_elements(char *block);
int get_parent(struct bx_tree *);
int is_full(char *);
void shift(char *, int);
void split(char *, struct bx_tree *);
long int split_root(struct bx_tree *);
void print_block(char *block);

#endif
