#include "header.h"

short input1[16384];
short input2[16384];
short input3[16384];
int   ia[16384] ALIGNED16;
int   ib[16384] ALIGNED16;
int   ic[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, short *__restrict__ input3, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 16384; i+=2) {
    ia[i] = (int) input1[i];
    ib[i] = (int) input2[i];
    ic[i] = (int) input3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[16384]);
  init_memory(&ib[0], &ib[16384]);
  init_memory(&ic[0], &ic[16384]);
  init_memory(&input1[0], &input1[16384]);
  init_memory(&input2[0], &input2[16384]);
  init_memory(&input3[0], &input3[16384]);
  BENCH("Example10b", example10b(input1,input2,input3,ia,ib,ic), Mi*4/16384*512, digest_memory(&ia[0], &ia[16384])+digest_memory(&ib[0], &ib[16384])+digest_memory(&ic[0], &ic[16384]));
 
  return 0;
}
