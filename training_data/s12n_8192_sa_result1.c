#include "header.h"

short sa[8192];
short sb[8192];
int   result1[8192] ALIGNED16;
int   result2[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 8192; i++) {
    result1[i] = (int) sa[i];
    result2[i] = (int) sb[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[8192]);
  init_memory(&result2[0], &result2[8192]);
  init_memory(&sa[0], &sa[8192]);
  init_memory(&sb[0], &sb[8192]);
  BENCH("Example10b", example10b(sa,sb,result1,result2), Mi*4/8192*512, digest_memory(&result1[0], &result1[8192])+digest_memory(&result2[0], &result2[8192]));
 
  return 0;
}
