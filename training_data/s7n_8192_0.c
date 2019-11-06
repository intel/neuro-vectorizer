#include "header.h"

int a[8192] ALIGNED16;
int b[8192] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<8192-1; i+=2){
      int j = a[i];
      b[i] = (j > MAX ? MAX : 0);
      b[i+1] = j;
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[8192]);
  init_memory(&b[0], &b[8192]);
  BENCH("Example4c",  example4c(), Mi*4/8192*512, digest_memory(&b[0], &b[8192]));
 
  
  return 0;
}
