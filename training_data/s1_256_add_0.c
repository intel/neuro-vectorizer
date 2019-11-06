#include "header.h"

int a[256] ALIGNED16;
int b[256] ALIGNED16;
int c[256] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<256; i++){
    a[i] = b[i] +c[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&a[0], &a[256]);
  init_memory(&b[0], &b[256]);
  init_memory(&c[0], &c[256]);
  BENCH("Example1",   example1(), Mi*4/256*256, digest_memory(&a[0], &a[256]));
  
  
  return 0;
}
