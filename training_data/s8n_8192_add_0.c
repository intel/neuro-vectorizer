#include "header.h"

int a[8192] ALIGNED16;
int b[8192] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<8192-x; i++){
      a[i] += b[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[8192]);
  init_memory(&b[0], &b[8192]);
 
  BENCH("Example7",   example7(4), Mi*8/8192*512, digest_memory(&a[0], &a[8192]));
  return 0;
}
