#include "header.h"

short in[4096];
int   ia[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 4096-1; i+=2) {
    ia[i] = (int) in[i];
ia[i+1] = (int) in[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4096]);
  init_memory(&in[0], &in[4096]);
  BENCH("Example10b", example10b(in,ia), Mi*4/4096*512, digest_memory(&ia[0], &ia[4096]));
 
  return 0;
}
