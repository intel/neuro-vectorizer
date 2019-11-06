#include "header.h"

int Out[16384][8192];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<16384-1; i+=2) {
     for (j=0; j<8192-1; j+=2) {
       Out[i][j] = x;
       Out[i+1][j] = x;
       Out[i][j+1] = x;
       Out[i+1][j+1] = x;

     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Out[0][0], &Out[0][8192]);
  BENCH("Example8",   example8(8), 8, digest_memory(&Out[0][0], &Out[0][8192]));
 
  return 0;
}
