#include "header.h"

int   ia[4096] ALIGNED16;
int in1[256][4096];
int in2[256][4096];

__attribute__((noinline))
void example13(int in1[][4096], int in2[][4096], int *out) {
  int i,j;
  for (i = 0; i < 256; i++) {
    int output = 0;
    for (j = 0; j < 4096; j+=8) {
      output += (in1[i][j] *in2[i][j]);
    }
    out[i] = output;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4096]);
  init_memory(&in1[0][0], &in1[0][4096]);
  init_memory(&in2[0][0],&in2[0][4096]);
  BENCH("Example13",  example13(in1,in2,ia), 2048, digest_memory(&ia[0], &ia[4096]));
  return 0;
}
