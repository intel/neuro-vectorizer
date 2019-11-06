
#include "header.h"
int out[64] ALIGNED16;
__attribute__((noinline))
void example2a (int z) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 64 ; i++) {
      out[i] = z;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&out[0], &out[64]);
  BENCH("Example2a",  example2a(2), Mi*4/64*512, digest_memory(&out[0], &out[64]));
  
  return 0;
}
