#include "header.h"

short short_a[2048];
int   out[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ out) {
  int i;
  for (i = 0; i < 2048; i+=2) {
    out[i] = (int) short_a[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[2048]);
  init_memory(&short_a[0], &short_a[2048]);
  BENCH("Example10b", example10b(short_a,out), Mi*4/2048*512, digest_memory(&out[0], &out[2048]));
 
  return 0;
}
