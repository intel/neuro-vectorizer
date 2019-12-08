#include "header.h"

int   ia[128] ALIGNED16;
int in1[8192][128];
int in2[8192][128];

__attribute__((noinline))
void example13(int in1[][128], int in2[][128], int *out) {
  int i,j;
  for (i = 0; i < 8192; i++) {
    int output = 0;
    for (j = 0; j < 128; j+=8) {
      output += (in1[i][j] -in2[i][j]);
    }
    out[i] = output;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&in1[0][0], &in1[0][128]);
  init_memory(&in2[0][0],&in2[0][128]);
  BENCH("Example13",  example13(in1,in2,ia), 2048, digest_memory(&ia[0], &ia[128]));
  return 0;
}
