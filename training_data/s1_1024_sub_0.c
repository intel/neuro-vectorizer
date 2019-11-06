#include "header.h"

int a[1024] ALIGNED16;
int b[1024] ALIGNED16;
int c[1024] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<1024; i++){
    a[i] = b[i] -c[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&a[0], &a[1024]);
  init_memory(&b[0], &b[1024]);
  init_memory(&c[0], &c[1024]);
  BENCH("Example1",   example1(), Mi*4/1024*256, digest_memory(&a[0], &a[1024]));
  
  
  return 0;
}
