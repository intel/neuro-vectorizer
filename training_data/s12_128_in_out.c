#include "header.h"

short in[128];
int   out[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ out) {
  int i;
  for (i = 0; i < 128; i++) {
    out[i] = (int) in[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[128]);
  init_memory(&in[0], &in[128]);
  BENCH("Example10b", example10b(in,out), Mi*4/128*512, digest_memory(&out[0], &out[128]));
 
  return 0;
}
