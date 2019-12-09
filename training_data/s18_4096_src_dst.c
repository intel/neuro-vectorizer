#include "header.h"
unsigned short usa[4096];
unsigned int   ua[4096];

__attribute__((noinline))
void example23 (unsigned short *src, unsigned int *dst)
{
  int i;

  for (i = 0; i < 4096; i++)
    *dst++ = *src++ << 7;
}

int main(int argc,char* argv[]){
  init_memory(&ua[0], &ua[4096]);
      BENCH("Example23",  example23(usa,ua), Mi*8/4096*512, digest_memory(&usa[0], &usa[4096]));
  return 0;
}
