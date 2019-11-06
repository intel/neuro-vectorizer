#include "header.h"

short in1[128];
short in2[128];
int   ia[128] ALIGNED16;
int   ib[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, int* __restrict__ ia, int* __restrict__ ib) {
  int i;
  for (i = 0; i < 128; i+=2) {
    ia[i] = (int) in1[i];
    ib[i] = (int) in2[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&ib[0], &ib[128]);
  init_memory(&in1[0], &in1[128]);
  init_memory(&in2[0], &in2[128]);
  BENCH("Example10b", example10b(in1,in2,ia,ib), Mi*4/128*512, digest_memory(&ia[0], &ia[128])+digest_memory(&ib[0], &ib[128]));
 
  return 0;
}
