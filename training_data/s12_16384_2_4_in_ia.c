#include "header.h"

short in[16384];
int   ia[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 16384-3; i+=4) {
    ia[i] = (int) in[i];
ia[i+1] = (int) in[i+1];
ia[i+2] = (int) in[i+2];
ia[i+3] = (int) in[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[16384]);
  init_memory(&in[0], &in[16384]);
  BENCH("Example10b", example10b(in,ia), Mi*4/16384*512, digest_memory(&ia[0], &ia[16384]));
 
  return 0;
}
