#include "header.h"

short in1[512];
short in2[512];
int   out1[512] ALIGNED16;
int   out2[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, int* __restrict__ out1, int* __restrict__ out2) {
  int i;
  for (i = 0; i < 512; i++) {
    out1[i] = (int) in1[i];
    out2[i] = (int) in2[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[512]);
  init_memory(&out2[0], &out2[512]);
  init_memory(&in1[0], &in1[512]);
  init_memory(&in2[0], &in2[512]);
  BENCH("Example10b", example10b(in1,in2,out1,out2), Mi*4/512*512, digest_memory(&out1[0], &out1[512])+digest_memory(&out2[0], &out2[512]));
 
  return 0;
}
