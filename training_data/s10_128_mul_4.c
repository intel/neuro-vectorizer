#include "header.h"

unsigned int op1[128];
unsigned int op2[128];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int sum_mul = 0;
  for (i = 0; i < 128; i++) {
    sum_mul += (op1[i] *op2[i]);
  }

  *ret = sum_mul;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&op1[0], &op1[128]);
  init_memory(&op2[0], &op2[128]);
  BENCH("Example9",   example9(&dummy), Mi*4/128*512, dummy);
 
  return 0;
}
