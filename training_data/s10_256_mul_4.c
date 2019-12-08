#include "header.h"

unsigned int op1[256];
unsigned int op2[256];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int sum_mul = 0;
  for (i = 0; i < 256; i++) {
    sum_mul += (op1[i] *op2[i]);
  }

  *ret = sum_mul;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&op1[0], &op1[256]);
  init_memory(&op2[0], &op2[256]);
  BENCH("Example9",   example9(&dummy), Mi*4/256*512, dummy);
 
  return 0;
}
