#include "header.h"

short input[1024];
int   out[1024] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ out) {
  int i;
  for (i = 0; i < 1024-1; i+=2) {
    out[i] = (int) input[i];
out[i+1] = (int) input[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[1024]);
  init_memory(&input[0], &input[1024]);
  BENCH("Example10b", example10b(input,out), Mi*4/1024*512, digest_memory(&out[0], &out[1024]));
 
  return 0;
}
