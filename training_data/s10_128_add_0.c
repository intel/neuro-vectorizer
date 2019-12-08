#include "header.h"

unsigned int ub[128];
unsigned int uc[128];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int add = 0;
  for (i = 0; i < 128; i++) {
    add += (ub[i] +uc[i]);
  }

  *ret = add;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&ub[0], &ub[128]);
  init_memory(&uc[0], &uc[128]);
  BENCH("Example9",   example9(&dummy), Mi*4/128*512, dummy);
 
  return 0;
}
