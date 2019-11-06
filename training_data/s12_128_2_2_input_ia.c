#include "header.h"

short input[128];
int   ia[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 128-1; i+=2) {
    ia[i] = (int) input[i];
ia[i+1] = (int) input[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&input[0], &input[128]);
  BENCH("Example10b", example10b(input,ia), Mi*4/128*512, digest_memory(&ia[0], &ia[128]));
 
  return 0;
}
