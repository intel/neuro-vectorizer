
#include "header.h"
int output[8192] ALIGNED16;
__attribute__((noinline))
void example2a (int z) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 8192-3 ; i+=4) {
      output[i] = z;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&output[0], &output[8192]);
  BENCH("Example2a",  example2a(2), Mi*4/8192*512, digest_memory(&output[0], &output[8192]));
  
  return 0;
}
