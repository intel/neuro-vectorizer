#include "header.h"

short sa[8192];
short sb[8192];
short sc[8192];
int   ia[8192] ALIGNED16;
int   ib[8192] ALIGNED16;
int   ic[8192] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 8192; i++) {
    ia[i] = ib[i] + ic[i];
    sa[i] = sb[i] + sc[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[8192]);
  init_memory(&ib[0], &ib[8192]);
  init_memory(&ic[0], &ic[8192]);
  init_memory(&sa[0], &sa[8192]);
  init_memory(&sb[0], &sb[8192]);
  init_memory(&sc[0], &sc[8192]);
  BENCH("Example10a", example10a(sa,sb,sc,ia,ib,ic), Mi/8192*512, digest_memory(&ia[0], &ia[8192]) + digest_memory(&sa[0], &sa[8192]));
 
  return 0;
}
