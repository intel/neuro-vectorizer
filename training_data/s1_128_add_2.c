#include "header.h"

int output[128] ALIGNED16;
int input1[128] ALIGNED16;
int input2[128] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<128; i++){
    output[i] = input1[i] +input2[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&output[0], &output[128]);
  init_memory(&input1[0], &input1[128]);
  init_memory(&input2[0], &input2[128]);
  BENCH("Example1",   example1(), Mi*4/128*256, digest_memory(&output[0], &output[128]));
  
  
  return 0;
}
