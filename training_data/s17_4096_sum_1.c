#include "header.h"

int   ia[4096] ALIGNED16;

__attribute__((noinline))
void example21(int *in, int n) {
  int i, sum = 0;

  for (i = n-1; i >= 0; i--)
    sum += in[i];

  in[0] = sum;
}


int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4096]);
  BENCH("Example21",  example21(ia, 4096), Mi*4/4096*512, digest_memory(&ia[0], &ia[4096]));
  return 0;
}
