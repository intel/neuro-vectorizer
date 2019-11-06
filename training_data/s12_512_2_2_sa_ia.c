#include "header.h"

short sa[512];
int   ia[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 512-1; i+=2) {
    ia[i] = (int) sa[i];
ia[i+1] = (int) sa[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[512]);
  init_memory(&sa[0], &sa[512]);
  BENCH("Example10b", example10b(sa,ia), Mi*4/512*512, digest_memory(&ia[0], &ia[512]));
 
  return 0;
}
