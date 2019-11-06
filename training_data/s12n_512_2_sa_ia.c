#include "header.h"

short sa[512];
short sb[512];
int   ia[512] ALIGNED16;
int   ib[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ ia, int* __restrict__ ib) {
  int i;
  for (i = 0; i < 512; i+=2) {
    ia[i] = (int) sa[i];
    ib[i] = (int) sb[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[512]);
  init_memory(&ib[0], &ib[512]);
  init_memory(&sa[0], &sa[512]);
  init_memory(&sb[0], &sb[512]);
  BENCH("Example10b", example10b(sa,sb,ia,ib), Mi*4/512*512, digest_memory(&ia[0], &ia[512])+digest_memory(&ib[0], &ib[512]));
 
  return 0;
}
