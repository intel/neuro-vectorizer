#include "header.h"

short sa[1024];
short sb[1024];
int   out1[1024] ALIGNED16;
int   out2[1024] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ out1, int* __restrict__ out2) {
  int i;
  for (i = 0; i < 1024; i+=2) {
    out1[i] = (int) sa[i];
    out2[i] = (int) sb[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[1024]);
  init_memory(&out2[0], &out2[1024]);
  init_memory(&sa[0], &sa[1024]);
  init_memory(&sb[0], &sb[1024]);
  BENCH("Example10b", example10b(sa,sb,out1,out2), Mi*4/1024*512, digest_memory(&out1[0], &out1[1024])+digest_memory(&out2[0], &out2[1024]));
 
  return 0;
}
