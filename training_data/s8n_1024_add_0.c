#include "header.h"

int a[1024] ALIGNED16;
int b[1024] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<1024-x; i++){
      a[i] += b[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[1024]);
  init_memory(&b[0], &b[1024]);
 
  BENCH("Example7",   example7(4), Mi/1024*512, digest_memory(&a[0], &a[1024]));
  return 0;
}
