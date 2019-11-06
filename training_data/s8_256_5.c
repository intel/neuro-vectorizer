#include "header.h"

int o[256] ALIGNED16;
int i1[256] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<256-x; i++){
      o[i] = i1[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&o[0], &o[256]);
  init_memory(&i1[0], &i1[256]);
 
  BENCH("Example7",   example7(4), Mi*8/256*512, digest_memory(&o[0], &o[256]));
  return 0;
}
