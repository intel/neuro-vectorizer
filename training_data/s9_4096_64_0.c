#include "header.h"

int G[4096][64];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<4096; i++) {
     for (j=0; j<64; j++) {
       G[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&G[0][0], &G[0][64]);
  BENCH("Example8",   example8(8), 4096, digest_memory(&G[0][0], &G[0][64]));
 
  return 0;
}
