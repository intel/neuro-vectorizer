#include "header.h"

int   ia[256] ALIGNED16;

__attribute__((noinline))
void example21(int *in_array, int n) {
  int i, mul = 1;

  for (i = n-1; i >= 0; i--)
    mul *= in_array[i];

  in_array[0] = mul;
}


int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[256]);
  BENCH("Example21",  example21(ia, 256), Mi/256*512, digest_memory(&ia[0], &ia[256]));
  return 0;
}
