
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
