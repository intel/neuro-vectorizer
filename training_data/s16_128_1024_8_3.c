#include "header.h"

int   ia[8];
int G[128][1024];
int G2[128+8][1024];

__attribute__((noinline))
void example14(int A[][1024], int coeff[][1024], int *out) {
  int k,j,i=0;
  for (k = 0; k < 8; k++) {
    int sum = 0;
    for (i = 0; i < 128; i++)
        for (j = 0; j < 1024; j++)
          sum += A[i+k][j] * coeff[i][j];

    out[k] = sum;
  }
}

int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[8]);
  init_memory(&G[0][0], &G[0][1024]);
  init_memory(&G2[0][0],&G2[0][1024]);
  BENCH("Example14",  example14(G2,G,ia), 2048, digest_memory(&ia[0], &ia[8]));
  return 0;
}
