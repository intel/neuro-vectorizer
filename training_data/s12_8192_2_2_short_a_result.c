#include "header.h"

short short_a[8192];
int   result[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ result) {
  int i;
  for (i = 0; i < 8192-1; i+=2) {
    result[i] = (int) short_a[i];
result[i+1] = (int) short_a[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[8192]);
  init_memory(&short_a[0], &short_a[8192]);
  BENCH("Example10b", example10b(short_a,result), Mi*4/8192*512, digest_memory(&result[0], &result[8192]));
 
  return 0;
}
