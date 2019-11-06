#include "header.h"

short sa[16384];
short sb[16384];
int   assign1[16384] ALIGNED16;
int   assign2[16384] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ assign1, int* __restrict__ assign2) {
  int i;
  for (i = 0; i < 16384; i+=2) {
    assign1[i] = (int) sa[i];
    assign2[i] = (int) sb[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[16384]);
  init_memory(&assign2[0], &assign2[16384]);
  init_memory(&sa[0], &sa[16384]);
  init_memory(&sb[0], &sb[16384]);
  BENCH("Example10b", example10b(sa,sb,assign1,assign2), Mi*4/16384*512, digest_memory(&assign1[0], &assign1[16384])+digest_memory(&assign2[0], &assign2[16384]));
 
  return 0;
}
