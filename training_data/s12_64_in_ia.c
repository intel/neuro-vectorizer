#include "header.h"

short in[64];
int   ia[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 64; i++) {
    ia[i] = (int) in[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[64]);
  init_memory(&in[0], &in[64]);
  BENCH("Example10b", example10b(in,ia), Mi*4/64*512, digest_memory(&ia[0], &ia[64]));
 
  return 0;
}
