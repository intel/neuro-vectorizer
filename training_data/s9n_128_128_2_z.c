#include "header.h"

int Output[128][128];
__attribute__((noinline))
void example8 (int z) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<128-1; i+=2) {
     for (j=0; j<128-1; j+=2) {
       Output[i][j] = z;
       Output[i+1][j] = z;
       Output[i][j+1] = z;
       Output[i+1][j+1] = z;

     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&Output[0][0], &Output[0][128]);
  BENCH("Example8",   example8(8), 65536, digest_memory(&Output[0][0], &Output[0][128]));
 
  return 0;
}
