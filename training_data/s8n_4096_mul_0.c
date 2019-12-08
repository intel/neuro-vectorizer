#include "header.h"

int a[4096] ALIGNED16;
int b[4096] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<4096-x; i++){
      a[i] *= b[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[4096]);
  init_memory(&b[0], &b[4096]);
 
  BENCH("Example7",   example7(4), Mi/4096*512, digest_memory(&a[0], &a[4096]));
  return 0;
}
