#include "header.h"

short sa[64];
short sb[64];
short sc[64];
int   ia[64] ALIGNED16;
int   ib[64] ALIGNED16;
int   ic[64] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 64; i++) {
    ia[i] = ib[i] + ic[i];
    sa[i] = sb[i] + sc[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[64]);
  init_memory(&ib[0], &ib[64]);
  init_memory(&ic[0], &ic[64]);
  init_memory(&sa[0], &sa[64]);
  init_memory(&sb[0], &sb[64]);
  init_memory(&sc[0], &sc[64]);
  BENCH("Example10a", example10a(sa,sb,sc,ia,ib,ic), Mi/64*512, digest_memory(&ia[0], &ia[64]) + digest_memory(&sa[0], &sa[64]));
 
  return 0;
}
