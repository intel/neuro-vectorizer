#include "header.h"

int   ia[4];
int G[8192][256];
int G2[8192+4][256];

__attribute__((noinline))
void example14(int mat1[][256], int mat2[][256], int *out) {
  int k,j,i=0;
  for (k = 0; k < 4; k++) {
    int sum = 0;
    for (i = 0; i < 8192; i++)
        for (j = 0; j < 256; j++)
          sum += mat1[i+k][j] * mat2[i][j];

    out[k] = sum;
  }
}

int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4]);
  init_memory(&G[0][0], &G[0][256]);
  init_memory(&G2[0][0],&G2[0][256]);
  BENCH("Example14",  example14(G2,G,ia), 256, digest_memory(&ia[0], &ia[4]));
  return 0;
}
