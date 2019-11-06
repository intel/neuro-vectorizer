#include "header.h"

short in1[8192];
short in2[8192];
int   result1[8192] ALIGNED16;
int   result2[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 8192-1; i+=2) {
    result1[i] = (int) in1[i];
result1[i+1] = (int) in1[i+1];
    result2[i] = (int) in2[i];
result2[i+1] = (int) in2[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[8192]);
  init_memory(&result2[0], &result2[8192]);
  init_memory(&in1[0], &in1[8192]);
  init_memory(&in2[0], &in2[8192]);
  BENCH("Example10b", example10b(in1,in2,result1,result2), Mi*4/8192*512, digest_memory(&result1[0], &result1[8192])+digest_memory(&result2[0], &result2[8192]));
 
  return 0;
}
