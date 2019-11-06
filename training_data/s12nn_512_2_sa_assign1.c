#include "header.h"

short sa[512];
short sb[512];
short sc[512];
int   assign1[512] ALIGNED16;
int   assign2[512] ALIGNED16;
int   assign3[512] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ assign1, int* __restrict__ assign2, int* __restrict__ assign3) {
  int i;
  for (i = 0; i < 512; i+=2) {
    assign1[i] = (int) sa[i];
    assign2[i] = (int) sb[i];
    assign3[i] = (int) sc[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[512]);
  init_memory(&assign2[0], &assign2[512]);
  init_memory(&assign3[0], &assign3[512]);
  init_memory(&sa[0], &sa[512]);
  init_memory(&sb[0], &sb[512]);
  init_memory(&sc[0], &sc[512]);
  BENCH("Example10b", example10b(sa,sb,sc,assign1,assign2,assign3), Mi*4/512*512, digest_memory(&assign1[0], &assign1[512])+digest_memory(&assign2[0], &assign2[512])+digest_memory(&assign3[0], &assign3[512]));
 
  return 0;
}
