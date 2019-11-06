#include "header.h"

short input[2048];
int   ia[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 2048; i+=2) {
    ia[i] = (int) input[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[2048]);
  init_memory(&input[0], &input[2048]);
  BENCH("Example10b", example10b(input,ia), Mi*4/2048*512, digest_memory(&ia[0], &ia[2048]));
 
  return 0;
}
