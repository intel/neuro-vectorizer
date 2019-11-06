#include "header.h"

short input[8192];
int   result[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ result) {
  int i;
  for (i = 0; i < 8192; i++) {
    result[i] = (int) input[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[8192]);
  init_memory(&input[0], &input[8192]);
  BENCH("Example10b", example10b(input,result), Mi*4/8192*512, digest_memory(&result[0], &result[8192]));
 
  return 0;
}
