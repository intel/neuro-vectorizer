#include "header.h"

int a[128] ALIGNED16;
int b[128] ALIGNED16;
int c[128] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<128; i++){
    a[i] = b[i] -c[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&a[0], &a[128]);
  init_memory(&b[0], &b[128]);
  init_memory(&c[0], &c[128]);
  BENCH("Example1",   example1(), Mi*4/128*256, digest_memory(&a[0], &a[128]));
  
  
  return 0;
}
