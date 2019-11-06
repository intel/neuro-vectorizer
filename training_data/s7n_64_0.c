#include "header.h"

int a[64] ALIGNED16;
int b[64] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<64-1; i+=2){
      int j = a[i];
      b[i] = (j > MAX ? MAX : 0);
      b[i+1] = j;
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[64]);
  init_memory(&b[0], &b[64]);
  BENCH("Example4c",  example4c(), Mi*4/64*512, digest_memory(&b[0], &b[64]));
 
  
  return 0;
}
