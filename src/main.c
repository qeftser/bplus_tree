
#include "b+tree.h"

int main(void) {
   
   if (!access("test.txt",F_OK)) {
      printf("removing old file\n");
      system("rm test.txt");
   }

   struct bx_tree bx;
   bx.filepath = "test.txt";
   start_bx_tree(&bx);

   /*
   add_bx_tree(10,5,&bx);
   add_bx_tree(20,5,&bx);
   add_bx_tree(30,5,&bx);
   add_bx_tree(40,5,&bx);
   add_bx_tree(50,5,&bx);
   add_bx_tree(60,5,&bx);
   add_bx_tree(70,5,&bx);
   add_bx_tree(80,5,&bx);
   add_bx_tree(90,5,&bx);
   add_bx_tree(100,5,&bx);
   add_bx_tree(110,5,&bx);
   add_bx_tree(120,5,&bx);
   add_bx_tree(130,5,&bx);
   add_bx_tree(140,5,&bx);
   add_bx_tree(150,5,&bx);
   add_bx_tree(160,5,&bx);
   add_bx_tree(170,5,&bx);
   add_bx_tree(180,5,&bx);
   add_bx_tree(190,5,&bx);
   add_bx_tree(200,5,&bx);
   add_bx_tree(210,5,&bx);
   add_bx_tree(220,5,&bx);
   add_bx_tree(230,5,&bx);
   add_bx_tree(240,5,&bx);
   add_bx_tree(250,5,&bx);
   add_bx_tree(260,5,&bx);
   add_bx_tree(270,5,&bx);
   add_bx_tree(280,5,&bx);
   add_bx_tree(290,5,&bx);
   add_bx_tree(300,5,&bx);
   add_bx_tree(310,5,&bx);
   add_bx_tree(320,5,&bx);
   add_bx_tree(330,5,&bx);
   add_bx_tree(340,5,&bx);
   add_bx_tree(350,5,&bx);
   add_bx_tree(360,5,&bx);
   add_bx_tree(370,5,&bx);
   add_bx_tree(380,5,&bx);
   add_bx_tree(390,5,&bx);

   write_block(bx.root,0,&bx);
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
   safe_load_cache(13,&bx);
   print_block(bx.cached);
   safe_load_cache(14,&bx);
   print_block(bx.cached);
   safe_load_cache(15,&bx);
   print_block(bx.cached);
   safe_load_cache(16,&bx);
   print_block(bx.cached);
   safe_load_cache(17,&bx);
   print_block(bx.cached);
   safe_load_cache(18,&bx);
   print_block(bx.cached);
   safe_load_cache(19,&bx);
   print_block(bx.cached);
   safe_load_cache(20,&bx);
   print_block(bx.cached);
   safe_load_cache(21,&bx);
   print_block(bx.cached);
   safe_load_cache(22,&bx);
   print_block(bx.cached);
   safe_load_cache(23,&bx);
   print_block(bx.cached);
   safe_load_cache(24,&bx);
   print_block(bx.cached);
   safe_load_cache(25,&bx);
   print_block(bx.cached);
   safe_load_cache(26,&bx);
   print_block(bx.cached);
   safe_load_cache(27,&bx);
   print_block(bx.cached);
   safe_load_cache(28,&bx);
   print_block(bx.cached);
   safe_load_cache(29,&bx);
   print_block(bx.cached);
   safe_load_cache(30,&bx);
   print_block(bx.cached);

   for (int i = 1; i < 40; i++) {
      if (get_bx_tree(i*10,&bx) == INT_MIN)
         printf("failed: %d\n",i*10);
   }

   */

   /*
   add_bx_tree(100,100,&bx);
   add_bx_tree(10,5,&bx);
   add_bx_tree(20,5,&bx);
   add_bx_tree(30,5,&bx);
   add_bx_tree(40,5,&bx);
   add_bx_tree(50,5,&bx);
   add_bx_tree(60,5,&bx);
   add_bx_tree(70,5,&bx);
   add_bx_tree(80,5,&bx);
   add_bx_tree(90,5,&bx);

   add_bx_tree(7,5,&bx);
   add_bx_tree(14,5,&bx);
   add_bx_tree(21,5,&bx);
   add_bx_tree(28,5,&bx);
   add_bx_tree(35,5,&bx);
   add_bx_tree(42,5,&bx);
   add_bx_tree(49,5,&bx);
   add_bx_tree(56,5,&bx);
   add_bx_tree(63,5,&bx);
   add_bx_tree(70,10,&bx);
   add_bx_tree(77,10,&bx);
   add_bx_tree(84,10,&bx);
   add_bx_tree(91,10,&bx);

   write_block(bx.root,0,&bx);
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
   */



   printf("add test - ");
   if (add_bx_tree(100,100,&bx)) printf("PASSED\n");
   else printf("FAILED\n");

   printf("get test - ");
   if (get_bx_tree(100,&bx) == 100) printf("PASSED\n");
   else printf("FAILED\n");

   printf("get test 2 - ");
   if (get_bx_tree(120,&bx) == INT_MIN) printf("PASSED\n");
   else printf("FAILED\n");

   printf("rem test - ");
   if (rem_bx_tree(100,&bx) == 1) printf("PASSED\n");
   else printf("FAILED\n");

   printf("rem test 2 - ");
   if (rem_bx_tree(120,&bx) == 0) printf("PASSED\n");
   else printf("FAILED\n");

   printf("get test 3 - ");
   if (get_bx_tree(100,&bx) == INT_MIN) printf("PASSED\n");
   else printf("FAILED\n");

   printf("add to split test - ");
   for (int i = 1; i < MAX_ELEMENTS+5; i++) {
      add_bx_tree(i*10,i*5,&bx);
   }
   printf("PASSED\n"); // it will crash on failure or fail later 

   int ret = 1;
   printf("get after split test - ");
   for (int i = 1; i < MAX_ELEMENTS+5; i++) {
      if (get_bx_tree(i*10,&bx) != i*5) {
         ret = 0; break;
      }
   }
   if (ret) printf("PASSED\n");
   else printf("FAILED\n");

   printf("add to split 2 test - ");
   for (int i = 1; i < MAX_ELEMENTS*(MAX_ELEMENTS+5); i++) {
      add_bx_tree(i*7,i*5,&bx); // minimum overlap with previous elements 
   }
   printf("PASSED\n");

   add_bx_tree(112,10000,&bx);

   ret = 1;
   printf("get after split 2 test - ");
   for (int i = 1; i < MAX_ELEMENTS+(MAX_ELEMENTS+5); i++) {
      if (get_bx_tree(i*7,&bx) != i*5) {
         printf("i: %d\n",i*7);
         ret = 0; //break;
      }
   }
   if (ret) printf("PASSED\n");
   else printf("FAILED\n");

   printf("add to split 3 test - ");
   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS*(MAX_ELEMENTS+5); i++) {
      add_bx_tree(i,i*5,&bx); // minimum overlap with previous elements 
   }
   printf("PASSED\n");

   ret = 1;
   printf("get after split 3 test - ");
   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS+(MAX_ELEMENTS+5); i++) {
      if (get_bx_tree(i,&bx) != i*5) {
         printf("i: %d\n",i);
         ret = 0; //break;
      }
   }
   if (ret) printf("PASSED\n");
   else printf("FAILED\n");

   printf("close test - ");
   close_bx_tree(&bx);
   printf("PASSED\n");

   printf("reopen test - ");
   start_bx_tree(&bx);
   printf("PASSED\n");

   ret = 1;
   printf("get after reopening test - ");
   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS+(MAX_ELEMENTS+5); i++) {
      if (get_bx_tree(i,&bx) != i*5) {
         printf("i: %d\n",i);
         ret = 0; //break;
      }
   }
   if (ret) printf("PASSED\n");
   else printf("FAILED\n");


   ret = 1;
   printf("remove all test - ");
   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS+(MAX_ELEMENTS+5); i++) {
      if (rem_bx_tree(i,&bx) != 1) {
         ret = 0; break;
      }
   }
   if (ret) printf("PASSED\n");
   else printf("FAILED\n");

   ret = 1;
   printf("get after removing test - ");
   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS+(MAX_ELEMENTS+5); i++) {
      if (get_bx_tree(i,&bx) != INT_MIN) {
         printf("i: %d\n",i);
         ret = 0; //break;
      }
   }
   if (ret) printf("PASSED\n");
   else printf("FAILED\n");


   close_bx_tree(&bx);


   return 0;
}
