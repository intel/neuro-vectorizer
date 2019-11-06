#include "header.h"

short input1[2048];
short input2[2048];
int   assign1[2048] ALIGNED16;
int   assign2[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, int* __restrict__ assign1, int* __restrict__ assign2) {
  int i;
  for (i = 0; i < 2048; i++) {
    assign1[i] = (int) input1[i];
    assign2[i] = (int) input2[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[2048]);
  init_memory(&assign2[0], &assign2[2048]);
  init_memory(&input1[0], &input1[2048]);
  init_memory(&input2[0], &input2[2048]);
  BENCH("Example10b", example10b(input1,input2,assign1,assign2), Mi*4/2048*512, digest_memory(&assign1[0], &assign1[2048])+digest_memory(&assign2[0], &assign2[2048]));
 
  return 0;
}
