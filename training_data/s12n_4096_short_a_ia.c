#include "header.h"

short short_a[4096];
short short_b[4096];
int   ia[4096] ALIGNED16;
int   ib[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, int* __restrict__ ia, int* __restrict__ ib) {
  int i;
  for (i = 0; i < 4096; i++) {
    ia[i] = (int) short_a[i];
    ib[i] = (int) short_b[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4096]);
  init_memory(&ib[0], &ib[4096]);
  init_memory(&short_a[0], &short_a[4096]);
  init_memory(&short_b[0], &short_b[4096]);
  BENCH("Example10b", example10b(short_a,short_b,ia,ib), Mi*4/4096*512, digest_memory(&ia[0], &ia[4096])+digest_memory(&ib[0], &ib[4096]));
 
  return 0;
}
