#include "header.h"

short short_a[256];
int   assign[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 256; i+=2) {
    assign[i] = (int) short_a[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[256]);
  init_memory(&short_a[0], &short_a[256]);
  BENCH("Example10b", example10b(short_a,assign), Mi*4/256*512, digest_memory(&assign[0], &assign[256]));
 
  return 0;
}
