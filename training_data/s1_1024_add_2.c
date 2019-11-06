#include "header.h"

int output[1024] ALIGNED16;
int input1[1024] ALIGNED16;
int input2[1024] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<1024; i++){
    output[i] = input1[i] +input2[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&output[0], &output[1024]);
  init_memory(&input1[0], &input1[1024]);
  init_memory(&input2[0], &input2[1024]);
  BENCH("Example1",   example1(), Mi*4/1024*256, digest_memory(&output[0], &output[1024]));
  
  
  return 0;
}
