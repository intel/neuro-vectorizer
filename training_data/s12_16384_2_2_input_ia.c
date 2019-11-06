#include "header.h"

short input[16384];
int   ia[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 16384-1; i+=2) {
    ia[i] = (int) input[i];
ia[i+1] = (int) input[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[16384]);
  init_memory(&input[0], &input[16384]);
  BENCH("Example10b", example10b(input,ia), Mi*4/16384*512, digest_memory(&ia[0], &ia[16384]));
 
  return 0;
}
