#include "header.h"

int result[512][4096];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<512; i++) {
     for (j=0; j<4096; j++) {
       result[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&result[0][0], &result[0][4096]);
  BENCH("Example8",   example8(8), 512, digest_memory(&result[0][0], &result[0][4096]));
 
  return 0;
}
