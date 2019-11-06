#include "header.h"

short sa[256];
short sb[256];
short sc[256];
int   assign1[256] ALIGNED16;
int   assign2[256] ALIGNED16;
int   assign3[256] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ assign1, int* __restrict__ assign2, int* __restrict__ assign3) {
  int i;
  for (i = 0; i < 256-1; i+=2) {
    assign1[i] = (int) sa[i];
assign1[i+1] = (int) sa[i+1];
    assign2[i] = (int) sb[i];
assign2[i+1] = (int) sb[i+1];
    assign3[i] = (int) sc[i];
assign3[i+1] = (int) sc[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&assign1[0], &assign1[256]);
  init_memory(&assign2[0], &assign2[256]);
  init_memory(&assign3[0], &assign3[256]);
  init_memory(&sa[0], &sa[256]);
  init_memory(&sb[0], &sb[256]);
  init_memory(&sc[0], &sc[256]);
  BENCH("Example10b", example10b(sa,sb,sc,assign1,assign2,assign3), Mi*4/256*512, digest_memory(&assign1[0], &assign1[256])+digest_memory(&assign2[0], &assign2[256])+digest_memory(&assign3[0], &assign3[256]));
 
  return 0;
}
