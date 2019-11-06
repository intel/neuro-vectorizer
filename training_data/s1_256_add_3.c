#include "header.h"

int o[256] ALIGNED16;
int i1[256] ALIGNED16;
int i2[256] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<256; i++){
    o[i] = i1[i] +i2[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&o[0], &o[256]);
  init_memory(&i1[0], &i1[256]);
  init_memory(&i2[0], &i2[256]);
  BENCH("Example1",   example1(), Mi*4/256*256, digest_memory(&o[0], &o[256]));
  
  
  return 0;
}
