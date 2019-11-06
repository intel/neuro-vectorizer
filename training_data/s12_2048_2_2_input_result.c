#include "header.h"

short input[2048];
int   result[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ result) {
  int i;
  for (i = 0; i < 2048-1; i+=2) {
    result[i] = (int) input[i];
result[i+1] = (int) input[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[2048]);
  init_memory(&input[0], &input[2048]);
  BENCH("Example10b", example10b(input,result), Mi*4/2048*512, digest_memory(&result[0], &result[2048]));
 
  return 0;
}
