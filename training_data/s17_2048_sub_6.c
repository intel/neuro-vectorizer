#include "header.h"

int   ia[2048] ALIGNED16;

__attribute__((noinline))
void example21(int *in, int n) {
  int i, output = 0;

  for (i = n-1; i >= 0; i--)
    output -= in[i];

  in[0] = output;
}


int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[2048]);
  BENCH("Example21",  example21(ia, 2048), Mi*4/2048*512, digest_memory(&ia[0], &ia[2048]));
  return 0;
}
