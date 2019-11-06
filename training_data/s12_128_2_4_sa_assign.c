#include "header.h"

short sa[128];
int   assign[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 128-3; i+=4) {
    assign[i] = (int) sa[i];
assign[i+1] = (int) sa[i+1];
assign[i+2] = (int) sa[i+2];
assign[i+3] = (int) sa[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[128]);
  init_memory(&sa[0], &sa[128]);
  BENCH("Example10b", example10b(sa,assign), Mi*4/128*512, digest_memory(&assign[0], &assign[128]));
 
  return 0;
}
