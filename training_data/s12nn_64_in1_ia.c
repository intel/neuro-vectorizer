#include "header.h"

short in1[64];
short in2[64];
short in3[64];
int   ia[64] ALIGNED16;
int   ib[64] ALIGNED16;
int   ic[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, short *__restrict__ in3, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 64; i++) {
    ia[i] = (int) in1[i];
    ib[i] = (int) in2[i];
    ic[i] = (int) in3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[64]);
  init_memory(&ib[0], &ib[64]);
  init_memory(&ic[0], &ic[64]);
  init_memory(&in1[0], &in1[64]);
  init_memory(&in2[0], &in2[64]);
  init_memory(&in3[0], &in3[64]);
  BENCH("Example10b", example10b(in1,in2,in3,ia,ib,ic), Mi*4/64*512, digest_memory(&ia[0], &ia[64])+digest_memory(&ib[0], &ib[64])+digest_memory(&ic[0], &ic[64]));
 
  return 0;
}
