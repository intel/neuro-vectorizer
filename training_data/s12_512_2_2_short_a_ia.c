#include "header.h"

short short_a[512];
int   ia[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 512-1; i+=2) {
    ia[i] = (int) short_a[i];
ia[i+1] = (int) short_a[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[512]);
  init_memory(&short_a[0], &short_a[512]);
  BENCH("Example10b", example10b(short_a,ia), Mi*4/512*512, digest_memory(&ia[0], &ia[512]));
 
  return 0;
}
