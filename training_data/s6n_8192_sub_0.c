#include "header.h"

int a[8192] ALIGNED16;
int b[8192] ALIGNED16;
int c[8192] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<8192-4; i+=2){
      a[i] = b[i+1] -c[i+3];
      a[i+1]=b[i] -c[i+2];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[8192]);
  init_memory(&b[0], &b[8192]);
  init_memory(&c[0], &c[8192]);
 
  BENCH("Example4b",  example4b(), Mi*2/8192*512, digest_memory(&a[0], &a[8192]));
  
  return 0;
}
