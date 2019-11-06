#include "header.h"

short in1[4096];
short in2[4096];
int   out1[4096] ALIGNED16;
int   out2[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, int* __restrict__ out1, int* __restrict__ out2) {
  int i;
  for (i = 0; i < 4096; i+=2) {
    out1[i] = (int) in1[i];
    out2[i] = (int) in2[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[4096]);
  init_memory(&out2[0], &out2[4096]);
  init_memory(&in1[0], &in1[4096]);
  init_memory(&in2[0], &in2[4096]);
  BENCH("Example10b", example10b(in1,in2,out1,out2), Mi*4/4096*512, digest_memory(&out1[0], &out1[4096])+digest_memory(&out2[0], &out2[4096]));
 
  return 0;
}
