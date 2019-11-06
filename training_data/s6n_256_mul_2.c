#include "header.h"

int output[256] ALIGNED16;
int input1[256] ALIGNED16;
int input2[256] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<256-4; i+=2){
      output[i] = input1[i+1] *input2[i+3];
      output[i+1]=input1[i] *input2[i+2];
   }
}

int main(int argc,char* argv[]){
  init_memory(&output[0], &output[256]);
  init_memory(&input1[0], &input1[256]);
  init_memory(&input2[0], &input2[256]);
 
  BENCH("Example4b",  example4b(), Mi*2/256*512, digest_memory(&output[0], &output[256]));
  
  return 0;
}
