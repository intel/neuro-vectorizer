#include "header.h"

short in[512];
int   assign[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 512; i++) {
    assign[i] = (int) in[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[512]);
  init_memory(&in[0], &in[512]);
  BENCH("Example10b", example10b(in,assign), Mi*4/512*512, digest_memory(&assign[0], &assign[512]));
 
  return 0;
}
