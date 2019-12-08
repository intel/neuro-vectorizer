#include "header.h"

int   ia[512] ALIGNED16;
int op1[128][512];
int op2[128][512];

__attribute__((noinline))
void example13(int op1[][512], int op2[][512], int *out) {
  int i,j;
  for (i = 0; i < 128; i++) {
    int sum_mul = 0;
    for (j = 0; j < 512; j+=8) {
      sum_mul += (op1[i][j] *op2[i][j]);
    }
    out[i] = sum_mul;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[512]);
  init_memory(&op1[0][0], &op1[0][512]);
  init_memory(&op2[0][0],&op2[0][512]);
  BENCH("Example13",  example13(op1,op2,ia), 32768, digest_memory(&ia[0], &ia[512]));
  return 0;
}
