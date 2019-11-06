#include "header.h"

short sa[16384];
int   out[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ out) {
  int i;
  for (i = 0; i < 16384; i++) {
    out[i] = (int) sa[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[16384]);
  init_memory(&sa[0], &sa[16384]);
  BENCH("Example10b", example10b(sa,out), Mi*4/16384*512, digest_memory(&out[0], &out[16384]));
 
  return 0;
}
