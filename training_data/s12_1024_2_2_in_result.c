#include "header.h"

short in[1024];
int   result[1024] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ result) {
  int i;
  for (i = 0; i < 1024-1; i+=2) {
    result[i] = (int) in[i];
result[i+1] = (int) in[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[1024]);
  init_memory(&in[0], &in[1024]);
  BENCH("Example10b", example10b(in,result), Mi*4/1024*512, digest_memory(&result[0], &result[1024]));
 
  return 0;
}
