#include "header.h"

int a[16384] ALIGNED16;
int b[16384] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<16384-x; i++){
      a[i] = b[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[16384]);
  init_memory(&b[0], &b[16384]);
 
  BENCH("Example7",   example7(4), Mi*8/16384*512, digest_memory(&a[0], &a[16384]));
  return 0;
}
