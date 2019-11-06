#include "header.h"

short sa[8192];
short sb[8192];
int   out1[8192] ALIGNED16;
int   out2[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ out1, int* __restrict__ out2) {
  int i;
  for (i = 0; i < 8192; i++) {
    out1[i] = (int) sa[i];
    out2[i] = (int) sb[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[8192]);
  init_memory(&out2[0], &out2[8192]);
  init_memory(&sa[0], &sa[8192]);
  init_memory(&sb[0], &sb[8192]);
  BENCH("Example10b", example10b(sa,sb,out1,out2), Mi*4/8192*512, digest_memory(&out1[0], &out1[8192])+digest_memory(&out2[0], &out2[8192]));
 
  return 0;
}
