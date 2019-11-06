#include "header.h"

short sa[4096];
int   assign[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 4096-3; i+=4) {
    assign[i] = (int) sa[i];
assign[i+1] = (int) sa[i+1];
assign[i+2] = (int) sa[i+2];
assign[i+3] = (int) sa[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[4096]);
  init_memory(&sa[0], &sa[4096]);
  BENCH("Example10b", example10b(sa,assign), Mi*4/4096*512, digest_memory(&assign[0], &assign[4096]));
 
  return 0;
}
