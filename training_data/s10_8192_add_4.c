#include "header.h"

unsigned int op1[8192];
unsigned int op2[8192];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int sum_add = 0;
  for (i = 0; i < 8192; i++) {
    sum_add += (op1[i] +op2[i]);
  }

  *ret = sum_add;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&op1[0], &op1[8192]);
  init_memory(&op2[0], &op2[8192]);
  BENCH("Example9",   example9(&dummy), Mi*4/8192*512, dummy);
 
  return 0;
}
