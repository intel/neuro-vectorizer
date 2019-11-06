#include "header.h"

int o1[16384] ALIGNED16;
int i1[16384] ALIGNED16;
int i2[16384] ALIGNED16;
int o2[16384] ALIGNED16;

__attribute__((noinline))
void example11() {
   int i;
  for (i = 0; i < 16384/4; i++){
    o1[i] = i1[2*i+1] * i2[2*i+1] - i1[2*i] * i2[2*i];
    o2[i] = i1[2*i] * i2[2*i+1] *i1[2*i+1] * i2[2*i];
  }
}

int main(int argc,char* argv[]){
  init_memory(&o1[0], &o1[16384]);
  init_memory(&i1[0], &i1[16384]);
  init_memory(&i2[0], &i2[16384]);
  init_memory(&o2[0], &o2[16384]);
  BENCH("Example11",  example11(), Mi*4/16384*256, digest_memory(&o2[0], &o2[16384]));
 
  return 0;
}
