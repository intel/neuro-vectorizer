#include "header.h"

int in[64] ALIGNED16;
int out[64] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<64; i++){
      int j = in[i];
      out[i] = (j > MAX ? MAX : 0);
   }
}

int main(int argc,char* argv[]){
  init_memory(&in[0], &in[64]);
  init_memory(&out[0], &out[64]);
  BENCH("Example4c",  example4c(), Mi*4/64*512, digest_memory(&out[0], &out[64]));
 
  
  return 0;
}
