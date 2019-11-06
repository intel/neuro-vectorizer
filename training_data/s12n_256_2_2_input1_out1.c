#include "header.h"

short input1[256];
short input2[256];
int   out1[256] ALIGNED16;
int   out2[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, int* __restrict__ out1, int* __restrict__ out2) {
  int i;
  for (i = 0; i < 256-1; i+=2) {
    out1[i] = (int) input1[i];
out1[i+1] = (int) input1[i+1];
    out2[i] = (int) input2[i];
out2[i+1] = (int) input2[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[256]);
  init_memory(&out2[0], &out2[256]);
  init_memory(&input1[0], &input1[256]);
  init_memory(&input2[0], &input2[256]);
  BENCH("Example10b", example10b(input1,input2,out1,out2), Mi*4/256*512, digest_memory(&out1[0], &out1[256])+digest_memory(&out2[0], &out2[256]));
 
  return 0;
}
