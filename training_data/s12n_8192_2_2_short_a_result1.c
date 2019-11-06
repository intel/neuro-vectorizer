#include "header.h"

short short_a[8192];
short short_b[8192];
int   result1[8192] ALIGNED16;
int   result2[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 8192-1; i+=2) {
    result1[i] = (int) short_a[i];
result1[i+1] = (int) short_a[i+1];
    result2[i] = (int) short_b[i];
result2[i+1] = (int) short_b[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[8192]);
  init_memory(&result2[0], &result2[8192]);
  init_memory(&short_a[0], &short_a[8192]);
  init_memory(&short_b[0], &short_b[8192]);
  BENCH("Example10b", example10b(short_a,short_b,result1,result2), Mi*4/8192*512, digest_memory(&result1[0], &result1[8192])+digest_memory(&result2[0], &result2[8192]));
 
  return 0;
}
