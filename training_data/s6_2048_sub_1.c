#include "header.h"

int out[2048] ALIGNED16;
int in1[2048] ALIGNED16;
int in2[2048] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<2048-4; i++){
      out[i] = in1[i+1] -in2[i+3];
   }
}

int main(int argc,char* argv[]){
  init_memory(&out[0], &out[2048]);
  init_memory(&in1[0], &in1[2048]);
  init_memory(&in2[0], &in2[2048]);
 
  BENCH("Example4b",  example4b(), Mi*2/2048*512, digest_memory(&out[0], &out[2048]));
  
  return 0;
}
