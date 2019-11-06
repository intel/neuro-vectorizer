#include "header.h"

short in[16384];
int   assign[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 16384; i++) {
    assign[i] = (int) in[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[16384]);
  init_memory(&in[0], &in[16384]);
  BENCH("Example10b", example10b(in,assign), Mi*4/16384*512, digest_memory(&assign[0], &assign[16384]));
 
  return 0;
}
