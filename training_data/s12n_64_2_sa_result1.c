#include "header.h"

short sa[64];
short sb[64];
int   result1[64] ALIGNED16;
int   result2[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 64; i+=2) {
    result1[i] = (int) sa[i];
    result2[i] = (int) sb[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[64]);
  init_memory(&result2[0], &result2[64]);
  init_memory(&sa[0], &sa[64]);
  init_memory(&sb[0], &sb[64]);
  BENCH("Example10b", example10b(sa,sb,result1,result2), Mi*4/64*512, digest_memory(&result1[0], &result1[64])+digest_memory(&result2[0], &result2[64]));
 
  return 0;
}
