
#include "b+tree.h"

int main(void) {
   
   if (!access("test.txt",F_OK)) {
      printf("removing old file\n");
      system("rm test.txt");
   }

   struct bx_tree bx;
   bx.filepath = "test.txt";
   start_bx_tree(&bx);

   add_bx_tree(140,10,&bx);
   add_bx_tree(120,10,&bx);
   add_bx_tree(100,10,&bx);
   add_bx_tree(80,10,&bx);
   add_bx_tree(60,10,&bx);
   add_bx_tree(40,10,&bx);
   add_bx_tree(20,10,&bx);
   add_bx_tree(10,10,&bx);
   add_bx_tree(200,10,&bx);
   add_bx_tree(15,10,&bx);
   add_bx_tree(16,10,&bx);
   add_bx_tree(17,10,&bx);
   add_bx_tree(18,10,&bx);
   add_bx_tree(12,10,&bx);
   add_bx_tree(13,10,&bx);
   add_bx_tree(70,10,&bx);
   add_bx_tree(71,10,&bx);
   add_bx_tree(72,10,&bx);
   add_bx_tree(73,10,&bx);
   add_bx_tree(74,10,&bx);
   add_bx_tree(75,10,&bx);
   add_bx_tree(210,10,&bx);
   add_bx_tree(220,10,&bx);
   add_bx_tree(230,10,&bx);
   add_bx_tree(1,10,&bx);
   add_bx_tree(2,10,&bx);
   add_bx_tree(3,10,&bx);
   add_bx_tree(130,10,&bx);
   add_bx_tree(131,10,&bx);
   add_bx_tree(133,10,&bx);
   add_bx_tree(123,10,&bx);
   add_bx_tree(132,10,&bx);
   add_bx_tree(240,10,&bx);
   add_bx_tree(250,10,&bx);
   add_bx_tree(260,10,&bx);
   add_bx_tree(215,10,&bx);
   add_bx_tree(270,10,&bx);
   add_bx_tree(280,10,&bx);
   add_bx_tree(290,10,&bx);

   print_block(bx.root);
   safe_load_cache(1,&bx);
   print_block(bx.cached);
   safe_load_cache(2,&bx);
   print_block(bx.cached);
   safe_load_cache(3,&bx);
   print_block(bx.cached);
   safe_load_cache(4,&bx);
   print_block(bx.cached);
   safe_load_cache(5,&bx);
   print_block(bx.cached);
   safe_load_cache(6,&bx);
   print_block(bx.cached);
   safe_load_cache(7,&bx);
   print_block(bx.cached);
   safe_load_cache(8,&bx);
   print_block(bx.cached);
   safe_load_cache(9,&bx);
   print_block(bx.cached);
   safe_load_cache(10,&bx);
   print_block(bx.cached);
   safe_load_cache(11,&bx);
   print_block(bx.cached);
   safe_load_cache(12,&bx);
   print_block(bx.cached);

   close_bx_tree(&bx);


   return 0;
}
