#include "header.h"

unsigned int ub[512];
unsigned int uc[512];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int mul = 0;
  for (i = 0; i < 512; i++) {
    mul += (ub[i] *uc[i]);
  }

  *ret = mul;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&ub[0], &ub[512]);
  init_memory(&uc[0], &uc[512]);
  BENCH("Example9",   example9(&dummy), Mi*4/512*512, dummy);
 
  return 0;
}
