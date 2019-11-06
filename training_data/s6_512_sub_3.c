#include "header.h"

int o[512] ALIGNED16;
int i1[512] ALIGNED16;
int i2[512] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<512-4; i++){
      o[i] = i1[i+1] -i2[i+3];
   }
}

int main(int argc,char* argv[]){
  init_memory(&o[0], &o[512]);
  init_memory(&i1[0], &i1[512]);
  init_memory(&i2[0], &i2[512]);
 
  BENCH("Example4b",  example4b(), Mi*2/512*512, digest_memory(&o[0], &o[512]));
  
  return 0;
}
