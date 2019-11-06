#include "header.h"

short short_a[1024];
short short_b[1024];
short short_c[1024];
int   ia[1024] ALIGNED16;
int   ib[1024] ALIGNED16;
int   ic[1024] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, short *__restrict__ short_c, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 1024; i+=2) {
    ia[i] = (int) short_a[i];
    ib[i] = (int) short_b[i];
    ic[i] = (int) short_c[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[1024]);
  init_memory(&ib[0], &ib[1024]);
  init_memory(&ic[0], &ic[1024]);
  init_memory(&short_a[0], &short_a[1024]);
  init_memory(&short_b[0], &short_b[1024]);
  init_memory(&short_c[0], &short_c[1024]);
  BENCH("Example10b", example10b(short_a,short_b,short_c,ia,ib,ic), Mi*4/1024*512, digest_memory(&ia[0], &ia[1024])+digest_memory(&ib[0], &ib[1024])+digest_memory(&ic[0], &ic[1024]));
 
  return 0;
}
