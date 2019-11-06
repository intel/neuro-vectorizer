#include "header.h"

short short_a[128];
int   ia[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ short_a, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 128-3; i+=4) {
    ia[i] = (int) short_a[i];
ia[i+1] = (int) short_a[i+1];
ia[i+2] = (int) short_a[i+2];
ia[i+3] = (int) short_a[i+3];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&short_a[0], &short_a[128]);
  BENCH("Example10b", example10b(short_a,ia), Mi*4/128*512, digest_memory(&ia[0], &ia[128]));
 
  return 0;
}
