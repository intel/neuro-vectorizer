#include "header.h"

short sa[1024];
int   assign[1024] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ assign) {
  int i;
  for (i = 0; i < 1024; i++) {
    assign[i] = (int) sa[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign[0], &assign[1024]);
  init_memory(&sa[0], &sa[1024]);
  BENCH("Example10b", example10b(sa,assign), Mi*4/1024*512, digest_memory(&assign[0], &assign[1024]));
 
  return 0;
}
