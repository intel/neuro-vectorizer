#include "header.h"

short in1[256];
short in2[256];
short in3[256];
int   assign1[256] ALIGNED16;
int   assign2[256] ALIGNED16;
int   assign3[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, short *__restrict__ in3, int* __restrict__ assign1, int* __restrict__ assign2, int* __restrict__ assign3) {
  int i;
  for (i = 0; i < 256; i+=2) {
    assign1[i] = (int) in1[i];
    assign2[i] = (int) in2[i];
    assign3[i] = (int) in3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[256]);
  init_memory(&assign2[0], &assign2[256]);
  init_memory(&assign3[0], &assign3[256]);
  init_memory(&in1[0], &in1[256]);
  init_memory(&in2[0], &in2[256]);
  init_memory(&in3[0], &in3[256]);
  BENCH("Example10b", example10b(in1,in2,in3,assign1,assign2,assign3), Mi*4/256*512, digest_memory(&assign1[0], &assign1[256])+digest_memory(&assign2[0], &assign2[256])+digest_memory(&assign3[0], &assign3[256]));
 
  return 0;
}
