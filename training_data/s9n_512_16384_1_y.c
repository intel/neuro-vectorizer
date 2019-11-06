#include "header.h"

int Out[512][16384];
__attribute__((noinline))
void example8 (int y) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<512-1; i+=2) {
     for (j=0; j<16384-1; j+=2) {
       Out[i][j] = y;
       Out[i+1][j] = y;
       Out[i][j+1] = y;
       Out[i+1][j+1] = y;

     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Out[0][0], &Out[0][16384]);
  BENCH("Example8",   example8(8), 128, digest_memory(&Out[0][0], &Out[0][16384]));
 
  return 0;
}
