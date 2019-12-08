#include "header.h"

int   ia[128] ALIGNED16;
int op1[256][128];
int op2[256][128];

__attribute__((noinline))
void example13(int op1[][128], int op2[][128], int *out) {
  int i,j;
  for (i = 0; i < 256; i++) {
    int sum_add = 0;
    for (j = 0; j < 128; j+=8) {
      sum_add += (op1[i][j] +op2[i][j]);
    }
    out[i] = sum_add;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&op1[0][0], &op1[0][128]);
  init_memory(&op2[0][0],&op2[0][128]);
  BENCH("Example13",  example13(op1,op2,ia), 65536, digest_memory(&ia[0], &ia[128]));
  return 0;
}
