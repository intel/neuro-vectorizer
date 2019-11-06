#include "header.h"

short in[2048];
int   ia[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 2048-3; i+=4) {
    ia[i] = (int) in[i];
ia[i+1] = (int) in[i+1];
ia[i+2] = (int) in[i+2];
ia[i+3] = (int) in[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[2048]);
  init_memory(&in[0], &in[2048]);
  BENCH("Example10b", example10b(in,ia), Mi*4/2048*512, digest_memory(&ia[0], &ia[2048]));
 
  return 0;
}
