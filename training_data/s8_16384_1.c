#include "header.h"

int out[16384] ALIGNED16;
int in[16384] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<16384-x; i++){
      out[i] = in[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&out[0], &out[16384]);
  init_memory(&in[0], &in[16384]);
 
  BENCH("Example7",   example7(4), Mi/16384*512, digest_memory(&out[0], &out[16384]));
  return 0;
}
