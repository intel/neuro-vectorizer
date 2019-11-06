#include "header.h"

short short_a[4096];
short short_b[4096];
int   result1[4096] ALIGNED16;
int   result2[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 4096-1; i+=2) {
    result1[i] = (int) short_a[i];
result1[i+1] = (int) short_a[i+1];
    result2[i] = (int) short_b[i];
result2[i+1] = (int) short_b[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[4096]);
  init_memory(&result2[0], &result2[4096]);
  init_memory(&short_a[0], &short_a[4096]);
  init_memory(&short_b[0], &short_b[4096]);
  BENCH("Example10b", example10b(short_a,short_b,result1,result2), Mi*4/4096*512, digest_memory(&result1[0], &result1[4096])+digest_memory(&result2[0], &result2[4096]));
 
  return 0;
}
