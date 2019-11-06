#include "header.h"

short sa[2048];
short sb[2048];
short sc[2048];
int   assign1[2048] ALIGNED16;
int   assign2[2048] ALIGNED16;
int   assign3[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ assign1, int* __restrict__ assign2, int* __restrict__ assign3) {
  int i;
  for (i = 0; i < 2048; i++) {
    assign1[i] = (int) sa[i];
    assign2[i] = (int) sb[i];
    assign3[i] = (int) sc[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[2048]);
  init_memory(&assign2[0], &assign2[2048]);
  init_memory(&assign3[0], &assign3[2048]);
  init_memory(&sa[0], &sa[2048]);
  init_memory(&sb[0], &sb[2048]);
  init_memory(&sc[0], &sc[2048]);
  BENCH("Example10b", example10b(sa,sb,sc,assign1,assign2,assign3), Mi*4/2048*512, digest_memory(&assign1[0], &assign1[2048])+digest_memory(&assign2[0], &assign2[2048])+digest_memory(&assign3[0], &assign3[2048]));
 
  return 0;
}
