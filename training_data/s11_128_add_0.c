#include "header.h"

short sa[128];
short sb[128];
short sc[128];
int   ia[128] ALIGNED16;
int   ib[128] ALIGNED16;
int   ic[128] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 128; i++) {
    ia[i] = ib[i] + ic[i];
    sa[i] = sb[i] + sc[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&ib[0], &ib[128]);
  init_memory(&ic[0], &ic[128]);
  init_memory(&sa[0], &sa[128]);
  init_memory(&sb[0], &sb[128]);
  init_memory(&sc[0], &sc[128]);
  BENCH("Example10a", example10a(sa,sb,sc,ia,ib,ic), Mi/128*512, digest_memory(&ia[0], &ia[128]) + digest_memory(&sa[0], &sa[128]));
 
  return 0;
}
