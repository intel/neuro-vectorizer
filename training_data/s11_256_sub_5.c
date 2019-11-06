#include "header.h"

short sub2[256];
short s2[256];
short s3[256];
int   sub1[256] ALIGNED16;
int   i2[256] ALIGNED16;
int   i3[256] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ sub2, short *__restrict__ s2, short *__restrict__ s3, int* __restrict__ sub1, int* __restrict__ i2, int* __restrict__ i3) {
  int i;
  for (i = 0; i < 256; i++) {
    sub1[i] = i2[i] + i3[i];
    sub2[i] = s2[i] + s3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&sub1[0], &sub1[256]);
  init_memory(&i2[0], &i2[256]);
  init_memory(&i3[0], &i3[256]);
  init_memory(&sub2[0], &sub2[256]);
  init_memory(&s2[0], &s2[256]);
  init_memory(&s3[0], &s3[256]);
  BENCH("Example10a", example10a(sub2,s2,s3,sub1,i2,i3), Mi/256*512, digest_memory(&sub1[0], &sub1[256]) + digest_memory(&sub2[0], &sub2[256]));
 
  return 0;
}
