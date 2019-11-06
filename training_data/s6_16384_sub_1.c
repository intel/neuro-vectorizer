#include "header.h"

int out[16384] ALIGNED16;
int in1[16384] ALIGNED16;
int in2[16384] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<16384-4; i++){
      out[i] = in1[i+1] -in2[i+3];
   }
}

int main(int argc,char* argv[]){
  init_memory(&out[0], &out[16384]);
  init_memory(&in1[0], &in1[16384]);
  init_memory(&in2[0], &in2[16384]);
 
  BENCH("Example4b",  example4b(), Mi*2/16384*512, digest_memory(&out[0], &out[16384]));
  
  return 0;
}
