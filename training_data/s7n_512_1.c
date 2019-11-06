#include "header.h"

int in[512] ALIGNED16;
int out[512] ALIGNED16;

__attribute__((noinline))
void example4c (){
   int i;
    const int MAX = 4;
   /* feature: support for if-conversion  */
   for (i=0; i<512-1; i+=2){
      int j = in[i];
      out[i] = (j > MAX ? MAX : 0);
      out[i+1] = j;
   }
}

int main(int argc,char* argv[]){
  init_memory(&in[0], &in[512]);
  init_memory(&out[0], &out[512]);
  BENCH("Example4c",  example4c(), Mi*4/512*512, digest_memory(&out[0], &out[512]));
 
  
  return 0;
}
