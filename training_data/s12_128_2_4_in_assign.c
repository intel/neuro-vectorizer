#include "header.h"

short in[128];
int   assign[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 128-3; i+=4) {
    assign[i] = (int) in[i];
assign[i+1] = (int) in[i+1];
assign[i+2] = (int) in[i+2];
assign[i+3] = (int) in[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[128]);
  init_memory(&in[0], &in[128]);
  BENCH("Example10b", example10b(in,assign), Mi*4/128*512, digest_memory(&assign[0], &assign[128]));
 
  return 0;
}
