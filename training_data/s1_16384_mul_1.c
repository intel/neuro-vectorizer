#include "header.h"

int out[16384] ALIGNED16;
int in1[16384] ALIGNED16;
int in2[16384] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<16384; i++){
    out[i] = in1[i] *in2[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&out[0], &out[16384]);
  init_memory(&in1[0], &in1[16384]);
  init_memory(&in2[0], &in2[16384]);
  BENCH("Example1",   example1(), Mi*4/16384*256, digest_memory(&out[0], &out[16384]));
  
  
  return 0;
}
