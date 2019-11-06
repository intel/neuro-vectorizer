#include "header.h"

short o2[128];
short i4[128];
short i5[128];
int   o1[128] ALIGNED16;
int   i2[128] ALIGNED16;
int   i3[128] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ o2, short *__restrict__ i4, short *__restrict__ i5, int* __restrict__ o1, int* __restrict__ i2, int* __restrict__ i3) {
  int i;
  for (i = 0; i < 128; i++) {
    o1[i] = i2[i] + i3[i];
    o2[i] = i4[i] + i5[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&o1[0], &o1[128]);
  init_memory(&i2[0], &i2[128]);
  init_memory(&i3[0], &i3[128]);
  init_memory(&o2[0], &o2[128]);
  init_memory(&i4[0], &i4[128]);
  init_memory(&i5[0], &i5[128]);
  BENCH("Example10a", example10a(o2,i4,i5,o1,i2,i3), Mi/128*512, digest_memory(&o1[0], &o1[128]) + digest_memory(&o2[0], &o2[128]));
 
  return 0;
}
