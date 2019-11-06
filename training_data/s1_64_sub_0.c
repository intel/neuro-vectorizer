#include "header.h"

int a[64] ALIGNED16;
int b[64] ALIGNED16;
int c[64] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<64; i++){
    a[i] = b[i] -c[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&a[0], &a[64]);
  init_memory(&b[0], &b[64]);
  init_memory(&c[0], &c[64]);
  BENCH("Example1",   example1(), Mi*4/64*256, digest_memory(&a[0], &a[64]));
  
  
  return 0;
}
