#include "header.h"

short in1[64];
short in2[64];
int   out1[64] ALIGNED16;
int   out2[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, int* __restrict__ out1, int* __restrict__ out2) {
  int i;
  for (i = 0; i < 64-1; i+=2) {
    out1[i] = (int) in1[i];
out1[i+1] = (int) in1[i+1];
    out2[i] = (int) in2[i];
out2[i+1] = (int) in2[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[64]);
  init_memory(&out2[0], &out2[64]);
  init_memory(&in1[0], &in1[64]);
  init_memory(&in2[0], &in2[64]);
  BENCH("Example10b", example10b(in1,in2,out1,out2), Mi*4/64*512, digest_memory(&out1[0], &out1[64])+digest_memory(&out2[0], &out2[64]));
 
  return 0;
}
