#include "header.h"

short in1[4096];
short in2[4096];
int   ia[4096] ALIGNED16;
int   ib[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, int* __restrict__ ia, int* __restrict__ ib) {
  int i;
  for (i = 0; i < 4096-1; i+=2) {
    ia[i] = (int) in1[i];
ia[i+1] = (int) in1[i+1];
    ib[i] = (int) in2[i];
ib[i+1] = (int) in2[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4096]);
  init_memory(&ib[0], &ib[4096]);
  init_memory(&in1[0], &in1[4096]);
  init_memory(&in2[0], &in2[4096]);
  BENCH("Example10b", example10b(in1,in2,ia,ib), Mi*4/4096*512, digest_memory(&ia[0], &ia[4096])+digest_memory(&ib[0], &ib[4096]));
 
  return 0;
}
