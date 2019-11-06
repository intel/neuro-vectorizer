#include "header.h"

int output[128] ALIGNED16;
int input1[128] ALIGNED16;
int input2[128] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<128-4; i++){
      output[i] = input1[i+1] *input2[i+3];
   }
}

int main(int argc,char* argv[]){
  init_memory(&output[0], &output[128]);
  init_memory(&input1[0], &input1[128]);
  init_memory(&input2[0], &input2[128]);
 
  BENCH("Example4b",  example4b(), Mi*2/128*512, digest_memory(&output[0], &output[128]));
  
  return 0;
}
