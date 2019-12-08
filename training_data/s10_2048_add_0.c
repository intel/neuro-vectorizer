#include "header.h"

unsigned int ub[2048];
unsigned int uc[2048];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int add = 0;
  for (i = 0; i < 2048; i++) {
    add += (ub[i] +uc[i]);
  }

  *ret = add;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&ub[0], &ub[2048]);
  init_memory(&uc[0], &uc[2048]);
  BENCH("Example9",   example9(&dummy), Mi*4/2048*512, dummy);
 
  return 0;
}
