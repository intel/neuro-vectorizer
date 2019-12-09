#include "header.h"
unsigned short usa[8192];
unsigned int   ua[8192];

__attribute__((noinline))
void example23 (unsigned short *a, unsigned int *output)
{
  int i;

  for (i = 0; i < 8192; i++)
    *output++ = *a++ << 7;
}

int main(int argc,char* argv[]){
  init_memory(&ua[0], &ua[8192]);
      BENCH("Example23",  example23(usa,ua), Mi*8/8192*512, digest_memory(&usa[0], &usa[8192]));
  return 0;
}
