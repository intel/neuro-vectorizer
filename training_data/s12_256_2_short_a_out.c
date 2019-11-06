#include "header.h"

short short_a[256];
int   out[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ out) {
  int i;
  for (i = 0; i < 256; i+=2) {
    out[i] = (int) short_a[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[256]);
  init_memory(&short_a[0], &short_a[256]);
  BENCH("Example10b", example10b(short_a,out), Mi*4/256*512, digest_memory(&out[0], &out[256]));
 
  return 0;
}
