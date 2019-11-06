#include "header.h"

int Output[8192][16384];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<8192; i++) {
     for (j=0; j<16384; j++) {
       Output[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Output[0][0], &Output[0][16384]);
  BENCH("Example8",   example8(8), 8, digest_memory(&Output[0][0], &Output[0][16384]));
 
  return 0;
}
