#include "header.h"

unsigned int ub[16384];
unsigned int uc[16384];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int sub = 0;
  for (i = 0; i < 16384; i++) {
    sub += (ub[i] -uc[i]);
  }

  *ret = sub;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&ub[0], &ub[16384]);
  init_memory(&uc[0], &uc[16384]);
  BENCH("Example9",   example9(&dummy), Mi*4/16384*512, dummy);
 
  return 0;
}
