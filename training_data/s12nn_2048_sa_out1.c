#include "header.h"

short sa[2048];
short sb[2048];
short sc[2048];
int   out1[2048] ALIGNED16;
int   out2[2048] ALIGNED16;
int   out3[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ out1, int* __restrict__ out2, int* __restrict__ out3) {
  int i;
  for (i = 0; i < 2048; i++) {
    out1[i] = (int) sa[i];
    out2[i] = (int) sb[i];
    out3[i] = (int) sc[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[2048]);
  init_memory(&out2[0], &out2[2048]);
  init_memory(&out3[0], &out3[2048]);
  init_memory(&sa[0], &sa[2048]);
  init_memory(&sb[0], &sb[2048]);
  init_memory(&sc[0], &sc[2048]);
  BENCH("Example10b", example10b(sa,sb,sc,out1,out2,out3), Mi*4/2048*512, digest_memory(&out1[0], &out1[2048])+digest_memory(&out2[0], &out2[2048])+digest_memory(&out3[0], &out3[2048]));
 
  return 0;
}
