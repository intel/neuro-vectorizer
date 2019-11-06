#include "header.h"

int assign[1024][8192];
__attribute__((noinline))
void example8 (int x) {
   int i,j;

   /* feature: support for multidimensional arrays  */
   for (i=0; i<1024; i++) {
     for (j=0; j<8192; j++) {
       assign[i][j] = x;
     }
   }
}


int main(int argc,char* argv[]){
  init_memory(&assign[0][0], &assign[0][8192]);
  BENCH("Example8",   example8(8), 128, digest_memory(&assign[0][0], &assign[0][8192]));
 
  return 0;
}
