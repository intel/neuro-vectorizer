#include "header.h"

int   ia[16384] ALIGNED16;

__attribute__((noinline))
void example21(int *in_array, int n) {
  int i, result = 0;

  for (i = n-1; i >= 0; i--)
    result -= in_array[i];

  in_array[0] = result;
}


int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[16384]);
  BENCH("Example21",  example21(ia, 16384), Mi*4/16384*512, digest_memory(&ia[0], &ia[16384]));
  return 0;
}
