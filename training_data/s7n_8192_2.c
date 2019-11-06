#include "header.h"

int input[8192] ALIGNED16;
int output[8192] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<8192-1; i+=2){
      int j = input[i];
      output[i] = (j > MAX ? MAX : 0);
      output[i+1] = j;
   }
}

int main(int argc,char* argv[]){
  init_memory(&input[0], &input[8192]);
  init_memory(&output[0], &output[8192]);
  BENCH("Example4c",  example4c(), Mi*4/8192*512, digest_memory(&output[0], &output[8192]));
 
  
  return 0;
}
