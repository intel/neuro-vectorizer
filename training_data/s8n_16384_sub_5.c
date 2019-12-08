#include "header.h"

int o[16384] ALIGNED16;
int i1[16384] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<16384-x; i++){
      o[i] -= i1[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&o[0], &o[16384]);
  init_memory(&i1[0], &i1[16384]);
 
  BENCH("Example7",   example7(4), Mi/16384*512, digest_memory(&o[0], &o[16384]));
  return 0;
}
