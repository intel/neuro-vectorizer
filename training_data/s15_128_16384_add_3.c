#include "header.h"

int   ia[16384] ALIGNED16;
int A[128][16384];
int B[128][16384];

__attribute__((noinline))
void example13(int A[][16384], int B[][16384], int *out) {
  int i,j;
  for (i = 0; i < 128; i++) {
    int result = 0;
    for (j = 0; j < 16384; j+=8) {
      result += (A[i][j] +B[i][j]);
    }
    out[i] = result;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[16384]);
  init_memory(&A[0][0], &A[0][16384]);
  init_memory(&B[0][0],&B[0][16384]);
  BENCH("Example13",  example13(A,B,ia), 1024, digest_memory(&ia[0], &ia[16384]));
  return 0;
}
