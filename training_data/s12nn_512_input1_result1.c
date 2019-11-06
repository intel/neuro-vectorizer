#include "header.h"

short input1[512];
short input2[512];
short input3[512];
int   result1[512] ALIGNED16;
int   result2[512] ALIGNED16;
int   result3[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, short *__restrict__ input3, int* __restrict__ result1, int* __restrict__ result2, int* __restrict__ result3) {
  int i;
  for (i = 0; i < 512; i++) {
    result1[i] = (int) input1[i];
    result2[i] = (int) input2[i];
    result3[i] = (int) input3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[512]);
  init_memory(&result2[0], &result2[512]);
  init_memory(&result3[0], &result3[512]);
  init_memory(&input1[0], &input1[512]);
  init_memory(&input2[0], &input2[512]);
  init_memory(&input3[0], &input3[512]);
  BENCH("Example10b", example10b(input1,input2,input3,result1,result2,result3), Mi*4/512*512, digest_memory(&result1[0], &result1[512])+digest_memory(&result2[0], &result2[512])+digest_memory(&result3[0], &result3[512]));
 
  return 0;
}
