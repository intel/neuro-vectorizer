#include "header.h"

int Out[2048][256];
__attribute__((noinline))
void example8 (int y) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<2048-1; i+=2) {
     for (j=0; j<256-1; j+=2) {
       Out[i][j] = y;
       Out[i+1][j] = y;
       Out[i][j+1] = y;
       Out[i+1][j+1] = y;

     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Out[0][0], &Out[0][256]);
  BENCH("Example8",   example8(8), 2048, digest_memory(&Out[0][0], &Out[0][256]));
 
  return 0;
}
