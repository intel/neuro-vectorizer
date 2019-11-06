#include "header.h"

int Out[128][64];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<128; i++) {
     for (j=0; j<64; j++) {
       Out[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Out[0][0], &Out[0][64]);
  BENCH("Example8",   example8(8), 131072, digest_memory(&Out[0][0], &Out[0][64]));
 
  return 0;
}
