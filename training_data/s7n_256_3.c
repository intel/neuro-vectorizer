#include "header.h"

int input[256] ALIGNED16;
int result[256] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<256-1; i+=2){
      int j = input[i];
      result[i] = (j > MAX ? MAX : 0);
      result[i+1] = j;
   }
}

int main(int argc,char* argv[]){
  init_memory(&input[0], &input[256]);
  init_memory(&result[0], &result[256]);
  BENCH("Example4c",  example4c(), Mi*4/256*512, digest_memory(&result[0], &result[256]));
 
  
  return 0;
}
