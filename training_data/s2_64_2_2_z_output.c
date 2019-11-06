
#include "header.h"
int output[64] ALIGNED16;
__attribute__((noinline))
void example2a (int z) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 64-1 ; i+=2) {
      output[i] = z;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&output[0], &output[64]);
  BENCH("Example2a",  example2a(2), Mi*4/64*512, digest_memory(&output[0], &output[64]));
  
  return 0;
}
