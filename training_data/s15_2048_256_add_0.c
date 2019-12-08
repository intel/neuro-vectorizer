#include "header.h"

int   ia[256] ALIGNED16;
int A[2048][256];
int B[2048][256];

__attribute__((noinline))
void example13(int A[][256], int B[][256], int *out) {
  int i,j;
  for (i = 0; i < 2048; i++) {
    int add = 0;
    for (j = 0; j < 256; j+=8) {
      add += (A[i][j] +B[i][j]);
    }
    out[i] = add;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[256]);
  init_memory(&A[0][0], &A[0][256]);
  init_memory(&B[0][0],&B[0][256]);
  BENCH("Example13",  example13(A,B,ia), 4096, digest_memory(&ia[0], &ia[256]));
  return 0;
}
