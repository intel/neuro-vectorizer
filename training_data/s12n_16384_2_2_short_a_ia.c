#include "header.h"

short short_a[16384];
short short_b[16384];
int   ia[16384] ALIGNED16;
int   ib[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, short *__restrict__ short_b, int* __restrict__ ia, int* __restrict__ ib) {
  int i;
  for (i = 0; i < 16384-1; i+=2) {
    ia[i] = (int) short_a[i];
ia[i+1] = (int) short_a[i+1];
    ib[i] = (int) short_b[i];
ib[i+1] = (int) short_b[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[16384]);
  init_memory(&ib[0], &ib[16384]);
  init_memory(&short_a[0], &short_a[16384]);
  init_memory(&short_b[0], &short_b[16384]);
  BENCH("Example10b", example10b(short_a,short_b,ia,ib), Mi*4/16384*512, digest_memory(&ia[0], &ia[16384])+digest_memory(&ib[0], &ib[16384]));
 
  return 0;
}
