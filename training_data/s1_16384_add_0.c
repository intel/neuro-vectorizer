#include "header.h"

int a[16384] ALIGNED16;
int b[16384] ALIGNED16;
int c[16384] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<16384; i++){
    a[i] = b[i] +c[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&a[0], &a[16384]);
  init_memory(&b[0], &b[16384]);
  init_memory(&c[0], &c[16384]);
  BENCH("Example1",   example1(), Mi*4/16384*256, digest_memory(&a[0], &a[16384]));
  
  
  return 0;
}
