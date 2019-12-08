#include "header.h"

int   ia[512] ALIGNED16;
int A[4096][512];
int B[4096][512];

__attribute__((noinline))
void example13(int A[][512], int B[][512], int *out) {
  int i,j;
  for (i = 0; i < 4096; i++) {
    int mul = 0;
    for (j = 0; j < 512; j+=8) {
      mul += (A[i][j] *B[i][j]);
    }
    out[i] = mul;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[512]);
  init_memory(&A[0][0], &A[0][512]);
  init_memory(&B[0][0],&B[0][512]);
  BENCH("Example13",  example13(A,B,ia), 1024, digest_memory(&ia[0], &ia[512]));
  return 0;
}
