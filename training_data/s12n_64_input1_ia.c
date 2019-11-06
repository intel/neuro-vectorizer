#include "header.h"

short input1[64];
short input2[64];
int   ia[64] ALIGNED16;
int   ib[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, int* __restrict__ ia, int* __restrict__ ib) {
  int i;
  for (i = 0; i < 64; i++) {
    ia[i] = (int) input1[i];
    ib[i] = (int) input2[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[64]);
  init_memory(&ib[0], &ib[64]);
  init_memory(&input1[0], &input1[64]);
  init_memory(&input2[0], &input2[64]);
  BENCH("Example10b", example10b(input1,input2,ia,ib), Mi*4/64*512, digest_memory(&ia[0], &ia[64])+digest_memory(&ib[0], &ib[64]));
 
  return 0;
}
