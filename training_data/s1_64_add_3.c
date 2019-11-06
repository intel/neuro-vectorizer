#include "header.h"

int o[64] ALIGNED16;
int i1[64] ALIGNED16;
int i2[64] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<64; i++){
    o[i] = i1[i] +i2[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&o[0], &o[64]);
  init_memory(&i1[0], &i1[64]);
  init_memory(&i2[0], &i2[64]);
  BENCH("Example1",   example1(), Mi*4/64*256, digest_memory(&o[0], &o[64]));
  
  
  return 0;
}
