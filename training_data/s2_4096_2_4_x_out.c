
#include "header.h"
int out[4096] ALIGNED16;
__attribute__((noinline))
void example2a (int x) {
   int i;

   /* feature: support for unknown loop bound  */
   /* feature: support for loop invariants  */
   for (i=0; i< 4096-3 ; i+=4) {
      out[i] = x;
out[i+1] = -x;
out[i+2]=x;
out[i+3]=-x;
   }
}
int main(int argc,char* argv[]){
 
  init_memory(&out[0], &out[4096]);
  BENCH("Example2a",  example2a(2), Mi*4/4096*512, digest_memory(&out[0], &out[4096]));
  
  return 0;
}
