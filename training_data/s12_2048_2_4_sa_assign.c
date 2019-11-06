#include "header.h"

short sa[2048];
int   assign[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 2048-3; i+=4) {
    assign[i] = (int) sa[i];
assign[i+1] = (int) sa[i+1];
assign[i+2] = (int) sa[i+2];
assign[i+3] = (int) sa[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[2048]);
  init_memory(&sa[0], &sa[2048]);
  BENCH("Example10b", example10b(sa,assign), Mi*4/2048*512, digest_memory(&assign[0], &assign[2048]));
 
  return 0;
}
