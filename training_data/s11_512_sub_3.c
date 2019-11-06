#include "header.h"

short short1[512];
short short2[512];
short short3[512];
int   int1[512] ALIGNED16;
int   int2[512] ALIGNED16;
int   int3[512] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ short1, short *__restrict__ short2, short *__restrict__ short3, int* __restrict__ int1, int* __restrict__ int2, int* __restrict__ int3) {
  int i;
  for (i = 0; i < 512; i++) {
    int1[i] = int2[i] + int3[i];
    short1[i] = short2[i] + short3[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&int1[0], &int1[512]);
  init_memory(&int2[0], &int2[512]);
  init_memory(&int3[0], &int3[512]);
  init_memory(&short1[0], &short1[512]);
  init_memory(&short2[0], &short2[512]);
  init_memory(&short3[0], &short3[512]);
  BENCH("Example10a", example10a(short1,short2,short3,int1,int2,int3), Mi/512*512, digest_memory(&int1[0], &int1[512]) + digest_memory(&short1[0], &short1[512]));
 
  return 0;
}
