#include "header.h"

int a[256] ALIGNED16;
int b[256] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<256; i++){
      int j = a[i];
      b[i] = (j > MAX ? MAX : 0);
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[256]);
  init_memory(&b[0], &b[256]);
  BENCH("Example4c",  example4c(), Mi*4/256*512, digest_memory(&b[0], &b[256]));
 
  
  return 0;
}
