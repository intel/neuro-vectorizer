#include "header.h"

int a[16384] ALIGNED16;
int b[16384] ALIGNED16;
int c[16384] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<16384-4; i++){
      a[i] = b[i+1] *c[i+3];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[16384]);
  init_memory(&b[0], &b[16384]);
  init_memory(&c[0], &c[16384]);
 
  BENCH("Example4b",  example4b(), Mi*2/16384*512, digest_memory(&a[0], &a[16384]));
  
  return 0;
}
