#include "header.h"

int a[4096] ALIGNED16;
int b[4096] ALIGNED16;
int c[4096] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<4096; i++){
    a[i] = b[i] -c[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&a[0], &a[4096]);
  init_memory(&b[0], &b[4096]);
  init_memory(&c[0], &c[4096]);
  BENCH("Example1",   example1(), Mi*4/4096*256, digest_memory(&a[0], &a[4096]));
  
  
  return 0;
}
