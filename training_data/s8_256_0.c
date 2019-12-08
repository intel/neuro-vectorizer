#include "header.h"

int a[256] ALIGNED16;
int b[256] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<256-x; i++){
      a[i] = b[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[256]);
  init_memory(&b[0], &b[256]);
 
  BENCH("Example7",   example7(4), Mi/256*512, digest_memory(&a[0], &a[256]));
  return 0;
}
