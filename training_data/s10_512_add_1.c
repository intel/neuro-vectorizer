#include "header.h"

unsigned int in1[512];
unsigned int in2[512];

__attribute__((noinline))
void example9 (unsigned *ret) {
  int i;

  /* feature: support summation reduction.
     note: in case of floats use -funsafe-math-optimizations  */
  unsigned int res = 0;
  for (i = 0; i < 512; i++) {
    res += (in1[i] +in2[i]);
  }

  *ret = res;
}

int main(int argc,char* argv[]){
  unsigned dummy = 0;
  init_memory(&in1[0], &in1[512]);
  init_memory(&in2[0], &in2[512]);
  BENCH("Example9",   example9(&dummy), Mi*4/512*512, dummy);
 
  return 0;
}
