#include "header.h"

int   ia[1024] ALIGNED16;
int A[8192][1024];
int B[8192][1024];

__attribute__((noinline))
void example13(int A[][1024], int B[][1024], int *out) {
  int i,j;
  for (i = 0; i < 8192; i++) {
    int add = 0;
    for (j = 0; j < 1024; j+=8) {
      add += (A[i][j] +B[i][j]);
    }
    out[i] = add;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[1024]);
  init_memory(&A[0][0], &A[0][1024]);
  init_memory(&B[0][0],&B[0][1024]);
  BENCH("Example13",  example13(A,B,ia), 256, digest_memory(&ia[0], &ia[1024]));
  return 0;
}
