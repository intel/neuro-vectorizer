
#include "header.h"
int b[512] ALIGNED16;
__attribute__((noinline))
void example2a (int z) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 512 ; i+=2) {
      b[i] = z;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&b[0], &b[512]);
  BENCH("Example2a",  example2a(2), Mi*4/512*512, digest_memory(&b[0], &b[512]));
  
  return 0;
}
