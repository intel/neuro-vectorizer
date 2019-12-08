#include "header.h"

int   ia[512] ALIGNED16;
int A[512][512];
int B[512][512];

__attribute__((noinline))
void example13(int A[][512], int B[][512], int *out) {
  int i,j;
  for (i = 0; i < 512; i++) {
    int result = 0;
    for (j = 0; j < 512; j+=8) {
      result += (A[i][j] -B[i][j]);
    }
    out[i] = result;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[512]);
  init_memory(&A[0][0], &A[0][512]);
  init_memory(&B[0][0],&B[0][512]);
  BENCH("Example13",  example13(A,B,ia), 8192, digest_memory(&ia[0], &ia[512]));
  return 0;
}
