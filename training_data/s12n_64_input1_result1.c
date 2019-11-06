#include "header.h"

short input1[64];
short input2[64];
int   result1[64] ALIGNED16;
int   result2[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 64; i++) {
    result1[i] = (int) input1[i];
    result2[i] = (int) input2[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[64]);
  init_memory(&result2[0], &result2[64]);
  init_memory(&input1[0], &input1[64]);
  init_memory(&input2[0], &input2[64]);
  BENCH("Example10b", example10b(input1,input2,result1,result2), Mi*4/64*512, digest_memory(&result1[0], &result1[64])+digest_memory(&result2[0], &result2[64]));
 
  return 0;
}
