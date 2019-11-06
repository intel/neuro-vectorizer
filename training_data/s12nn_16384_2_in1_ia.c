#include "header.h"

short in1[16384];
short in2[16384];
short in3[16384];
int   ia[16384] ALIGNED16;
int   ib[16384] ALIGNED16;
int   ic[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, short *__restrict__ in3, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 16384; i+=2) {
    ia[i] = (int) in1[i];
    ib[i] = (int) in2[i];
    ic[i] = (int) in3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[16384]);
  init_memory(&ib[0], &ib[16384]);
  init_memory(&ic[0], &ic[16384]);
  init_memory(&in1[0], &in1[16384]);
  init_memory(&in2[0], &in2[16384]);
  init_memory(&in3[0], &in3[16384]);
  BENCH("Example10b", example10b(in1,in2,in3,ia,ib,ic), Mi*4/16384*512, digest_memory(&ia[0], &ia[16384])+digest_memory(&ib[0], &ib[16384])+digest_memory(&ic[0], &ic[16384]));
 
  return 0;
}
