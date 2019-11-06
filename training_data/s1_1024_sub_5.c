#include "header.h"

int sum[1024] ALIGNED16;
int in1[1024] ALIGNED16;
int in2[1024] ALIGNED16;
__attribute__((noinline))
void example1 () {
  int i;

  for (i=0; i<1024; i++){
    sum[i] = in1[i] -in2[i];
  }
}
int main(int argc,char* argv[]){
 
  init_memory(&sum[0], &sum[1024]);
  init_memory(&in1[0], &in1[1024]);
  init_memory(&in2[0], &in2[1024]);
  BENCH("Example1",   example1(), Mi*4/1024*256, digest_memory(&sum[0], &sum[1024]));
  
  
  return 0;
}
