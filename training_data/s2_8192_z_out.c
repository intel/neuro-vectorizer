
#include "header.h"
int out[8192] ALIGNED16;
__attribute__((noinline))
void example2a (int z) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 8192 ; i++) {
      out[i] = z;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&out[0], &out[8192]);
  BENCH("Example2a",  example2a(2), Mi*4/8192*512, digest_memory(&out[0], &out[8192]));
  
  return 0;
}
