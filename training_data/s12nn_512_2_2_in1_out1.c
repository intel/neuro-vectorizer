#include "header.h"

short in1[512];
short in2[512];
short in3[512];
int   out1[512] ALIGNED16;
int   out2[512] ALIGNED16;
int   out3[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, short *__restrict__ in3, int* __restrict__ out1, int* __restrict__ out2, int* __restrict__ out3) {
  int i;
  for (i = 0; i < 512-1; i+=2) {
    out1[i] = (int) in1[i];
out1[i+1] = (int) in1[i+1];
    out2[i] = (int) in2[i];
out2[i+1] = (int) in2[i+1];
    out3[i] = (int) in3[i];
out3[i+1] = (int) in3[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[512]);
  init_memory(&out2[0], &out2[512]);
  init_memory(&out3[0], &out3[512]);
  init_memory(&in1[0], &in1[512]);
  init_memory(&in2[0], &in2[512]);
  init_memory(&in3[0], &in3[512]);
  BENCH("Example10b", example10b(in1,in2,in3,out1,out2,out3), Mi*4/512*512, digest_memory(&out1[0], &out1[512])+digest_memory(&out2[0], &out2[512])+digest_memory(&out3[0], &out3[512]));
 
  return 0;
}
