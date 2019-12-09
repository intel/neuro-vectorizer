#include "header.h"

int   ia[512] ALIGNED16;

__attribute__((noinline))
void example21(int *i1, int n) {
  int i, out = 1;

  for (i = n-1; i >= 0; i--)
    out *= i1[i];

  i1[0] = out;
}


int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[512]);
  BENCH("Example21",  example21(ia, 512), Mi/512*512, digest_memory(&ia[0], &ia[512]));
  return 0;
}
