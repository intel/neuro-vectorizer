#include "header.h"

int a[512] ALIGNED16;
int b[512] ALIGNED16;
int c[512] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<512-4; i++){
      a[i] = b[i+1] +c[i+3];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[512]);
  init_memory(&b[0], &b[512]);
  init_memory(&c[0], &c[512]);
 
  BENCH("Example4b",  example4b(), Mi*2/512*512, digest_memory(&a[0], &a[512]));
  
  return 0;
}
