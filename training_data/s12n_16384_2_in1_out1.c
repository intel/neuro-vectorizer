#include "header.h"

short in1[16384];
short in2[16384];
int   out1[16384] ALIGNED16;
int   out2[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, int* __restrict__ out1, int* __restrict__ out2) {
  int i;
  for (i = 0; i < 16384; i+=2) {
    out1[i] = (int) in1[i];
    out2[i] = (int) in2[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[16384]);
  init_memory(&out2[0], &out2[16384]);
  init_memory(&in1[0], &in1[16384]);
  init_memory(&in2[0], &in2[16384]);
  BENCH("Example10b", example10b(in1,in2,out1,out2), Mi*4/16384*512, digest_memory(&out1[0], &out1[16384])+digest_memory(&out2[0], &out2[16384]));
 
  return 0;
}
