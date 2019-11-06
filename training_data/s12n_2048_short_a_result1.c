#include "header.h"

short short_a[2048];
short short_b[2048];
int   result1[2048] ALIGNED16;
int   result2[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 2048; i++) {
    result1[i] = (int) short_a[i];
    result2[i] = (int) short_b[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[2048]);
  init_memory(&result2[0], &result2[2048]);
  init_memory(&short_a[0], &short_a[2048]);
  init_memory(&short_b[0], &short_b[2048]);
  BENCH("Example10b", example10b(short_a,short_b,result1,result2), Mi*4/2048*512, digest_memory(&result1[0], &result1[2048])+digest_memory(&result2[0], &result2[2048]));
 
  return 0;
}
