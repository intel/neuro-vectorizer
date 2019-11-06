#include "header.h"

short input[512];
int   assign[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 512; i+=2) {
    assign[i] = (int) input[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[512]);
  init_memory(&input[0], &input[512]);
  BENCH("Example10b", example10b(input,assign), Mi*4/512*512, digest_memory(&assign[0], &assign[512]));
 
  return 0;
}
