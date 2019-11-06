#include "header.h"

short input1[4096];
short input2[4096];
int   ia[4096] ALIGNED16;
int   ib[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, int* __restrict__ ia, int* __restrict__ ib) {
  int i;
  for (i = 0; i < 4096; i++) {
    ia[i] = (int) input1[i];
    ib[i] = (int) input2[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4096]);
  init_memory(&ib[0], &ib[4096]);
  init_memory(&input1[0], &input1[4096]);
  init_memory(&input2[0], &input2[4096]);
  BENCH("Example10b", example10b(input1,input2,ia,ib), Mi*4/4096*512, digest_memory(&ia[0], &ia[4096])+digest_memory(&ib[0], &ib[4096]));
 
  return 0;
}
