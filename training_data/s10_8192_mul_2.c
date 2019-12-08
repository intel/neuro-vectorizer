#include "header.h"

unsigned int in1[8192];
unsigned int in2[8192];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int output = 0;
  for (i = 0; i < 8192; i++) {
    output += (in1[i] *in2[i]);
  }

  *ret = output;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&in1[0], &in1[8192]);
  init_memory(&in2[0], &in2[8192]);
  BENCH("Example9",   example9(&dummy), Mi*4/8192*512, dummy);
 
  return 0;
}
