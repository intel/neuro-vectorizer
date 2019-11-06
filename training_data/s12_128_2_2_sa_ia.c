#include "header.h"

short sa[128];
int   ia[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 128-1; i+=2) {
    ia[i] = (int) sa[i];
ia[i+1] = (int) sa[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&sa[0], &sa[128]);
  BENCH("Example10b", example10b(sa,ia), Mi*4/128*512, digest_memory(&ia[0], &ia[128]));
 
  return 0;
}
