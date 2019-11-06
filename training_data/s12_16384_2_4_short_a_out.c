#include "header.h"

short short_a[16384];
int   out[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ out) {
  int i;
  for (i = 0; i < 16384-3; i+=4) {
    out[i] = (int) short_a[i];
out[i+1] = (int) short_a[i+1];
out[i+2] = (int) short_a[i+2];
out[i+3] = (int) short_a[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[16384]);
  init_memory(&short_a[0], &short_a[16384]);
  BENCH("Example10b", example10b(short_a,out), Mi*4/16384*512, digest_memory(&out[0], &out[16384]));
 
  return 0;
}
