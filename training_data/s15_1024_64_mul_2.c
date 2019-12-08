#include "header.h"

int   ia[64] ALIGNED16;
int in1[1024][64];
int in2[1024][64];

__attribute__((noinline))
void example13(int in1[][64], int in2[][64], int *out) {
  int i,j;
  for (i = 0; i < 1024; i++) {
    int output = 0;
    for (j = 0; j < 64; j+=8) {
      output += (in1[i][j] *in2[i][j]);
    }
    out[i] = output;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[64]);
  init_memory(&in1[0][0], &in1[0][64]);
  init_memory(&in2[0][0],&in2[0][64]);
  BENCH("Example13",  example13(in1,in2,ia), 32768, digest_memory(&ia[0], &ia[64]));
  return 0;
}
