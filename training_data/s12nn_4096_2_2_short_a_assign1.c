#include "header.h"

short short_a[4096];
short short_b[4096];
short short_c[4096];
int   assign1[4096] ALIGNED16;
int   assign2[4096] ALIGNED16;
int   assign3[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, short *__restrict__ short_c, int* __restrict__ assign1, int* __restrict__ assign2, int* __restrict__ assign3) {
  int i;
  for (i = 0; i < 4096-1; i+=2) {
    assign1[i] = (int) short_a[i];
assign1[i+1] = (int) short_a[i+1];
    assign2[i] = (int) short_b[i];
assign2[i+1] = (int) short_b[i+1];
    assign3[i] = (int) short_c[i];
assign3[i+1] = (int) short_c[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[4096]);
  init_memory(&assign2[0], &assign2[4096]);
  init_memory(&assign3[0], &assign3[4096]);
  init_memory(&short_a[0], &short_a[4096]);
  init_memory(&short_b[0], &short_b[4096]);
  init_memory(&short_c[0], &short_c[4096]);
  BENCH("Example10b", example10b(short_a,short_b,short_c,assign1,assign2,assign3), Mi*4/4096*512, digest_memory(&assign1[0], &assign1[4096])+digest_memory(&assign2[0], &assign2[4096])+digest_memory(&assign3[0], &assign3[4096]));
 
  return 0;
}
