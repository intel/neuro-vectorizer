#include "header.h"

short short_a[4096];
int   result[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ result) {
  int i;
  for (i = 0; i < 4096-1; i+=2) {
    result[i] = (int) short_a[i];
result[i+1] = (int) short_a[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[4096]);
  init_memory(&short_a[0], &short_a[4096]);
  BENCH("Example10b", example10b(short_a,result), Mi*4/4096*512, digest_memory(&result[0], &result[4096]));
 
  return 0;
}
