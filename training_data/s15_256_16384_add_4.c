#include "header.h"

int   ia[16384] ALIGNED16;
int op1[256][16384];
int op2[256][16384];

__attribute__((noinline))
void example13(int op1[][16384], int op2[][16384], int *out) {
  int i,j;
  for (i = 0; i < 256; i++) {
    int sum_add = 0;
    for (j = 0; j < 16384; j+=8) {
      sum_add += (op1[i][j] +op2[i][j]);
    }
    out[i] = sum_add;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[16384]);
  init_memory(&op1[0][0], &op1[0][16384]);
  init_memory(&op2[0][0],&op2[0][16384]);
  BENCH("Example13",  example13(op1,op2,ia), 512, digest_memory(&ia[0], &ia[16384]));
  return 0;
}
