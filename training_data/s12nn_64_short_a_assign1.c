#include "header.h"

short short_a[64];
short short_b[64];
short short_c[64];
int   assign1[64] ALIGNED16;
int   assign2[64] ALIGNED16;
int   assign3[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, short *__restrict__ short_c, int* __restrict__ assign1, int* __restrict__ assign2, int* __restrict__ assign3) {
  int i;
  for (i = 0; i < 64; i++) {
    assign1[i] = (int) short_a[i];
    assign2[i] = (int) short_b[i];
    assign3[i] = (int) short_c[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[64]);
  init_memory(&assign2[0], &assign2[64]);
  init_memory(&assign3[0], &assign3[64]);
  init_memory(&short_a[0], &short_a[64]);
  init_memory(&short_b[0], &short_b[64]);
  init_memory(&short_c[0], &short_c[64]);
  BENCH("Example10b", example10b(short_a,short_b,short_c,assign1,assign2,assign3), Mi*4/64*512, digest_memory(&assign1[0], &assign1[64])+digest_memory(&assign2[0], &assign2[64])+digest_memory(&assign3[0], &assign3[64]));
 
  return 0;
}
