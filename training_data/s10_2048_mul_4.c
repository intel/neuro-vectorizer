#include "header.h"

unsigned int op1[2048];
unsigned int op2[2048];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int sum_diff = 0;
  for (i = 0; i < 2048; i++) {
    sum_diff += (op1[i] *op2[i]);
  }

  *ret = sum_diff;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&op1[0], &op1[2048]);
  init_memory(&op2[0], &op2[2048]);
  BENCH("Example9",   example9(&dummy), Mi*4/2048*512, dummy);
 
  return 0;
}
