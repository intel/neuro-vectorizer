#include "header.h"

int   ia[1024] ALIGNED16;

__attribute__((noinline))
void example21(int *in_array, int n) {
  int i, sum = 0;

  for (i = n-1; i >= 0; i--)
    sum += in_array[i];

  in_array[0] = sum;
}


int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[1024]);
  BENCH("Example21",  example21(ia, 1024), Mi*4/1024*512, digest_memory(&ia[0], &ia[1024]));
  return 0;
}
