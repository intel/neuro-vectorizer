#include "header.h"
unsigned short usa[128];
unsigned int   ua[128];

__attribute__((noinline))
void example23 (unsigned short *i1, unsigned int *output)
{
  int i;

  for (i = 0; i < 128; i++)
    *output++ = *i1++ << 7;
}

int main(int argc,char* argv[]){
  init_memory(&ua[0], &ua[128]);
      BENCH("Example23",  example23(usa,ua), Mi*8/128*512, digest_memory(&usa[0], &usa[128]));
  return 0;
}
