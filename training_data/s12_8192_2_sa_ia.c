#include "header.h"

short sa[8192];
int   ia[8192] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 8192; i+=2) {
    ia[i] = (int) sa[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[8192]);
  init_memory(&sa[0], &sa[8192]);
  BENCH("Example10b", example10b(sa,ia), Mi*4/8192*512, digest_memory(&ia[0], &ia[8192]));
 
  return 0;
}
