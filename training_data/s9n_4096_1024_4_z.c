#include "header.h"

int assign[4096][1024];
__attribute__((noinline))
void example8 (int z) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<4096-1; i+=2) {
     for (j=0; j<1024-1; j+=2) {
       assign[i][j] = z;
       assign[i+1][j] = z;
       assign[i][j+1] = z;
       assign[i+1][j+1] = z;

     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&assign[0][0], &assign[0][1024]);
  BENCH("Example8",   example8(8), 256, digest_memory(&assign[0][0], &assign[0][1024]));
 
  return 0;
}
