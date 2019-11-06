#include "header.h"

int Output[2048][1024];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<2048; i++) {
     for (j=0; j<1024; j++) {
       Output[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Output[0][0], &Output[0][1024]);
  BENCH("Example8",   example8(8), 512, digest_memory(&Output[0][0], &Output[0][1024]));
 
  return 0;
}
