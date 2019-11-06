#include "header.h"

int in[1024] ALIGNED16;
int res[1024] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<1024-1; i+=2){
      int j = in[i];
      res[i] = (j > MAX ? MAX : 0);
      res[i+1] = j;
   }
}

int main(int argc,char* argv[]){
  init_memory(&in[0], &in[1024]);
  init_memory(&res[0], &res[1024]);
  BENCH("Example4c",  example4c(), Mi*4/1024*512, digest_memory(&res[0], &res[1024]));
 
  
  return 0;
}
