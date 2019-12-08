#include "header.h"

int   ia[8192] ALIGNED16;
int op1[128][8192];
int op2[128][8192];

__attribute__((noinline))
void example13(int op1[][8192], int op2[][8192], int *out) {
  int i,j;
  for (i = 0; i < 128; i++) {
    int sum_add = 0;
    for (j = 0; j < 8192; j+=8) {
      sum_add += (op1[i][j] +op2[i][j]);
    }
    out[i] = sum_add;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[8192]);
  init_memory(&op1[0][0], &op1[0][8192]);
  init_memory(&op2[0][0],&op2[0][8192]);
  BENCH("Example13",  example13(op1,op2,ia), 2048, digest_memory(&ia[0], &ia[8192]));
  return 0;
}
