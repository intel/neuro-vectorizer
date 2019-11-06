#include "header.h"

short short_a[4096];
short short_b[4096];
int   out1[4096] ALIGNED16;
int   out2[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, int* __restrict__ out1, int* __restrict__ out2) {
  int i;
  for (i = 0; i < 4096; i+=2) {
    out1[i] = (int) short_a[i];
    out2[i] = (int) short_b[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[4096]);
  init_memory(&out2[0], &out2[4096]);
  init_memory(&short_a[0], &short_a[4096]);
  init_memory(&short_b[0], &short_b[4096]);
  BENCH("Example10b", example10b(short_a,short_b,out1,out2), Mi*4/4096*512, digest_memory(&out1[0], &out1[4096])+digest_memory(&out2[0], &out2[4096]));
 
  return 0;
}
