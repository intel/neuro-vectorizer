#include "header.h"

short sa[16384];
short sb[16384];
int   result1[16384] ALIGNED16;
int   result2[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 16384; i++) {
    result1[i] = (int) sa[i];
    result2[i] = (int) sb[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[16384]);
  init_memory(&result2[0], &result2[16384]);
  init_memory(&sa[0], &sa[16384]);
  init_memory(&sb[0], &sb[16384]);
  BENCH("Example10b", example10b(sa,sb,result1,result2), Mi*4/16384*512, digest_memory(&result1[0], &result1[16384])+digest_memory(&result2[0], &result2[16384]));
 
  return 0;
}
