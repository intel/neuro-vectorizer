#include "header.h"

short sa[8192];
short sb[8192];
short sc[8192];
int   ia[8192] ALIGNED16;
int   ib[8192] ALIGNED16;
int   ic[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 8192-1; i+=2) {
    ia[i] = (int) sa[i];
ia[i+1] = (int) sa[i+1];
    ib[i] = (int) sb[i];
ib[i+1] = (int) sb[i+1];
    ic[i] = (int) sc[i];
ic[i+1] = (int) sc[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[8192]);
  init_memory(&ib[0], &ib[8192]);
  init_memory(&ic[0], &ic[8192]);
  init_memory(&sa[0], &sa[8192]);
  init_memory(&sb[0], &sb[8192]);
  init_memory(&sc[0], &sc[8192]);
  BENCH("Example10b", example10b(sa,sb,sc,ia,ib,ic), Mi*4/8192*512, digest_memory(&ia[0], &ia[8192])+digest_memory(&ib[0], &ib[8192])+digest_memory(&ic[0], &ic[8192]));
 
  return 0;
}
