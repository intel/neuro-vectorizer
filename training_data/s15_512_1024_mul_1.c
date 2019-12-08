#include "header.h"

int   ia[1024] ALIGNED16;
int in1[512][1024];
int in2[512][1024];

__attribute__((noinline))
void example13(int in1[][1024], int in2[][1024], int *out) {
  int i,j;
  for (i = 0; i < 512; i++) {
    int res = 0;
    for (j = 0; j < 1024; j+=8) {
      res += (in1[i][j] *in2[i][j]);
    }
    out[i] = res;
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[1024]);
  init_memory(&in1[0][0], &in1[0][1024]);
  init_memory(&in2[0][0],&in2[0][1024]);
  BENCH("Example13",  example13(in1,in2,ia), 4096, digest_memory(&ia[0], &ia[1024]));
  return 0;
}
