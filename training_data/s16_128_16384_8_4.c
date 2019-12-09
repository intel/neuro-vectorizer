#include "header.h"

int   ia[8];
int G[128][16384];
int G2[128+8][16384];

__attribute__((noinline))
void example14(int in[][16384], int coeff[][16384], int *out) {
  int k,j,i=0;
  for (k = 0; k < 8; k++) {
    int dot_product = 0;
    for (i = 0; i < 128; i++)
        for (j = 0; j < 16384; j++)
          dot_product += in[i+k][j] * coeff[i][j];

    out[k] = dot_product;
  }
}

int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[8]);
  init_memory(&G[0][0], &G[0][16384]);
  init_memory(&G2[0][0],&G2[0][16384]);
  BENCH("Example14",  example14(G2,G,ia), 128, digest_memory(&ia[0], &ia[8]));
  return 0;
}
