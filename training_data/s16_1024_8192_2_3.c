#include "header.h"

int   ia[2];
int G[1024][8192];
int G2[1024+2][8192];

__attribute__((noinline))
void example14(int A[][8192], int coeff[][8192], int *out) {
  int k,j,i=0;
  for (k = 0; k < 2; k++) {
    int sum = 0;
    for (i = 0; i < 1024; i++)
        for (j = 0; j < 8192; j++)
          sum += A[i+k][j] * coeff[i][j];

    out[k] = sum;
  }
}

int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[2]);
  init_memory(&G[0][0], &G[0][8192]);
  init_memory(&G2[0][0],&G2[0][8192]);
  BENCH("Example14",  example14(G2,G,ia), 128, digest_memory(&ia[0], &ia[2]));
  return 0;
}