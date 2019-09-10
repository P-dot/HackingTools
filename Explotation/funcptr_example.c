#include <stdio.h> 

int func_one() {
   printf("This is function one\n");
   return 1;
}

int func_two() {
   printf("This is function two\n");
   return 2;
}

int main() {
   int value; 
   int (*function_ptr) ();

   function_ptr = func_one;
   printf("functions_ptr is 0x%08x\n", function_ptr);
   value = function_ptr();
   printf("value returned was %d\n", value);

   function_ptr = func_two;
   printf("function_ptris 0x%08x\n", function_ptr);
   value = function_ptr();
   printf("value returned was %d\n", value);
}
