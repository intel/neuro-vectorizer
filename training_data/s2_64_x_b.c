
#include "header.h"
int b[64] ALIGNED16;
__attribute__((noinline))
void example2a (int x) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 64 ; i++) {
      b[i] = x;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&b[0], &b[64]);
  BENCH("Example2a",  example2a(2), Mi*4/64*512, digest_memory(&b[0], &b[64]));
  
  return 0;
}
