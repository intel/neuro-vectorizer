#include "header.h"

short short_a[256];
int   result[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ result) {
  int i;
  for (i = 0; i < 256-1; i+=2) {
    result[i] = (int) short_a[i];
result[i+1] = (int) short_a[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[256]);
  init_memory(&short_a[0], &short_a[256]);
  BENCH("Example10b", example10b(short_a,result), Mi*4/256*512, digest_memory(&result[0], &result[256]));
 
  return 0;
}
