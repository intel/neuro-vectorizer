#include "header.h"

short short_a[4096];
short short_b[4096];
int   assign1[4096] ALIGNED16;
int   assign2[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, int* __restrict__ assign1, int* __restrict__ assign2) {
  int i;
  for (i = 0; i < 4096; i++) {
    assign1[i] = (int) short_a[i];
    assign2[i] = (int) short_b[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[4096]);
  init_memory(&assign2[0], &assign2[4096]);
  init_memory(&short_a[0], &short_a[4096]);
  init_memory(&short_b[0], &short_b[4096]);
  BENCH("Example10b", example10b(short_a,short_b,assign1,assign2), Mi*4/4096*512, digest_memory(&assign1[0], &assign1[4096])+digest_memory(&assign2[0], &assign2[4096]));
 
  return 0;
}
