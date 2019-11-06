#include "header.h"

short res2[16384];
short s2[16384];
short s3[16384];
int   res1[16384] ALIGNED16;
int   i2[16384] ALIGNED16;
int   i3[16384] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ res2, short *__restrict__ s2, short *__restrict__ s3, int* __restrict__ res1, int* __restrict__ i2, int* __restrict__ i3) {
  int i;
  for (i = 0; i < 16384; i++) {
    res1[i] = i2[i] + i3[i];
    res2[i] = s2[i] + s3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&res1[0], &res1[16384]);
  init_memory(&i2[0], &i2[16384]);
  init_memory(&i3[0], &i3[16384]);
  init_memory(&res2[0], &res2[16384]);
  init_memory(&s2[0], &s2[16384]);
  init_memory(&s3[0], &s3[16384]);
  BENCH("Example10a", example10a(res2,s2,s3,res1,i2,i3), Mi/16384*512, digest_memory(&res1[0], &res1[16384]) + digest_memory(&res2[0], &res2[16384]));
 
  return 0;
}
