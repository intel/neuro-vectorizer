#include "header.h"

int o[4096] ALIGNED16;
int i1[4096] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<4096-x; i++){
      o[i] *= i1[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&o[0], &o[4096]);
  init_memory(&i1[0], &i1[4096]);
 
  BENCH("Example7",   example7(4), Mi*8/4096*512, digest_memory(&o[0], &o[4096]));
  return 0;
}
