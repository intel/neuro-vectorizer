#include "header.h"

short input[256];
int   ia[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ input, int* __restrict__ ia) {
  int i;
  for (i = 0; i < 256; i+=2) {
    ia[i] = (int) input[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[256]);
  init_memory(&input[0], &input[256]);
  BENCH("Example10b", example10b(input,ia), Mi*4/256*512, digest_memory(&ia[0], &ia[256]));
 
  return 0;
}
