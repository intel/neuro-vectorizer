
#include "header.h"
int b[128] ALIGNED16;
__attribute__((noinline))
void example2a (int x) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 128 ; i+=2) {
      b[i] = x;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&b[0], &b[128]);
  BENCH("Example2a",  example2a(2), Mi*4/128*512, digest_memory(&b[0], &b[128]));
  
  return 0;
}
