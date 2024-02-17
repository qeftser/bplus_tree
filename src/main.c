
#include "b+tree.h"
#include "b+iterator.h"

int main(void) {
   
   if (!access("test.txt",F_OK)) {
      printf("removing old file\n");
      system("rm test.txt");
   }

   struct bx_tree bx;
   bx.filepath = "test.txt";
   

   start_bx_tree(&bx);

   /*
   struct bx_iterator bxi;

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

   
   start_bx_iterator(&bx,&bxi);
   while (has_next_bx_iterator(&bxi)) {
      next_bx_iterator(&bxi);
      if (get_key_bx_iterator(&bxi) % 50 == 0) {
         set_bx_iterator(20,&bxi);
      }
      else if (get_key_bx_iterator(&bxi) % 50 == 10) {
         rem_bx_iterator(&bxi);
      }
   }
   write_block(bx.cached,bx.cache_pos,&bx);
   struct bx_iterator bxi_2;
   start_bx_iterator(&bx,&bxi_2);
   while (has_next_bx_iterator(&bxi_2)) {
      next_bx_iterator(&bxi_2);
      printf("key: %ld, val: %d\n",get_key_bx_iterator(&bxi_2),get_val_bx_iterator(&bxi_2));
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

   if (!access("test.txt",F_OK)) {
      printf("removing old file\n");
      system("rm test.txt");
   }

   struct bx_tree bx_2;
   bx_2.filepath = "test.txt";

   start_bx_tree(&bx_2);
   struct bx_iterator bxi_1, bxi_2, bxi_3, bxi_4;

   printf("start iterator test - ");
   start_bx_iterator(&bx_2,&bxi_1);
   printf("PASSED\n");

   printf("has next iterator test - ");
   if (has_next_bx_iterator(&bxi_1)) printf("FAILED\n");
   else printf("PASSED\n");

   for (int i = 1; i < MAX_ELEMENTS; i++) {
      add_bx_tree(i*10,i*5,&bx_2);
   }

   start_bx_iterator(&bx_2,&bxi_2);
   ret = 1;
   printf("get iterator test - ");
   for (int i = 1; i < MAX_ELEMENTS; i++) {
      next_bx_iterator(&bxi_2);
      if (get_key_bx_iterator(&bxi_2) != i*10 && get_val_bx_iterator(&bxi_2) != i*5) {
         ret = 0; break;
      }
   }
   if (ret) printf("PASSED\n");
   else printf("FAILED\n");

   for (int i = 1; i < MAX_ELEMENTS; i++) {
      rem_bx_tree(i*10,&bx_2);
   }

   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS; i++) {
      add_bx_tree(i*9,i*5,&bx_2);
   }

   start_bx_iterator(&bx_2,&bxi_2);
   ret = 1;
   printf("get iterator test 2 - ");
   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS; i++) {
      next_bx_iterator(&bxi_2);
      if (get_key_bx_iterator(&bxi_2) != i*9 && get_val_bx_iterator(&bxi_2) != i*5) {
         ret = 0; break;
      }
   }
   if (ret) printf("PASSED\n");
   else printf("FAILED\n");

   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS; i++) {
      rem_bx_tree(i*9,&bx_2);
   }

   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS*MAX_ELEMENTS; i++) {
      add_bx_tree(i*3,i*5,&bx_2);
   }

   start_bx_iterator(&bx_2,&bxi_2);
   ret = 1;
   printf("get iterator test 3 - ");
   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS*MAX_ELEMENTS; i++) {
      next_bx_iterator(&bxi_2);
      if (get_key_bx_iterator(&bxi_2) != i*3 && get_val_bx_iterator(&bxi_2) != i*5) {
         ret = 0; break;
      }
   }
   if (ret) printf("PASSED\n");
   else printf("FAILED\n");

   printf("set iterator test - ");
   start_bx_iterator(&bx_2,&bxi_3);
   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS*MAX_ELEMENTS; i++) {
      next_bx_iterator(&bxi_3);
      set_bx_iterator(100,&bxi_3);
   }
   ret = 1;
   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS*MAX_ELEMENTS; i++) {
      if (get_bx_tree(i*3,&bx_2) != 100) {
         ret = 0; break;
      }
   }
   if (ret) printf("PASSED\n");
   else printf("FAILED\n");

   printf("rem iterator test - ");
   start_bx_iterator(&bx_2,&bxi_3);
   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS*MAX_ELEMENTS; i++) {
      next_bx_iterator(&bxi_3);
      rem_bx_iterator(&bxi_3);
   }
   ret = 1;
   for (int i = 1; i < MAX_ELEMENTS*MAX_ELEMENTS*MAX_ELEMENTS; i++) {
      if (get_bx_tree(i*3,&bx_2) != INT_MIN) {
         ret = 0; break;
      }
   }
   if (ret) printf("PASSED\n");
   else printf("FAILED\n");

   close_bx_tree(&bx_2);

   return 0;
}
