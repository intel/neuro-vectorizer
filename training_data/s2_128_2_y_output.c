
#include "header.h"
int output[128] ALIGNED16;
__attribute__((noinline))
void example2a (int y) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 128 ; i+=2) {
      output[i] = y;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&output[0], &output[128]);
  BENCH("Example2a",  example2a(2), Mi*4/128*512, digest_memory(&output[0], &output[128]));
  
  return 0;
}
