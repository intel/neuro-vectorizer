#include "header.h"

int   ia[1024] ALIGNED16;
int A[2048][1024];
int B[2048][1024];

__attribute__((noinline))
void example13(int A[][1024], int B[][1024], int *out) {
  int i,j;
  for (i = 0; i < 2048; i++) {
    int mul = 0;
    for (j = 0; j < 1024; j+=8) {
      mul += (A[i][j] *B[i][j]);
    }
    out[i] = mul;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[1024]);
  init_memory(&A[0][0], &A[0][1024]);
  init_memory(&B[0][0],&B[0][1024]);
  BENCH("Example13",  example13(A,B,ia), 1024, digest_memory(&ia[0], &ia[1024]));
  return 0;
}
