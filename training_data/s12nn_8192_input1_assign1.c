#include "header.h"

short input1[8192];
short input2[8192];
short input3[8192];
int   assign1[8192] ALIGNED16;
int   assign2[8192] ALIGNED16;
int   assign3[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, short *__restrict__ input3, int* __restrict__ assign1, int* __restrict__ assign2, int* __restrict__ assign3) {
  int i;
  for (i = 0; i < 8192; i++) {
    assign1[i] = (int) input1[i];
    assign2[i] = (int) input2[i];
    assign3[i] = (int) input3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[8192]);
  init_memory(&assign2[0], &assign2[8192]);
  init_memory(&assign3[0], &assign3[8192]);
  init_memory(&input1[0], &input1[8192]);
  init_memory(&input2[0], &input2[8192]);
  init_memory(&input3[0], &input3[8192]);
  BENCH("Example10b", example10b(input1,input2,input3,assign1,assign2,assign3), Mi*4/8192*512, digest_memory(&assign1[0], &assign1[8192])+digest_memory(&assign2[0], &assign2[8192])+digest_memory(&assign3[0], &assign3[8192]));
 
  return 0;
}
