#include "header.h"

short input[64];
int   assign[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 64-3; i+=4) {
    assign[i] = (int) input[i];
assign[i+1] = (int) input[i+1];
assign[i+2] = (int) input[i+2];
assign[i+3] = (int) input[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[64]);
  init_memory(&input[0], &input[64]);
  BENCH("Example10b", example10b(input,assign), Mi*4/64*512, digest_memory(&assign[0], &assign[64]));
 
  return 0;
}
