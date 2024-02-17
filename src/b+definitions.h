
#ifndef BX_DEFINITIONS

#define BX_DEFINITIONS
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>

#define BLOCKSIZE 4096 
#define MAX_ELEMENTS (BLOCKSIZE-16) / 16


struct bx_tree { char *filepath; int file; char root[BLOCKSIZE]; char cached[BLOCKSIZE]; int cache_written;
                 int root_pos; int cache_pos; int total_blocks; int parent_stack[32]; int parents; 
                 int total_iterators; int invalidation; };

#endif
