#include "header.h"

short mul2[8192];
short s2[8192];
short s3[8192];
int   mul1[8192] ALIGNED16;
int   i2[8192] ALIGNED16;
int   i3[8192] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ mul2, short *__restrict__ s2, short *__restrict__ s3, int* __restrict__ mul1, int* __restrict__ i2, int* __restrict__ i3) {
  int i;
  for (i = 0; i < 8192; i++) {
    mul1[i] = i2[i] + i3[i];
    mul2[i] = s2[i] + s3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&mul1[0], &mul1[8192]);
  init_memory(&i2[0], &i2[8192]);
  init_memory(&i3[0], &i3[8192]);
  init_memory(&mul2[0], &mul2[8192]);
  init_memory(&s2[0], &s2[8192]);
  init_memory(&s3[0], &s3[8192]);
  BENCH("Example10a", example10a(mul2,s2,s3,mul1,i2,i3), Mi/8192*512, digest_memory(&mul1[0], &mul1[8192]) + digest_memory(&mul2[0], &mul2[8192]));
 
  return 0;
}
