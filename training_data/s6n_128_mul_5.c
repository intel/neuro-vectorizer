#include "header.h"

int sum[128] ALIGNED16;
int in1[128] ALIGNED16;
int in2[128] ALIGNED16;

__attribute__((noinline))
void example4b () {
   int i;

   /* feature: support for read accesses with a compile time known misalignment  */
   for (i=0; i<128-4; i+=2){
      sum[i] = in1[i+1] *in2[i+3];
      sum[i+1]=in1[i] *in2[i+2];
   }
}

int main(int argc,char* argv[]){
  init_memory(&sum[0], &sum[128]);
  init_memory(&in1[0], &in1[128]);
  init_memory(&in2[0], &in2[128]);
 
  BENCH("Example4b",  example4b(), Mi*2/128*512, digest_memory(&sum[0], &sum[128]));
  
  return 0;
}
