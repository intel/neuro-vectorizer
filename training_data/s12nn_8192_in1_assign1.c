#include "header.h"

short in1[8192];
short in2[8192];
short in3[8192];
int   assign1[8192] ALIGNED16;
int   assign2[8192] ALIGNED16;
int   assign3[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, short *__restrict__ in3, int* __restrict__ assign1, int* __restrict__ assign2, int* __restrict__ assign3) {
  int i;
  for (i = 0; i < 8192; i++) {
    assign1[i] = (int) in1[i];
    assign2[i] = (int) in2[i];
    assign3[i] = (int) in3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[8192]);
  init_memory(&assign2[0], &assign2[8192]);
  init_memory(&assign3[0], &assign3[8192]);
  init_memory(&in1[0], &in1[8192]);
  init_memory(&in2[0], &in2[8192]);
  init_memory(&in3[0], &in3[8192]);
  BENCH("Example10b", example10b(in1,in2,in3,assign1,assign2,assign3), Mi*4/8192*512, digest_memory(&assign1[0], &assign1[8192])+digest_memory(&assign2[0], &assign2[8192])+digest_memory(&assign3[0], &assign3[8192]));
 
  return 0;
}
