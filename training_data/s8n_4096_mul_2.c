#include "header.h"

int output[4096] ALIGNED16;
int input[4096] ALIGNED16;

__attribute__((noinline))
void  example7 (int x) {
   int i;

   /* feature: support for read accesses with an unknown misalignment  */
   for (i=0; i<4096-x; i++){
      output[i] *= input[i+x];
   }
}

int main(int argc,char* argv[]){
  init_memory(&output[0], &output[4096]);
  init_memory(&input[0], &input[4096]);
 
  BENCH("Example7",   example7(4), Mi/4096*512, digest_memory(&output[0], &output[4096]));
  return 0;
}
