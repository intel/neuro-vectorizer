#include "header.h"

int output[4096] ALIGNED16;
int input1[4096] ALIGNED16;
int input2[4096] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<4096-4; i+=2){
      output[i] = input1[i+1] *input2[i+3];
      output[i+1]=input1[i] *input2[i+2];
   }
}

int main(int argc,char* argv[]){
  init_memory(&output[0], &output[4096]);
  init_memory(&input1[0], &input1[4096]);
  init_memory(&input2[0], &input2[4096]);
 
  BENCH("Example4b",  example4b(), Mi*2/4096*512, digest_memory(&output[0], &output[4096]));
  
  return 0;
}
