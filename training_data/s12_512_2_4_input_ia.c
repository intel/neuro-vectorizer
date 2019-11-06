#include "header.h"

short input[512];
int   ia[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 512-3; i+=4) {
    ia[i] = (int) input[i];
ia[i+1] = (int) input[i+1];
ia[i+2] = (int) input[i+2];
ia[i+3] = (int) input[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[512]);
  init_memory(&input[0], &input[512]);
  BENCH("Example10b", example10b(input,ia), Mi*4/512*512, digest_memory(&ia[0], &ia[512]));
 
  return 0;
}
