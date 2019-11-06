
#include "header.h"
int out[128] ALIGNED16;
__attribute__((noinline))
void example2a (int y) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 128-1 ; i+=2) {
      out[i] = y;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&out[0], &out[128]);
  BENCH("Example2a",  example2a(2), Mi*4/128*512, digest_memory(&out[0], &out[128]));
  
  return 0;
}
