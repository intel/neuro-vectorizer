#include "header.h"

short o2[4096];
short i4[4096];
short i5[4096];
int   o1[4096] ALIGNED16;
int   i2[4096] ALIGNED16;
int   i3[4096] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ o2, short *__restrict__ i4, short *__restrict__ i5, int* __restrict__ o1, int* __restrict__ i2, int* __restrict__ i3) {
  int i;
  for (i = 0; i < 4096; i++) {
    o1[i] = i2[i] + i3[i];
    o2[i] = i4[i] + i5[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&o1[0], &o1[4096]);
  init_memory(&i2[0], &i2[4096]);
  init_memory(&i3[0], &i3[4096]);
  init_memory(&o2[0], &o2[4096]);
  init_memory(&i4[0], &i4[4096]);
  init_memory(&i5[0], &i5[4096]);
  BENCH("Example10a", example10a(o2,i4,i5,o1,i2,i3), Mi/4096*512, digest_memory(&o1[0], &o1[4096]) + digest_memory(&o2[0], &o2[4096]));
 
  return 0;
}
