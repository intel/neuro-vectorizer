#include "header.h"

short input[64];
int   out[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ out) {
  int i;
  for (i = 0; i < 64-1; i+=2) {
    out[i] = (int) input[i];
out[i+1] = (int) input[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[64]);
  init_memory(&input[0], &input[64]);
  BENCH("Example10b", example10b(input,out), Mi*4/64*512, digest_memory(&out[0], &out[64]));
 
  return 0;
}
