#include "header.h"

short input[64];
int   ia[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 64; i+=2) {
    ia[i] = (int) input[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[64]);
  init_memory(&input[0], &input[64]);
  BENCH("Example10b", example10b(input,ia), Mi*4/64*512, digest_memory(&ia[0], &ia[64]));
 
  return 0;
}
