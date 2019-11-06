#include "header.h"

short input[4096];
int   out[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ out) {
  int i;
  for (i = 0; i < 4096; i+=2) {
    out[i] = (int) input[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[4096]);
  init_memory(&input[0], &input[4096]);
  BENCH("Example10b", example10b(input,out), Mi*4/4096*512, digest_memory(&out[0], &out[4096]));
 
  return 0;
}
