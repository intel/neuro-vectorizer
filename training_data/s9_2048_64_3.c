#include "header.h"

int result[2048][64];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<2048; i++) {
     for (j=0; j<64; j++) {
       result[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&result[0][0], &result[0][64]);
  BENCH("Example8",   example8(8), 8192, digest_memory(&result[0][0], &result[0][64]));
 
  return 0;
}
