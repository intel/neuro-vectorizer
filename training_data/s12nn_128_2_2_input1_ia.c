#include "header.h"

short input1[128];
short input2[128];
short input3[128];
int   ia[128] ALIGNED16;
int   ib[128] ALIGNED16;
int   ic[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input1, short *__restrict__ input2, short *__restrict__ input3, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 128-1; i+=2) {
    ia[i] = (int) input1[i];
ia[i+1] = (int) input1[i+1];
    ib[i] = (int) input2[i];
ib[i+1] = (int) input2[i+1];
    ic[i] = (int) input3[i];
ic[i+1] = (int) input3[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&ib[0], &ib[128]);
  init_memory(&ic[0], &ic[128]);
  init_memory(&input1[0], &input1[128]);
  init_memory(&input2[0], &input2[128]);
  init_memory(&input3[0], &input3[128]);
  BENCH("Example10b", example10b(input1,input2,input3,ia,ib,ic), Mi*4/128*512, digest_memory(&ia[0], &ia[128])+digest_memory(&ib[0], &ib[128])+digest_memory(&ic[0], &ic[128]));
 
  return 0;
}
