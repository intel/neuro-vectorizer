#include "header.h"

int   ia[4];
int G[64][1024];
int G2[64+4][1024];

__attribute__((noinline))
void example14(int in[][1024], int coeff[][1024], int *out) {
  int k,j,i=0;
  for (k = 0; k < 4; k++) {
    int dot_product = 0;
    for (i = 0; i < 64; i++)
        for (j = 0; j < 1024; j++)
          dot_product += in[i+k][j] * coeff[i][j];

    out[k] = dot_product;
  }
}

int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4]);
  init_memory(&G[0][0], &G[0][1024]);
  init_memory(&G2[0][0],&G2[0][1024]);
  BENCH("Example14",  example14(G2,G,ia), 8192, digest_memory(&ia[0], &ia[4]));
  return 0;
}
