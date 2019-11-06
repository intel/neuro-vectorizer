#include "header.h"

int a[512] ALIGNED16;
int b[512] ALIGNED16;
int c[512] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<512; i++){
    a[i] = b[i] *c[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&a[0], &a[512]);
  init_memory(&b[0], &b[512]);
  init_memory(&c[0], &c[512]);
  BENCH("Example1",   example1(), Mi*4/512*256, digest_memory(&a[0], &a[512]));
  
  
  return 0;
}
