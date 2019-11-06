#include "header.h"

int Out[16384][16384];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<16384; i++) {
     for (j=0; j<16384; j++) {
       Out[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Out[0][0], &Out[0][16384]);
  BENCH("Example8",   example8(8), 4, digest_memory(&Out[0][0], &Out[0][16384]));
 
  return 0;
}
