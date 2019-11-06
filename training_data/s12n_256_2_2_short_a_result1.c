#include "header.h"

short short_a[256];
short short_b[256];
int   result1[256] ALIGNED16;
int   result2[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 256-1; i+=2) {
    result1[i] = (int) short_a[i];
result1[i+1] = (int) short_a[i+1];
    result2[i] = (int) short_b[i];
result2[i+1] = (int) short_b[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[256]);
  init_memory(&result2[0], &result2[256]);
  init_memory(&short_a[0], &short_a[256]);
  init_memory(&short_b[0], &short_b[256]);
  BENCH("Example10b", example10b(short_a,short_b,result1,result2), Mi*4/256*512, digest_memory(&result1[0], &result1[256])+digest_memory(&result2[0], &result2[256]));
 
  return 0;
}
