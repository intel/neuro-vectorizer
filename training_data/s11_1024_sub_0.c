#include "header.h"

short sa[1024];
short sb[1024];
short sc[1024];
int   ia[1024] ALIGNED16;
int   ib[1024] ALIGNED16;
int   ic[1024] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 1024; i++) {
    ia[i] = ib[i] + ic[i];
    sa[i] = sb[i] + sc[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[1024]);
  init_memory(&ib[0], &ib[1024]);
  init_memory(&ic[0], &ic[1024]);
  init_memory(&sa[0], &sa[1024]);
  init_memory(&sb[0], &sb[1024]);
  init_memory(&sc[0], &sc[1024]);
  BENCH("Example10a", example10a(sa,sb,sc,ia,ib,ic), Mi/1024*512, digest_memory(&ia[0], &ia[1024]) + digest_memory(&sa[0], &sa[1024]));
 
  return 0;
}
