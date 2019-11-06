#include "header.h"

short sa[512];
short sb[512];
int   assign1[512] ALIGNED16;
int   assign2[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ assign1, int* __restrict__ assign2) {
  int i;
  for (i = 0; i < 512-1; i+=2) {
    assign1[i] = (int) sa[i];
assign1[i+1] = (int) sa[i+1];
    assign2[i] = (int) sb[i];
assign2[i+1] = (int) sb[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[512]);
  init_memory(&assign2[0], &assign2[512]);
  init_memory(&sa[0], &sa[512]);
  init_memory(&sb[0], &sb[512]);
  BENCH("Example10b", example10b(sa,sb,assign1,assign2), Mi*4/512*512, digest_memory(&assign1[0], &assign1[512])+digest_memory(&assign2[0], &assign2[512]));
 
  return 0;
}
