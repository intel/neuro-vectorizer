#include "header.h"

int input[128] ALIGNED16;
int result[128] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<128-1; i+=2){
      int j = input[i];
      result[i] = (j > MAX ? MAX : 0);
      result[i+1] = j;
   }
}

int main(int argc,char* argv[]){
  init_memory(&input[0], &input[128]);
  init_memory(&result[0], &result[128]);
  BENCH("Example4c",  example4c(), Mi*4/128*512, digest_memory(&result[0], &result[128]));
 
  
  return 0;
}
