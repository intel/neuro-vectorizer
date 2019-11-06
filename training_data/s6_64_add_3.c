#include "header.h"

int o[64] ALIGNED16;
int i1[64] ALIGNED16;
int i2[64] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<64-4; i++){
      o[i] = i1[i+1] +i2[i+3];
   }
}

int main(int argc,char* argv[]){
  init_memory(&o[0], &o[64]);
  init_memory(&i1[0], &i1[64]);
  init_memory(&i2[0], &i2[64]);
 
  BENCH("Example4b",  example4b(), Mi*2/64*512, digest_memory(&o[0], &o[64]));
  
  return 0;
}
