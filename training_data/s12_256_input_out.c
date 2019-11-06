#include "header.h"

short input[256];
int   out[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ out) {
  int i;
  for (i = 0; i < 256; i++) {
    out[i] = (int) input[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[256]);
  init_memory(&input[0], &input[256]);
  BENCH("Example10b", example10b(input,out), Mi*4/256*512, digest_memory(&out[0], &out[256]));
 
  return 0;
}
