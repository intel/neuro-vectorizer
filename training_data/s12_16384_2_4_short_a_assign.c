#include "header.h"

short short_a[16384];
int   assign[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 16384-3; i+=4) {
    assign[i] = (int) short_a[i];
assign[i+1] = (int) short_a[i+1];
assign[i+2] = (int) short_a[i+2];
assign[i+3] = (int) short_a[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[16384]);
  init_memory(&short_a[0], &short_a[16384]);
  BENCH("Example10b", example10b(short_a,assign), Mi*4/16384*512, digest_memory(&assign[0], &assign[16384]));
 
  return 0;
}
