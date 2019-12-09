#include "header.h"
unsigned short usa[2048];
unsigned int   ua[2048];

__attribute__((noinline))
void example23 (unsigned short *a, unsigned int *result)
{
  int i;

  for (i = 0; i < 2048; i++)
    *result++ = *a++ << 7;
}

int main(int argc,char* argv[]){
  init_memory(&ua[0], &ua[2048]);
      BENCH("Example23",  example23(usa,ua), Mi*8/2048*512, digest_memory(&usa[0], &usa[2048]));
  return 0;
}
