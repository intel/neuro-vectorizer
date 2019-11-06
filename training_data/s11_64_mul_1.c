#include "header.h"

short o2[64];
short i4[64];
short i5[64];
int   o1[64] ALIGNED16;
int   i2[64] ALIGNED16;
int   i3[64] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ o2, short *__restrict__ i4, short *__restrict__ i5, int* __restrict__ o1, int* __restrict__ i2, int* __restrict__ i3) {
  int i;
  for (i = 0; i < 64; i++) {
    o1[i] = i2[i] + i3[i];
    o2[i] = i4[i] + i5[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&o1[0], &o1[64]);
  init_memory(&i2[0], &i2[64]);
  init_memory(&i3[0], &i3[64]);
  init_memory(&o2[0], &o2[64]);
  init_memory(&i4[0], &i4[64]);
  init_memory(&i5[0], &i5[64]);
  BENCH("Example10a", example10a(o2,i4,i5,o1,i2,i3), Mi/64*512, digest_memory(&o1[0], &o1[64]) + digest_memory(&o2[0], &o2[64]));
 
  return 0;
}
