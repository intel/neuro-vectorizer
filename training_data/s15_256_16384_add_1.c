#include "header.h"

int   ia[16384] ALIGNED16;
int in1[256][16384];
int in2[256][16384];

__attribute__((noinline))
void example13(int in1[][16384], int in2[][16384], int *out) {
  int i,j;
  for (i = 0; i < 256; i++) {
    int res = 0;
    for (j = 0; j < 16384; j+=8) {
      res += (in1[i][j] +in2[i][j]);
    }
    out[i] = res;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[16384]);
  init_memory(&in1[0][0], &in1[0][16384]);
  init_memory(&in2[0][0],&in2[0][16384]);
  BENCH("Example13",  example13(in1,in2,ia), 512, digest_memory(&ia[0], &ia[16384]));
  return 0;
}
