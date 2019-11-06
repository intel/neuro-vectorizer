
#include "header.h"
int out[16384] ALIGNED16;
__attribute__((noinline))
void example2a (int z) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 16384-1 ; i+=2) {
      out[i] = z;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&out[0], &out[16384]);
  BENCH("Example2a",  example2a(2), Mi*4/16384*512, digest_memory(&out[0], &out[16384]));
  
  return 0;
}
