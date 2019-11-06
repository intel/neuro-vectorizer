#include "header.h"

unsigned int ub[256];
unsigned int uc[256];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int diff = 0;
  for (i = 0; i < 256; i++) {
    diff += (ub[i] -uc[i]);
  }

  *ret = diff;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&ub[0], &ub[256]);
  init_memory(&uc[0], &uc[256]);
  BENCH("Example9",   example9(&dummy), Mi*4/256*512, dummy);
 
  return 0;
}
