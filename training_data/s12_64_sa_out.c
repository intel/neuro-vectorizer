#include "header.h"

short sa[64];
int   out[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ out) {
  int i;
  for (i = 0; i < 64; i++) {
    out[i] = (int) sa[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[64]);
  init_memory(&sa[0], &sa[64]);
  BENCH("Example10b", example10b(sa,out), Mi*4/64*512, digest_memory(&out[0], &out[64]));
 
  return 0;
}
