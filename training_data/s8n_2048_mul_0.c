#include "header.h"

int a[2048] ALIGNED16;
int b[2048] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<2048-x; i++){
      a[i] *= b[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[2048]);
  init_memory(&b[0], &b[2048]);
 
  BENCH("Example7",   example7(4), Mi/2048*512, digest_memory(&a[0], &a[2048]));
  return 0;
}
