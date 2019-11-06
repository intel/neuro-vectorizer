#include "header.h"

short in[2048];
int   ia[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 2048; i+=2) {
    ia[i] = (int) in[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[2048]);
  init_memory(&in[0], &in[2048]);
  BENCH("Example10b", example10b(in,ia), Mi*4/2048*512, digest_memory(&ia[0], &ia[2048]));
 
  return 0;
}
