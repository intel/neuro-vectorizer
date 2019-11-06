#include "header.h"

int a[512] ALIGNED16;
int b[512] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<512-x; i++){
      a[i] *= b[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[512]);
  init_memory(&b[0], &b[512]);
 
  BENCH("Example7",   example7(4), Mi*8/512*512, digest_memory(&a[0], &a[512]));
  return 0;
}
