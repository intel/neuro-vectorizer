#include "header.h"

int output[8192] ALIGNED16;
int input1[8192] ALIGNED16;
int input2[8192] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<8192-4; i++){
      output[i] = input1[i+1] +input2[i+3];
   }
}

int main(int argc,char* argv[]){
  init_memory(&output[0], &output[8192]);
  init_memory(&input1[0], &input1[8192]);
  init_memory(&input2[0], &input2[8192]);
 
  BENCH("Example4b",  example4b(), Mi*2/8192*512, digest_memory(&output[0], &output[8192]));
  
  return 0;
}
