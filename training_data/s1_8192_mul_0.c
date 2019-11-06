#include "header.h"

int a[8192] ALIGNED16;
int b[8192] ALIGNED16;
int c[8192] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<8192; i++){
    a[i] = b[i] *c[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&a[0], &a[8192]);
  init_memory(&b[0], &b[8192]);
  init_memory(&c[0], &c[8192]);
  BENCH("Example1",   example1(), Mi*4/8192*256, digest_memory(&a[0], &a[8192]));
  
  
  return 0;
}
