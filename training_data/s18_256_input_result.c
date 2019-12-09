#include "header.h"
unsigned short usa[256];
unsigned int   ua[256];

__attribute__((noinline))
void example23 (unsigned short *input, unsigned int *result)
{
  int i;

  for (i = 0; i < 256; i++)
    *result++ = *input++ << 7;
}

int main(int argc,char* argv[]){
  init_memory(&ua[0], &ua[256]);
      BENCH("Example23",  example23(usa,ua), Mi*8/256*512, digest_memory(&usa[0], &usa[256]));
  return 0;
}
