#include "header.h"

int output1[64] ALIGNED16;
int input1[64] ALIGNED16;
int input2[64] ALIGNED16;
int output2[64] ALIGNED16;

__attribute__((noinline))
void example11() {
   int i;
  for (i = 0; i < 64/4; i++){
    output1[i] = input1[2*i+1] * input2[2*i+1] - input1[2*i] * input2[2*i];
    output2[i] = input1[2*i] * input2[2*i+1] +input1[2*i+1] * input2[2*i];
  }
}

int main(int argc,char* argv[]){
  init_memory(&output1[0], &output1[64]);
  init_memory(&input1[0], &input1[64]);
  init_memory(&input2[0], &input2[64]);
  init_memory(&output2[0], &output2[64]);
  BENCH("Example11",  example11(), Mi*4/64*256, digest_memory(&output2[0], &output2[64]));
 
  return 0;
}
