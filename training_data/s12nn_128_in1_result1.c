#include "header.h"

short in1[128];
short in2[128];
short in3[128];
int   result1[128] ALIGNED16;
int   result2[128] ALIGNED16;
int   result3[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, short *__restrict__ in3, int* __restrict__ result1, int* __restrict__ result2, int* __restrict__ result3) {
  int i;
  for (i = 0; i < 128; i++) {
    result1[i] = (int) in1[i];
    result2[i] = (int) in2[i];
    result3[i] = (int) in3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[128]);
  init_memory(&result2[0], &result2[128]);
  init_memory(&result3[0], &result3[128]);
  init_memory(&in1[0], &in1[128]);
  init_memory(&in2[0], &in2[128]);
  init_memory(&in3[0], &in3[128]);
  BENCH("Example10b", example10b(in1,in2,in3,result1,result2,result3), Mi*4/128*512, digest_memory(&result1[0], &result1[128])+digest_memory(&result2[0], &result2[128])+digest_memory(&result3[0], &result3[128]));
 
  return 0;
}
