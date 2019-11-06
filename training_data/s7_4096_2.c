#include "header.h"

int input[4096] ALIGNED16;
int output[4096] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<4096; i++){
      int j = input[i];
      output[i] = (j > MAX ? MAX : 0);
   }
}

int main(int argc,char* argv[]){
  init_memory(&input[0], &input[4096]);
  init_memory(&output[0], &output[4096]);
  BENCH("Example4c",  example4c(), Mi*4/4096*512, digest_memory(&output[0], &output[4096]));
 
  
  return 0;
}
