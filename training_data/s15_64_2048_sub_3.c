#include "header.h"

int   ia[2048] ALIGNED16;
int A[64][2048];
int B[64][2048];

__attribute__((noinline))
void example13(int A[][2048], int B[][2048], int *out) {
  int i,j;
  for (i = 0; i < 64; i++) {
    int result = 0;
    for (j = 0; j < 2048; j+=8) {
      result += (A[i][j] -B[i][j]);
    }
    out[i] = result;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[2048]);
  init_memory(&A[0][0], &A[0][2048]);
  init_memory(&B[0][0],&B[0][2048]);
  BENCH("Example13",  example13(A,B,ia), 16384, digest_memory(&ia[0], &ia[2048]));
  return 0;
}
