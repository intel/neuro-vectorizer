#include "header.h"

int res[8192] ALIGNED16;
int in[8192] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<8192-x; i++){
      res[i] -= in[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&res[0], &res[8192]);
  init_memory(&in[0], &in[8192]);
 
  BENCH("Example7",   example7(4), Mi*8/8192*512, digest_memory(&res[0], &res[8192]));
  return 0;
}
