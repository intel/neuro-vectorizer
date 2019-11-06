#include "header.h"

int in[256] ALIGNED16;
int res[256] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<256; i++){
      int j = in[i];
      res[i] = (j > MAX ? MAX : 0);
   }
}

int main(int argc,char* argv[]){
  init_memory(&in[0], &in[256]);
  init_memory(&res[0], &res[256]);
  BENCH("Example4c",  example4c(), Mi*4/256*512, digest_memory(&res[0], &res[256]));
 
  
  return 0;
}
