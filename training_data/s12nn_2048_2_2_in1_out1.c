#include "header.h"

short in1[2048];
short in2[2048];
short in3[2048];
int   out1[2048] ALIGNED16;
int   out2[2048] ALIGNED16;
int   out3[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, short *__restrict__ in3, int* __restrict__ out1, int* __restrict__ out2, int* __restrict__ out3) {
  int i;
  for (i = 0; i < 2048-1; i+=2) {
    out1[i] = (int) in1[i];
out1[i+1] = (int) in1[i+1];
    out2[i] = (int) in2[i];
out2[i+1] = (int) in2[i+1];
    out3[i] = (int) in3[i];
out3[i+1] = (int) in3[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[2048]);
  init_memory(&out2[0], &out2[2048]);
  init_memory(&out3[0], &out3[2048]);
  init_memory(&in1[0], &in1[2048]);
  init_memory(&in2[0], &in2[2048]);
  init_memory(&in3[0], &in3[2048]);
  BENCH("Example10b", example10b(in1,in2,in3,out1,out2,out3), Mi*4/2048*512, digest_memory(&out1[0], &out1[2048])+digest_memory(&out2[0], &out2[2048])+digest_memory(&out3[0], &out3[2048]));
 
  return 0;
}
