#include "header.h"

short in[8192];
int   assign[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 8192-1; i+=2) {
    assign[i] = (int) in[i];
assign[i+1] = (int) in[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[8192]);
  init_memory(&in[0], &in[8192]);
  BENCH("Example10b", example10b(in,assign), Mi*4/8192*512, digest_memory(&assign[0], &assign[8192]));
 
  return 0;
}
