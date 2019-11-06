#include "header.h"

int result[128] ALIGNED16;
int in1[128] ALIGNED16;
int in2[128] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<128; i++){
    result[i] = in1[i] +in2[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&result[0], &result[128]);
  init_memory(&in1[0], &in1[128]);
  init_memory(&in2[0], &in2[128]);
  BENCH("Example1",   example1(), Mi*4/128*256, digest_memory(&result[0], &result[128]));
  
  
  return 0;
}
