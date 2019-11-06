#include "header.h"

int o[2048] ALIGNED16;
int i1[2048] ALIGNED16;
int i2[2048] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<2048-4; i+=2){
      o[i] = i1[i+1] *i2[i+3];
      o[i+1]=i1[i] *i2[i+2];
   }
}

int main(int argc,char* argv[]){
  init_memory(&o[0], &o[2048]);
  init_memory(&i1[0], &i1[2048]);
  init_memory(&i2[0], &i2[2048]);
 
  BENCH("Example4b",  example4b(), Mi*2/2048*512, digest_memory(&o[0], &o[2048]));
  
  return 0;
}
