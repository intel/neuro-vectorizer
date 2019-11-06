
#include "header.h"
int output[16384] ALIGNED16;
__attribute__((noinline))
void example2a (int y) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 16384-1 ; i+=2) {
      output[i] = y;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&output[0], &output[16384]);
  BENCH("Example2a",  example2a(2), Mi*4/16384*512, digest_memory(&output[0], &output[16384]));
  
  return 0;
}
