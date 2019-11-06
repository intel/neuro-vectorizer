#include "header.h"

short input[8192];
int   out[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ out) {
  int i;
  for (i = 0; i < 8192; i+=2) {
    out[i] = (int) input[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[8192]);
  init_memory(&input[0], &input[8192]);
  BENCH("Example10b", example10b(input,out), Mi*4/8192*512, digest_memory(&out[0], &out[8192]));
 
  return 0;
}
