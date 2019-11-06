#include "header.h"

short input[1024];
int   assign[1024] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 1024-1; i+=2) {
    assign[i] = (int) input[i];
assign[i+1] = (int) input[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[1024]);
  init_memory(&input[0], &input[1024]);
  BENCH("Example10b", example10b(input,assign), Mi*4/1024*512, digest_memory(&assign[0], &assign[1024]));
 
  return 0;
}
