#include "header.h"

short input1[512];
short input2[512];
short input3[512];
int   out1[512] ALIGNED16;
int   out2[512] ALIGNED16;
int   out3[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, short *__restrict__ input3, int* __restrict__ out1, int* __restrict__ out2, int* __restrict__ out3) {
  int i;
  for (i = 0; i < 512; i++) {
    out1[i] = (int) input1[i];
    out2[i] = (int) input2[i];
    out3[i] = (int) input3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[512]);
  init_memory(&out2[0], &out2[512]);
  init_memory(&out3[0], &out3[512]);
  init_memory(&input1[0], &input1[512]);
  init_memory(&input2[0], &input2[512]);
  init_memory(&input3[0], &input3[512]);
  BENCH("Example10b", example10b(input1,input2,input3,out1,out2,out3), Mi*4/512*512, digest_memory(&out1[0], &out1[512])+digest_memory(&out2[0], &out2[512])+digest_memory(&out3[0], &out3[512]));
 
  return 0;
}
