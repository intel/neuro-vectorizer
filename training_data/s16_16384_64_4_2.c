#include "header.h"

int   ia[4];
int G[16384][64];
int G2[16384+4][64];

__attribute__((noinline))
void example14(int in[][64], int coeff[][64], int *result) {
  int k,j,i=0;
  for (k = 0; k < 4; k++) {
    int sum = 0;
    for (i = 0; i < 16384; i++)
        for (j = 0; j < 64; j++)
          sum += in[i+k][j] * coeff[i][j];

    result[k] = sum;
  }
}

int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4]);
  init_memory(&G[0][0], &G[0][64]);
  init_memory(&G2[0][0],&G2[0][64]);
  BENCH("Example14",  example14(G2,G,ia), 512, digest_memory(&ia[0], &ia[4]));
  return 0;
}
