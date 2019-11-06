#include "header.h"

int sum_mul1[8192] ALIGNED16;
int in1[8192] ALIGNED16;
int in2[8192] ALIGNED16;
int sum_mul2[8192] ALIGNED16;

__attribute__((noinline))
void example11() {
   int i;
  for (i = 0; i < 8192/4; i++){
    sum_mul1[i] = in1[2*i+1] * in2[2*i+1] - in1[2*i] * in2[2*i];
    sum_mul2[i] = in1[2*i] * in2[2*i+1] *in1[2*i+1] * in2[2*i];
  }
}

int main(int argc,char* argv[]){
  init_memory(&sum_mul1[0], &sum_mul1[8192]);
  init_memory(&in1[0], &in1[8192]);
  init_memory(&in2[0], &in2[8192]);
  init_memory(&sum_mul2[0], &sum_mul2[8192]);
  BENCH("Example11",  example11(), Mi*4/8192*256, digest_memory(&sum_mul2[0], &sum_mul2[8192]));
 
  return 0;
}
