#include "header.h"

short sa[256];
int   ia[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 256; i++) {
    ia[i] = (int) sa[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[256]);
  init_memory(&sa[0], &sa[256]);
  BENCH("Example10b", example10b(sa,ia), Mi*4/256*512, digest_memory(&ia[0], &ia[256]));
 
  return 0;
}
