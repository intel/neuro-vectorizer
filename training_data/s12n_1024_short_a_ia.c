#include "header.h"

short short_a[1024];
short short_b[1024];
int   ia[1024] ALIGNED16;
int   ib[1024] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, int* __restrict__ ia, int* __restrict__ ib) {
  int i;
  for (i = 0; i < 1024; i++) {
    ia[i] = (int) short_a[i];
    ib[i] = (int) short_b[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[1024]);
  init_memory(&ib[0], &ib[1024]);
  init_memory(&short_a[0], &short_a[1024]);
  init_memory(&short_b[0], &short_b[1024]);
  BENCH("Example10b", example10b(short_a,short_b,ia,ib), Mi*4/1024*512, digest_memory(&ia[0], &ia[1024])+digest_memory(&ib[0], &ib[1024]));
 
  return 0;
}
