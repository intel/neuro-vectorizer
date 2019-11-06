#include "header.h"

int a[2048] ALIGNED16;
int b[2048] ALIGNED16;
int c[2048] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<2048; i++){
    a[i] = b[i] *c[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&a[0], &a[2048]);
  init_memory(&b[0], &b[2048]);
  init_memory(&c[0], &c[2048]);
  BENCH("Example1",   example1(), Mi*4/2048*256, digest_memory(&a[0], &a[2048]));
  
  
  return 0;
}
