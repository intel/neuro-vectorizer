#include "header.h"

short short_a[128];
int   assign[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 128-3; i+=4) {
    assign[i] = (int) short_a[i];
assign[i+1] = (int) short_a[i+1];
assign[i+2] = (int) short_a[i+2];
assign[i+3] = (int) short_a[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[128]);
  init_memory(&short_a[0], &short_a[128]);
  BENCH("Example10b", example10b(short_a,assign), Mi*4/128*512, digest_memory(&assign[0], &assign[128]));
 
  return 0;
}
