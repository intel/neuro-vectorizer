#include "header.h"

int o[2048] ALIGNED16;
int i1[2048] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<2048-x; i++){
      o[i] -= i1[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&o[0], &o[2048]);
  init_memory(&i1[0], &i1[2048]);
 
  BENCH("Example7",   example7(4), Mi/2048*512, digest_memory(&o[0], &o[2048]));
  return 0;
}
