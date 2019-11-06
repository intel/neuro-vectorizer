#include "header.h"

short input[1024];
int   result[1024] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ result) {
  int i;
  for (i = 0; i < 1024; i+=2) {
    result[i] = (int) input[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[1024]);
  init_memory(&input[0], &input[1024]);
  BENCH("Example10b", example10b(input,result), Mi*4/1024*512, digest_memory(&result[0], &result[1024]));
 
  return 0;
}
