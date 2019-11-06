#include "header.h"

short input1[4096];
short input2[4096];
short input3[4096];
int   result1[4096] ALIGNED16;
int   result2[4096] ALIGNED16;
int   result3[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, short *__restrict__ input3, int* __restrict__ result1, int* __restrict__ result2, int* __restrict__ result3) {
  int i;
  for (i = 0; i < 4096-1; i+=2) {
    result1[i] = (int) input1[i];
result1[i+1] = (int) input1[i+1];
    result2[i] = (int) input2[i];
result2[i+1] = (int) input2[i+1];
    result3[i] = (int) input3[i];
result3[i+1] = (int) input3[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[4096]);
  init_memory(&result2[0], &result2[4096]);
  init_memory(&result3[0], &result3[4096]);
  init_memory(&input1[0], &input1[4096]);
  init_memory(&input2[0], &input2[4096]);
  init_memory(&input3[0], &input3[4096]);
  BENCH("Example10b", example10b(input1,input2,input3,result1,result2,result3), Mi*4/4096*512, digest_memory(&result1[0], &result1[4096])+digest_memory(&result2[0], &result2[4096])+digest_memory(&result3[0], &result3[4096]));
 
  return 0;
}
