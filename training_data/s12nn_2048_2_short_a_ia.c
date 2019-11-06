#include "header.h"

short short_a[2048];
short short_b[2048];
short short_c[2048];
int   ia[2048] ALIGNED16;
int   ib[2048] ALIGNED16;
int   ic[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, short *__restrict__ short_c, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 2048; i+=2) {
    ia[i] = (int) short_a[i];
    ib[i] = (int) short_b[i];
    ic[i] = (int) short_c[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[2048]);
  init_memory(&ib[0], &ib[2048]);
  init_memory(&ic[0], &ic[2048]);
  init_memory(&short_a[0], &short_a[2048]);
  init_memory(&short_b[0], &short_b[2048]);
  init_memory(&short_c[0], &short_c[2048]);
  BENCH("Example10b", example10b(short_a,short_b,short_c,ia,ib,ic), Mi*4/2048*512, digest_memory(&ia[0], &ia[2048])+digest_memory(&ib[0], &ib[2048])+digest_memory(&ic[0], &ic[2048]));
 
  return 0;
}
