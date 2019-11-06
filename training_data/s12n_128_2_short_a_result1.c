#include "header.h"

short short_a[128];
short short_b[128];
int   result1[128] ALIGNED16;
int   result2[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 128; i+=2) {
    result1[i] = (int) short_a[i];
    result2[i] = (int) short_b[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[128]);
  init_memory(&result2[0], &result2[128]);
  init_memory(&short_a[0], &short_a[128]);
  init_memory(&short_b[0], &short_b[128]);
  BENCH("Example10b", example10b(short_a,short_b,result1,result2), Mi*4/128*512, digest_memory(&result1[0], &result1[128])+digest_memory(&result2[0], &result2[128]));
 
  return 0;
}
