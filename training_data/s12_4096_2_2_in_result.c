#include "header.h"

short in[4096];
int   result[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ result) {
  int i;
  for (i = 0; i < 4096-1; i+=2) {
    result[i] = (int) in[i];
result[i+1] = (int) in[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[4096]);
  init_memory(&in[0], &in[4096]);
  BENCH("Example10b", example10b(in,result), Mi*4/4096*512, digest_memory(&result[0], &result[4096]));
 
  return 0;
}
