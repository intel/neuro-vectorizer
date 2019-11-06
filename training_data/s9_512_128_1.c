#include "header.h"

int Out[512][128];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<512; i++) {
     for (j=0; j<128; j++) {
       Out[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Out[0][0], &Out[0][128]);
  BENCH("Example8",   example8(8), 16384, digest_memory(&Out[0][0], &Out[0][128]));
 
  return 0;
}
