#include "header.h"

int   ia[128] ALIGNED16;
int A[16384][128];
int B[16384][128];

__attribute__((noinline))
void example13(int A[][128], int B[][128], int *out) {
  int i,j;
  for (i = 0; i < 16384; i++) {
    int sub = 0;
    for (j = 0; j < 128; j+=8) {
      sub += (A[i][j] -B[i][j]);
    }
    out[i] = sub;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[128]);
  init_memory(&A[0][0], &A[0][128]);
  init_memory(&B[0][0],&B[0][128]);
  BENCH("Example13",  example13(A,B,ia), 1024, digest_memory(&ia[0], &ia[128]));
  return 0;
}
