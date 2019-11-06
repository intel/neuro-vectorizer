#include "header.h"

int G[4096][256];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<4096; i++) {
     for (j=0; j<256; j++) {
       G[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&G[0][0], &G[0][256]);
  BENCH("Example8",   example8(8), 1024, digest_memory(&G[0][0], &G[0][256]));
 
  return 0;
}
