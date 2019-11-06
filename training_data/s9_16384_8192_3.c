#include "header.h"

int result[16384][8192];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<16384; i++) {
     for (j=0; j<8192; j++) {
       result[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&result[0][0], &result[0][8192]);
  BENCH("Example8",   example8(8), 8, digest_memory(&result[0][0], &result[0][8192]));
 
  return 0;
}
