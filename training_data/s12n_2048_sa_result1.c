#include "header.h"

short sa[2048];
short sb[2048];
int   result1[2048] ALIGNED16;
int   result2[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 2048; i++) {
    result1[i] = (int) sa[i];
    result2[i] = (int) sb[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[2048]);
  init_memory(&result2[0], &result2[2048]);
  init_memory(&sa[0], &sa[2048]);
  init_memory(&sb[0], &sb[2048]);
  BENCH("Example10b", example10b(sa,sb,result1,result2), Mi*4/2048*512, digest_memory(&result1[0], &result1[2048])+digest_memory(&result2[0], &result2[2048]));
 
  return 0;
}
