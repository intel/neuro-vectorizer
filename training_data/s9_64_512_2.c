#include "header.h"

int Output[64][512];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<64; i++) {
     for (j=0; j<512; j++) {
       Output[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Output[0][0], &Output[0][512]);
  BENCH("Example8",   example8(8), 32768, digest_memory(&Output[0][0], &Output[0][512]));
 
  return 0;
}
