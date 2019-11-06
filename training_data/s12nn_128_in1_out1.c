#include "header.h"

short in1[128];
short in2[128];
short in3[128];
int   out1[128] ALIGNED16;
int   out2[128] ALIGNED16;
int   out3[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, short *__restrict__ in3, int* __restrict__ out1, int* __restrict__ out2, int* __restrict__ out3) {
  int i;
  for (i = 0; i < 128; i++) {
    out1[i] = (int) in1[i];
    out2[i] = (int) in2[i];
    out3[i] = (int) in3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[128]);
  init_memory(&out2[0], &out2[128]);
  init_memory(&out3[0], &out3[128]);
  init_memory(&in1[0], &in1[128]);
  init_memory(&in2[0], &in2[128]);
  init_memory(&in3[0], &in3[128]);
  BENCH("Example10b", example10b(in1,in2,in3,out1,out2,out3), Mi*4/128*512, digest_memory(&out1[0], &out1[128])+digest_memory(&out2[0], &out2[128])+digest_memory(&out3[0], &out3[128]));
 
  return 0;
}
