#include "header.h"

int   ia[2];
int G[16384][128];
int G2[16384+2][128];

__attribute__((noinline))
void example14(int in[][128], int coeff[][128], int *result) {
  int k,j,i=0;
  for (k = 0; k < 2; k++) {
    int sum = 0;
    for (i = 0; i < 16384; i++)
        for (j = 0; j < 128; j++)
          sum += in[i+k][j] * coeff[i][j];

    result[k] = sum;
  }
}

int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[2]);
  init_memory(&G[0][0], &G[0][128]);
  init_memory(&G2[0][0],&G2[0][128]);
  BENCH("Example14",  example14(G2,G,ia), 512, digest_memory(&ia[0], &ia[2]));
  return 0;
}
