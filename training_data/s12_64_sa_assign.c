#include "header.h"

short sa[64];
int   assign[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 64; i++) {
    assign[i] = (int) sa[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[64]);
  init_memory(&sa[0], &sa[64]);
  BENCH("Example10b", example10b(sa,assign), Mi*4/64*512, digest_memory(&assign[0], &assign[64]));
 
  return 0;
}
