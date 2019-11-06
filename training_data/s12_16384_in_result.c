#include "header.h"

short in[16384];
int   result[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ result) {
  int i;
  for (i = 0; i < 16384; i++) {
    result[i] = (int) in[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[16384]);
  init_memory(&in[0], &in[16384]);
  BENCH("Example10b", example10b(in,result), Mi*4/16384*512, digest_memory(&result[0], &result[16384]));
 
  return 0;
}
