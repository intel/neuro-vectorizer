#include "header.h"

short short_a[128];
short short_b[128];
short short_c[128];
int   ia[128] ALIGNED16;
int   ib[128] ALIGNED16;
int   ic[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, short *__restrict__ short_c, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 128-1; i+=2) {
    ia[i] = (int) short_a[i];
ia[i+1] = (int) short_a[i+1];
    ib[i] = (int) short_b[i];
ib[i+1] = (int) short_b[i+1];
    ic[i] = (int) short_c[i];
ic[i+1] = (int) short_c[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&ib[0], &ib[128]);
  init_memory(&ic[0], &ic[128]);
  init_memory(&short_a[0], &short_a[128]);
  init_memory(&short_b[0], &short_b[128]);
  init_memory(&short_c[0], &short_c[128]);
  BENCH("Example10b", example10b(short_a,short_b,short_c,ia,ib,ic), Mi*4/128*512, digest_memory(&ia[0], &ia[128])+digest_memory(&ib[0], &ib[128])+digest_memory(&ic[0], &ic[128]));
 
  return 0;
}
