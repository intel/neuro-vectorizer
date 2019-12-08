#include "header.h"

unsigned int op1[64];
unsigned int op2[64];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int sum_mul = 0;
  for (i = 0; i < 64; i++) {
    sum_mul += (op1[i] *op2[i]);
  }

  *ret = sum_mul;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&op1[0], &op1[64]);
  init_memory(&op2[0], &op2[64]);
  BENCH("Example9",   example9(&dummy), Mi*4/64*512, dummy);
 
  return 0;
}
