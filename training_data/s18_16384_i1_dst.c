#include "header.h"
unsigned short usa[16384];
unsigned int   ua[16384];

__attribute__((noinline))
void example23 (unsigned short *i1, unsigned int *dst)
{
  int i;

  for (i = 0; i < 16384; i++)
    *dst++ = *i1++ << 7;
}

int main(int argc,char* argv[]){
  init_memory(&ua[0], &ua[16384]);
      BENCH("Example23",  example23(usa,ua), Mi*8/16384*512, digest_memory(&usa[0], &usa[16384]));
  return 0;
}
