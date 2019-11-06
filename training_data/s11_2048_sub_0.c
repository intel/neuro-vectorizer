#include "header.h"

short sa[2048];
short sb[2048];
short sc[2048];
int   ia[2048] ALIGNED16;
int   ib[2048] ALIGNED16;
int   ic[2048] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 2048; i++) {
    ia[i] = ib[i] + ic[i];
    sa[i] = sb[i] + sc[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[2048]);
  init_memory(&ib[0], &ib[2048]);
  init_memory(&ic[0], &ic[2048]);
  init_memory(&sa[0], &sa[2048]);
  init_memory(&sb[0], &sb[2048]);
  init_memory(&sc[0], &sc[2048]);
  BENCH("Example10a", example10a(sa,sb,sc,ia,ib,ic), Mi/2048*512, digest_memory(&ia[0], &ia[2048]) + digest_memory(&sa[0], &sa[2048]));
 
  return 0;
}
