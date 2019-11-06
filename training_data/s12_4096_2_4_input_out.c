#include "header.h"

short input[4096];
int   out[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ out) {
  int i;
  for (i = 0; i < 4096-3; i+=4) {
    out[i] = (int) input[i];
out[i+1] = (int) input[i+1];
out[i+2] = (int) input[i+2];
out[i+3] = (int) input[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[4096]);
  init_memory(&input[0], &input[4096]);
  BENCH("Example10b", example10b(input,out), Mi*4/4096*512, digest_memory(&out[0], &out[4096]));
 
  return 0;
}
