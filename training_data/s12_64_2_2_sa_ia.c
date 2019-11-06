#include "header.h"

short sa[64];
int   ia[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 64-1; i+=2) {
    ia[i] = (int) sa[i];
ia[i+1] = (int) sa[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[64]);
  init_memory(&sa[0], &sa[64]);
  BENCH("Example10b", example10b(sa,ia), Mi*4/64*512, digest_memory(&ia[0], &ia[64]));
 
  return 0;
}
