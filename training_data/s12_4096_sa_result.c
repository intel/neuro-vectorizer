#include "header.h"

short sa[4096];
int   result[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ result) {
  int i;
  for (i = 0; i < 4096; i++) {
    result[i] = (int) sa[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[4096]);
  init_memory(&sa[0], &sa[4096]);
  BENCH("Example10b", example10b(sa,result), Mi*4/4096*512, digest_memory(&result[0], &result[4096]));
 
  return 0;
}
