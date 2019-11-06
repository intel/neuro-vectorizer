#include "header.h"

short in[64];
int   result[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ result) {
  int i;
  for (i = 0; i < 64-1; i+=2) {
    result[i] = (int) in[i];
result[i+1] = (int) in[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[64]);
  init_memory(&in[0], &in[64]);
  BENCH("Example10b", example10b(in,result), Mi*4/64*512, digest_memory(&result[0], &result[64]));
 
  return 0;
}
