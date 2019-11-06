#include "header.h"

int assign[8192][4096];
__attribute__((noinline))
void example8 (int y) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<8192-1; i+=2) {
     for (j=0; j<4096-1; j+=2) {
       assign[i][j] = y;
       assign[i+1][j] = y;
       assign[i][j+1] = y;
       assign[i+1][j+1] = y;

     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&assign[0][0], &assign[0][4096]);
  BENCH("Example8",   example8(8), 32, digest_memory(&assign[0][0], &assign[0][4096]));
 
  return 0;
}
