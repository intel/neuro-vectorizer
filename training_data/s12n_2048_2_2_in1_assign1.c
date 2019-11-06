#include "header.h"

short in1[2048];
short in2[2048];
int   assign1[2048] ALIGNED16;
int   assign2[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, int* __restrict__ assign1, int* __restrict__ assign2) {
  int i;
  for (i = 0; i < 2048-1; i+=2) {
    assign1[i] = (int) in1[i];
assign1[i+1] = (int) in1[i+1];
    assign2[i] = (int) in2[i];
assign2[i+1] = (int) in2[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[2048]);
  init_memory(&assign2[0], &assign2[2048]);
  init_memory(&in1[0], &in1[2048]);
  init_memory(&in2[0], &in2[2048]);
  BENCH("Example10b", example10b(in1,in2,assign1,assign2), Mi*4/2048*512, digest_memory(&assign1[0], &assign1[2048])+digest_memory(&assign2[0], &assign2[2048]));
 
  return 0;
}
