#include "header.h"

short short_a[128];
int   out[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ out) {
  int i;
  for (i = 0; i < 128; i++) {
    out[i] = (int) short_a[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[128]);
  init_memory(&short_a[0], &short_a[128]);
  BENCH("Example10b", example10b(short_a,out), Mi*4/128*512, digest_memory(&out[0], &out[128]));
 
  return 0;
}
