#include "header.h"

int G[4096][8192];
__attribute__((noinline))
void example8 (int y) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<4096-1; i+=2) {
     for (j=0; j<8192-1; j+=2) {
       G[i][j] = y;
       G[i+1][j] = y;
       G[i][j+1] = y;
       G[i+1][j+1] = y;

     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&G[0][0], &G[0][8192]);
  BENCH("Example8",   example8(8), 32, digest_memory(&G[0][0], &G[0][8192]));
 
  return 0;
}
