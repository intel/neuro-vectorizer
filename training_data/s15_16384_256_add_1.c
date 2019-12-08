#include "header.h"

int   ia[256] ALIGNED16;
int in1[16384][256];
int in2[16384][256];

__attribute__((noinline))
void example13(int in1[][256], int in2[][256], int *out) {
  int i,j;
  for (i = 0; i < 16384; i++) {
    int res = 0;
    for (j = 0; j < 256; j+=8) {
      res += (in1[i][j] +in2[i][j]);
    }
    out[i] = res;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[256]);
  init_memory(&in1[0][0], &in1[0][256]);
  init_memory(&in2[0][0],&in2[0][256]);
  BENCH("Example13",  example13(in1,in2,ia), 512, digest_memory(&ia[0], &ia[256]));
  return 0;
}
