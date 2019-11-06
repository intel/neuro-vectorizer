#include "header.h"

short sa[512];
short sb[512];
int   out1[512] ALIGNED16;
int   out2[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ out1, int* __restrict__ out2) {
  int i;
  for (i = 0; i < 512-1; i+=2) {
    out1[i] = (int) sa[i];
out1[i+1] = (int) sa[i+1];
    out2[i] = (int) sb[i];
out2[i+1] = (int) sb[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[512]);
  init_memory(&out2[0], &out2[512]);
  init_memory(&sa[0], &sa[512]);
  init_memory(&sb[0], &sb[512]);
  BENCH("Example10b", example10b(sa,sb,out1,out2), Mi*4/512*512, digest_memory(&out1[0], &out1[512])+digest_memory(&out2[0], &out2[512]));
 
  return 0;
}
