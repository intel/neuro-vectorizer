#include "header.h"

int a[256] ALIGNED16;
int b[256] ALIGNED16;
int c[256] ALIGNED16;
int d[256] ALIGNED16;

__attribute__((noinline))
void example11() {
   int i;
  for (i = 0; i < 256/4; i++){
    a[i] = b[2*i+1] * c[2*i+1] - b[2*i] * c[2*i];
    d[i] = b[2*i] * c[2*i+1] -b[2*i+1] * c[2*i];
  }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[256]);
  init_memory(&b[0], &b[256]);
  init_memory(&c[0], &c[256]);
  init_memory(&d[0], &d[256]);
  BENCH("Example11",  example11(), Mi*4/256*256, digest_memory(&d[0], &d[256]));
 
  return 0;
}
