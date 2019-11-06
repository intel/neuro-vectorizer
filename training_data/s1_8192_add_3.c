#include "header.h"

int o[8192] ALIGNED16;
int i1[8192] ALIGNED16;
int i2[8192] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<8192; i++){
    o[i] = i1[i] +i2[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&o[0], &o[8192]);
  init_memory(&i1[0], &i1[8192]);
  init_memory(&i2[0], &i2[8192]);
  BENCH("Example1",   example1(), Mi*4/8192*256, digest_memory(&o[0], &o[8192]));
  
  
  return 0;
}
