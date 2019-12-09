#include "header.h"

int   ia[4096] ALIGNED16;

__attribute__((noinline))
void example21(int *i1, int n) {
  int i, out = 0;

  for (i = n-1; i >= 0; i--)
    out -= i1[i];

  i1[0] = out;
}


int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4096]);
  BENCH("Example21",  example21(ia, 4096), Mi*4/4096*512, digest_memory(&ia[0], &ia[4096]));
  return 0;
}
