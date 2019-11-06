#include "header.h"

short short_a[8192];
int   out[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ out) {
  int i;
  for (i = 0; i < 8192; i++) {
    out[i] = (int) short_a[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[8192]);
  init_memory(&short_a[0], &short_a[8192]);
  BENCH("Example10b", example10b(short_a,out), Mi*4/8192*512, digest_memory(&out[0], &out[8192]));
 
  return 0;
}
