#include "header.h"

short sa[256];
int   out[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ out) {
  int i;
  for (i = 0; i < 256-3; i+=4) {
    out[i] = (int) sa[i];
out[i+1] = (int) sa[i+1];
out[i+2] = (int) sa[i+2];
out[i+3] = (int) sa[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[256]);
  init_memory(&sa[0], &sa[256]);
  BENCH("Example10b", example10b(sa,out), Mi*4/256*512, digest_memory(&out[0], &out[256]));
 
  return 0;
}
