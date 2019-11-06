#include "header.h"

unsigned int op1[512];
unsigned int op2[512];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int sum_diff = 0;
  for (i = 0; i < 512; i++) {
    sum_diff += (op1[i] +op2[i]);
  }

  *ret = sum_diff;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&op1[0], &op1[512]);
  init_memory(&op2[0], &op2[512]);
  BENCH("Example9",   example9(&dummy), Mi*4/512*512, dummy);
 
  return 0;
}
