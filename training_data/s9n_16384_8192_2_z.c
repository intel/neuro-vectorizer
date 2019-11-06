#include "header.h"

int Output[16384][8192];
__attribute__((noinline))
void example8 (int z) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<16384-1; i+=2) {
     for (j=0; j<8192-1; j+=2) {
       Output[i][j] = z;
       Output[i+1][j] = z;
       Output[i][j+1] = z;
       Output[i+1][j+1] = z;

     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Output[0][0], &Output[0][8192]);
  BENCH("Example8",   example8(8), 8, digest_memory(&Output[0][0], &Output[0][8192]));
 
  return 0;
}
