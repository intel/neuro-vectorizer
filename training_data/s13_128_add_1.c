#include "header.h"

int out1[128] ALIGNED16;
int in1[128] ALIGNED16;
int in2[128] ALIGNED16;
int out2[128] ALIGNED16;

__attribute__((noinline))
void example11() {
   int i;
  for (i = 0; i < 128/4; i++){
    out1[i] = in1[2*i+1] * in2[2*i+1] - in1[2*i] * in2[2*i];
    out2[i] = in1[2*i] * in2[2*i+1] +in1[2*i+1] * in2[2*i];
  }
}

int main(int argc,char* argv[]){
  init_memory(&out1[0], &out1[128]);
  init_memory(&in1[0], &in1[128]);
  init_memory(&in2[0], &in2[128]);
  init_memory(&out2[0], &out2[128]);
  BENCH("Example11",  example11(), Mi*4/128*256, digest_memory(&out2[0], &out2[128]));
 
  return 0;
}
