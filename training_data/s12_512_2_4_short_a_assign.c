#include "header.h"

short short_a[512];
int   assign[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 512-3; i+=4) {
    assign[i] = (int) short_a[i];
assign[i+1] = (int) short_a[i+1];
assign[i+2] = (int) short_a[i+2];
assign[i+3] = (int) short_a[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[512]);
  init_memory(&short_a[0], &short_a[512]);
  BENCH("Example10b", example10b(short_a,assign), Mi*4/512*512, digest_memory(&assign[0], &assign[512]));
 
  return 0;
}
