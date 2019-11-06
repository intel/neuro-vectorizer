#include "header.h"

int Out[512][256];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<512-1; i+=2) {
     for (j=0; j<256-1; j+=2) {
       Out[i][j] = x;
       Out[i+1][j] = x;
       Out[i][j+1] = x;
       Out[i+1][j+1] = x;

     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Out[0][0], &Out[0][256]);
  BENCH("Example8",   example8(8), 8192, digest_memory(&Out[0][0], &Out[0][256]));
 
  return 0;
}
