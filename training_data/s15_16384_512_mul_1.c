#include "header.h"

int   ia[512] ALIGNED16;
int in1[16384][512];
int in2[16384][512];

__attribute__((noinline))
void example13(int in1[][512], int in2[][512], int *out) {
  int i,j;
  for (i = 0; i < 16384; i++) {
    int res = 0;
    for (j = 0; j < 512; j+=8) {
      res += (in1[i][j] *in2[i][j]);
    }
    out[i] = res;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[512]);
  init_memory(&in1[0][0], &in1[0][512]);
  init_memory(&in2[0][0],&in2[0][512]);
  BENCH("Example13",  example13(in1,in2,ia), 256, digest_memory(&ia[0], &ia[512]));
  return 0;
}
