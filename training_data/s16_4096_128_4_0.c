#include "header.h"

int   ia[4];
int G[4096][128];
int G2[4096+4][128];

__attribute__((noinline))
void example14(int in[][128], int coeff[][128], int *out) {
  int k,j,i=0;
  for (k = 0; k < 4; k++) {
    int sum = 0;
    for (i = 0; i < 4096; i++)
        for (j = 0; j < 128; j++)
          sum += in[i+k][j] * coeff[i][j];

    out[k] = sum;
  }
}

int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[4]);
  init_memory(&G[0][0], &G[0][128]);
  init_memory(&G2[0][0],&G2[0][128]);
  BENCH("Example14",  example14(G2,G,ia), 1024, digest_memory(&ia[0], &ia[4]));
  return 0;
}
