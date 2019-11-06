#include "header.h"

short sa[16384];
short sb[16384];
short sc[16384];
int   ia[16384] ALIGNED16;
int   ib[16384] ALIGNED16;
int   ic[16384] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 16384; i++) {
    ia[i] = ib[i] + ic[i];
    sa[i] = sb[i] + sc[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[16384]);
  init_memory(&ib[0], &ib[16384]);
  init_memory(&ic[0], &ic[16384]);
  init_memory(&sa[0], &sa[16384]);
  init_memory(&sb[0], &sb[16384]);
  init_memory(&sc[0], &sc[16384]);
  BENCH("Example10a", example10a(sa,sb,sc,ia,ib,ic), Mi/16384*512, digest_memory(&ia[0], &ia[16384]) + digest_memory(&sa[0], &sa[16384]));
 
  return 0;
}
