#include "header.h"

unsigned int in1[16384];
unsigned int in2[16384];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int difference = 0;
  for (i = 0; i < 16384; i++) {
    difference += (in1[i] *in2[i]);
  }

  *ret = difference;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&in1[0], &in1[16384]);
  init_memory(&in2[0], &in2[16384]);
  BENCH("Example9",   example9(&dummy), Mi*4/16384*512, dummy);
 
  return 0;
}
