#include "header.h"

short sa[4096];
short sb[4096];
int   assign1[4096] ALIGNED16;
int   assign2[4096] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ assign1, int* __restrict__ assign2) {
  int i;
  for (i = 0; i < 4096-1; i+=2) {
    assign1[i] = (int) sa[i];
assign1[i+1] = (int) sa[i+1];
    assign2[i] = (int) sb[i];
assign2[i+1] = (int) sb[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[4096]);
  init_memory(&assign2[0], &assign2[4096]);
  init_memory(&sa[0], &sa[4096]);
  init_memory(&sb[0], &sb[4096]);
  BENCH("Example10b", example10b(sa,sb,assign1,assign2), Mi*4/4096*512, digest_memory(&assign1[0], &assign1[4096])+digest_memory(&assign2[0], &assign2[4096]));
 
  return 0;
}
