#include "header.h"

short in[256];
int   ia[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 256-3; i+=4) {
    ia[i] = (int) in[i];
ia[i+1] = (int) in[i+1];
ia[i+2] = (int) in[i+2];
ia[i+3] = (int) in[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[256]);
  init_memory(&in[0], &in[256]);
  BENCH("Example10b", example10b(in,ia), Mi*4/256*512, digest_memory(&ia[0], &ia[256]));
 
  return 0;
}
