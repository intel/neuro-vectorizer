#include "header.h"

int   ia[8192] ALIGNED16;
int in1[512][8192];
int in2[512][8192];

__attribute__((noinline))
void example13(int in1[][8192], int in2[][8192], int *out) {
  int i,j;
  for (i = 0; i < 512; i++) {
    int output = 0;
    for (j = 0; j < 8192; j+=8) {
      output += (in1[i][j] +in2[i][j]);
    }
    out[i] = output;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[8192]);
  init_memory(&in1[0][0], &in1[0][8192]);
  init_memory(&in2[0][0],&in2[0][8192]);
  BENCH("Example13",  example13(in1,in2,ia), 512, digest_memory(&ia[0], &ia[8192]));
  return 0;
}
