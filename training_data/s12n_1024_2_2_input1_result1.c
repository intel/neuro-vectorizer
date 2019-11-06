#include "header.h"

short input1[1024];
short input2[1024];
int   result1[1024] ALIGNED16;
int   result2[1024] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 1024-1; i+=2) {
    result1[i] = (int) input1[i];
result1[i+1] = (int) input1[i+1];
    result2[i] = (int) input2[i];
result2[i+1] = (int) input2[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[1024]);
  init_memory(&result2[0], &result2[1024]);
  init_memory(&input1[0], &input1[1024]);
  init_memory(&input2[0], &input2[1024]);
  BENCH("Example10b", example10b(input1,input2,result1,result2), Mi*4/1024*512, digest_memory(&result1[0], &result1[1024])+digest_memory(&result2[0], &result2[1024]));
 
  return 0;
}
