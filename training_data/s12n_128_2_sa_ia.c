#include "header.h"

short sa[128];
short sb[128];
int   ia[128] ALIGNED16;
int   ib[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ ia, int* __restrict__ ib) {
  int i;
  for (i = 0; i < 128; i+=2) {
    ia[i] = (int) sa[i];
    ib[i] = (int) sb[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&ib[0], &ib[128]);
  init_memory(&sa[0], &sa[128]);
  init_memory(&sb[0], &sb[128]);
  BENCH("Example10b", example10b(sa,sb,ia,ib), Mi*4/128*512, digest_memory(&ia[0], &ia[128])+digest_memory(&ib[0], &ib[128]));
 
  return 0;
}
