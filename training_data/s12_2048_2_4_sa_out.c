#include "header.h"

short sa[2048];
int   out[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ out) {
  int i;
  for (i = 0; i < 2048-3; i+=4) {
    out[i] = (int) sa[i];
out[i+1] = (int) sa[i+1];
out[i+2] = (int) sa[i+2];
out[i+3] = (int) sa[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&out[0], &out[2048]);
  init_memory(&sa[0], &sa[2048]);
  BENCH("Example10b", example10b(sa,out), Mi*4/2048*512, digest_memory(&out[0], &out[2048]));
 
  return 0;
}
