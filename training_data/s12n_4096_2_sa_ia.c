#include "header.h"

short sa[4096];
short sb[4096];
int   ia[4096] ALIGNED16;
int   ib[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ ia, int* __restrict__ ib) {
  int i;
  for (i = 0; i < 4096; i+=2) {
    ia[i] = (int) sa[i];
    ib[i] = (int) sb[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4096]);
  init_memory(&ib[0], &ib[4096]);
  init_memory(&sa[0], &sa[4096]);
  init_memory(&sb[0], &sb[4096]);
  BENCH("Example10b", example10b(sa,sb,ia,ib), Mi*4/4096*512, digest_memory(&ia[0], &ia[4096])+digest_memory(&ib[0], &ib[4096]));
 
  return 0;
}
