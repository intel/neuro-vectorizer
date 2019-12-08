#include "header.h"

int o[8192] ALIGNED16;
int i1[8192] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<8192-x; i++){
      o[i] *= i1[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&o[0], &o[8192]);
  init_memory(&i1[0], &i1[8192]);
 
  BENCH("Example7",   example7(4), Mi/8192*512, digest_memory(&o[0], &o[8192]));
  return 0;
}
