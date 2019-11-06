#include "header.h"

short sa[8192];
int   out[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ out) {
  int i;
  for (i = 0; i < 8192; i++) {
    out[i] = (int) sa[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[8192]);
  init_memory(&sa[0], &sa[8192]);
  BENCH("Example10b", example10b(sa,out), Mi*4/8192*512, digest_memory(&out[0], &out[8192]));
 
  return 0;
}
