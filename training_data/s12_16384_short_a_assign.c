#include "header.h"

short short_a[16384];
int   assign[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 16384; i++) {
    assign[i] = (int) short_a[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[16384]);
  init_memory(&short_a[0], &short_a[16384]);
  BENCH("Example10b", example10b(short_a,assign), Mi*4/16384*512, digest_memory(&assign[0], &assign[16384]));
 
  return 0;
}
