#include "header.h"

short sa[128];
short sb[128];
int   result1[128] ALIGNED16;
int   result2[128] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, short *__restrict__ sb, int* __restrict__ result1, int* __restrict__ result2) {
  int i;
  for (i = 0; i < 128-1; i+=2) {
    result1[i] = (int) sa[i];
result1[i+1] = (int) sa[i+1];
    result2[i] = (int) sb[i];
result2[i+1] = (int) sb[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result1[0], &result1[128]);
  init_memory(&result2[0], &result2[128]);
  init_memory(&sa[0], &sa[128]);
  init_memory(&sb[0], &sb[128]);
  BENCH("Example10b", example10b(sa,sb,result1,result2), Mi*4/128*512, digest_memory(&result1[0], &result1[128])+digest_memory(&result2[0], &result2[128]));
 
  return 0;
}
