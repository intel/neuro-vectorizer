
#include "header.h"
int b[8192] ALIGNED16;
__attribute__((noinline))
void example2a (int y) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 8192 ; i++) {
      b[i] = y;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&b[0], &b[8192]);
  BENCH("Example2a",  example2a(2), Mi*4/8192*512, digest_memory(&b[0], &b[8192]));
  
  return 0;
}
