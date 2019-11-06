#include "header.h"

short input1[4096];
short input2[4096];
int   out1[4096] ALIGNED16;
int   out2[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, int* __restrict__ out1, int* __restrict__ out2) {
  int i;
  for (i = 0; i < 4096; i++) {
    out1[i] = (int) input1[i];
    out2[i] = (int) input2[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[4096]);
  init_memory(&out2[0], &out2[4096]);
  init_memory(&input1[0], &input1[4096]);
  init_memory(&input2[0], &input2[4096]);
  BENCH("Example10b", example10b(input1,input2,out1,out2), Mi*4/4096*512, digest_memory(&out1[0], &out1[4096])+digest_memory(&out2[0], &out2[4096]));
 
  return 0;
}
