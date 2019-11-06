#include "header.h"

int a[128] ALIGNED16;
int b[128] ALIGNED16;
int c[128] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<128-4; i++){
      a[i] = b[i+1] +c[i+3];
   }
}

int main(int argc,char* argv[]){
  init_memory(&a[0], &a[128]);
  init_memory(&b[0], &b[128]);
  init_memory(&c[0], &c[128]);
 
  BENCH("Example4b",  example4b(), Mi*2/128*512, digest_memory(&a[0], &a[128]));
  
  return 0;
}
