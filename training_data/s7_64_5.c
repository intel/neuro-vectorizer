#include "header.h"

int i1[64] ALIGNED16;
int o[64] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<64; i++){
      int j = i1[i];
      o[i] = (j > MAX ? MAX : 0);
   }
}

int main(int argc,char* argv[]){
  init_memory(&i1[0], &i1[64]);
  init_memory(&o[0], &o[64]);
  BENCH("Example4c",  example4c(), Mi*4/64*512, digest_memory(&o[0], &o[64]));
 
  
  return 0;
}
