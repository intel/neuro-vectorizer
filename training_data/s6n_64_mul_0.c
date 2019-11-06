#include "header.h"

int a[64] ALIGNED16;
int b[64] ALIGNED16;
int c[64] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<64-4; i+=2){
      a[i] = b[i+1] *c[i+3];
      a[i+1]=b[i] *c[i+2];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[64]);
  init_memory(&b[0], &b[64]);
  init_memory(&c[0], &c[64]);
 
  BENCH("Example4b",  example4b(), Mi*2/64*512, digest_memory(&a[0], &a[64]));
  
  return 0;
}
