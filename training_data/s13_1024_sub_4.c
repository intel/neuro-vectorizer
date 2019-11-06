#include "header.h"

int result1[1024] ALIGNED16;
int in1[1024] ALIGNED16;
int in2[1024] ALIGNED16;
int result2[1024] ALIGNED16;

__attribute__((noinline))
void example11() {
   int i;
  for (i = 0; i < 1024/4; i++){
    result1[i] = in1[2*i+1] * in2[2*i+1] - in1[2*i] * in2[2*i];
    result2[i] = in1[2*i] * in2[2*i+1] -in1[2*i+1] * in2[2*i];
  }
}

int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[1024]);
  init_memory(&in1[0], &in1[1024]);
  init_memory(&in2[0], &in2[1024]);
  init_memory(&result2[0], &result2[1024]);
  BENCH("Example11",  example11(), Mi*4/1024*256, digest_memory(&result2[0], &result2[1024]));
 
  return 0;
}
