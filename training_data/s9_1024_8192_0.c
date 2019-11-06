#include "header.h"

int G[1024][8192];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<1024; i++) {
     for (j=0; j<8192; j++) {
       G[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&G[0][0], &G[0][8192]);
  BENCH("Example8",   example8(8), 128, digest_memory(&G[0][0], &G[0][8192]));
 
  return 0;
}
