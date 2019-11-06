#include "header.h"

unsigned int ub[64];
unsigned int uc[64];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int result = 0;
  for (i = 0; i < 64; i++) {
    result += (ub[i] *uc[i]);
  }

  *ret = result;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&ub[0], &ub[64]);
  init_memory(&uc[0], &uc[64]);
  BENCH("Example9",   example9(&dummy), Mi*4/64*512, dummy);
 
  return 0;
}
