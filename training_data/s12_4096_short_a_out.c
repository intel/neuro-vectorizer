#include "header.h"

short short_a[4096];
int   out[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ out) {
  int i;
  for (i = 0; i < 4096; i++) {
    out[i] = (int) short_a[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[4096]);
  init_memory(&short_a[0], &short_a[4096]);
  BENCH("Example10b", example10b(short_a,out), Mi*4/4096*512, digest_memory(&out[0], &out[4096]));
 
  return 0;
}
