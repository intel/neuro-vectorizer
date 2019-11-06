#include "header.h"

short in[8192];
int   result[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ result) {
  int i;
  for (i = 0; i < 8192; i+=2) {
    result[i] = (int) in[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[8192]);
  init_memory(&in[0], &in[8192]);
  BENCH("Example10b", example10b(in,result), Mi*4/8192*512, digest_memory(&result[0], &result[8192]));
 
  return 0;
}
