#include "header.h"

int a[2048] ALIGNED16;
int b[2048] ALIGNED16;
int c[2048] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<2048-4; i++){
      a[i] = b[i+1] +c[i+3];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[2048]);
  init_memory(&b[0], &b[2048]);
  init_memory(&c[0], &c[2048]);
 
  BENCH("Example4b",  example4b(), Mi*2/2048*512, digest_memory(&a[0], &a[2048]));
  
  return 0;
}
