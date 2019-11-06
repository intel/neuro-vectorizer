#include "header.h"

int output[64] ALIGNED16;
int input1[64] ALIGNED16;
int input2[64] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<64; i++){
    output[i] = input1[i] +input2[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&output[0], &output[64]);
  init_memory(&input1[0], &input1[64]);
  init_memory(&input2[0], &input2[64]);
  BENCH("Example1",   example1(), Mi*4/64*256, digest_memory(&output[0], &output[64]));
  
  
  return 0;
}
