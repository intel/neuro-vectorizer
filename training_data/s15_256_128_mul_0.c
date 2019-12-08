#include "header.h"

int   ia[128] ALIGNED16;
int A[256][128];
int B[256][128];

__attribute__((noinline))
void example13(int A[][128], int B[][128], int *out) {
  int i,j;
  for (i = 0; i < 256; i++) {
    int mul = 0;
    for (j = 0; j < 128; j+=8) {
      mul += (A[i][j] *B[i][j]);
    }
    out[i] = mul;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&A[0][0], &A[0][128]);
  init_memory(&B[0][0],&B[0][128]);
  BENCH("Example13",  example13(A,B,ia), 65536, digest_memory(&ia[0], &ia[128]));
  return 0;
}
