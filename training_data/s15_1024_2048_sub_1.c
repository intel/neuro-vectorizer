#include "header.h"

int   ia[2048] ALIGNED16;
int in1[1024][2048];
int in2[1024][2048];

__attribute__((noinline))
void example13(int in1[][2048], int in2[][2048], int *out) {
  int i,j;
  for (i = 0; i < 1024; i++) {
    int res = 0;
    for (j = 0; j < 2048; j+=8) {
      res += (in1[i][j] -in2[i][j]);
    }
    out[i] = res;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[2048]);
  init_memory(&in1[0][0], &in1[0][2048]);
  init_memory(&in2[0][0],&in2[0][2048]);
  BENCH("Example13",  example13(in1,in2,ia), 1024, digest_memory(&ia[0], &ia[2048]));
  return 0;
}
