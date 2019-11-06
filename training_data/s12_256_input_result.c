#include "header.h"

short input[256];
int   result[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ result) {
  int i;
  for (i = 0; i < 256; i++) {
    result[i] = (int) input[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[256]);
  init_memory(&input[0], &input[256]);
  BENCH("Example10b", example10b(input,result), Mi*4/256*512, digest_memory(&result[0], &result[256]));
 
  return 0;
}
