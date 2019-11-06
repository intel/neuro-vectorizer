
#include "header.h"
int output[512] ALIGNED16;
__attribute__((noinline))
void example2a (int x) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 512 ; i++) {
      output[i] = x;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&output[0], &output[512]);
  BENCH("Example2a",  example2a(2), Mi*4/512*512, digest_memory(&output[0], &output[512]));
  
  return 0;
}
