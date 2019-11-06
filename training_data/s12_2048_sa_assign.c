#include "header.h"

short sa[2048];
int   assign[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 2048; i++) {
    assign[i] = (int) sa[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[2048]);
  init_memory(&sa[0], &sa[2048]);
  BENCH("Example10b", example10b(sa,assign), Mi*4/2048*512, digest_memory(&assign[0], &assign[2048]));
 
  return 0;
}
