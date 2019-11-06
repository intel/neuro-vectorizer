#include "header.h"

short input[16384];
int   out[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ out) {
  int i;
  for (i = 0; i < 16384-3; i+=4) {
    out[i] = (int) input[i];
out[i+1] = (int) input[i+1];
out[i+2] = (int) input[i+2];
out[i+3] = (int) input[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[16384]);
  init_memory(&input[0], &input[16384]);
  BENCH("Example10b", example10b(input,out), Mi*4/16384*512, digest_memory(&out[0], &out[16384]));
 
  return 0;
}
