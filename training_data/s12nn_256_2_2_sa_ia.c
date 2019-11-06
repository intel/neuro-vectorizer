#include "header.h"

short sa[256];
short sb[256];
short sc[256];
int   ia[256] ALIGNED16;
int   ib[256] ALIGNED16;
int   ic[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 256-1; i+=2) {
    ia[i] = (int) sa[i];
ia[i+1] = (int) sa[i+1];
    ib[i] = (int) sb[i];
ib[i+1] = (int) sb[i+1];
    ic[i] = (int) sc[i];
ic[i+1] = (int) sc[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[256]);
  init_memory(&ib[0], &ib[256]);
  init_memory(&ic[0], &ic[256]);
  init_memory(&sa[0], &sa[256]);
  init_memory(&sb[0], &sb[256]);
  init_memory(&sc[0], &sc[256]);
  BENCH("Example10b", example10b(sa,sb,sc,ia,ib,ic), Mi*4/256*512, digest_memory(&ia[0], &ia[256])+digest_memory(&ib[0], &ib[256])+digest_memory(&ic[0], &ic[256]));
 
  return 0;
}
