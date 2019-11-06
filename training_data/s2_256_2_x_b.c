
#include "header.h"
int b[256] ALIGNED16;
__attribute__((noinline))
void example2a (int x) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 256 ; i+=2) {
      b[i] = x;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&b[0], &b[256]);
  BENCH("Example2a",  example2a(2), Mi*4/256*512, digest_memory(&b[0], &b[256]));
  
  return 0;
}
