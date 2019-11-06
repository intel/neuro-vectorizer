#include "header.h"

int i1[1024] ALIGNED16;
int o[1024] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<1024-1; i+=2){
      int j = i1[i];
      o[i] = (j > MAX ? MAX : 0);
      o[i+1] = j;
   }
}

int main(int argc,char* argv[]){
  init_memory(&i1[0], &i1[1024]);
  init_memory(&o[0], &o[1024]);
  BENCH("Example4c",  example4c(), Mi*4/1024*512, digest_memory(&o[0], &o[1024]));
 
  
  return 0;
}
