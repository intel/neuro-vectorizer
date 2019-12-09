#include "header.h"
unsigned short usa[1024];
unsigned int   ua[1024];

__attribute__((noinline))
void example23 (unsigned short *src, unsigned int *res)
{
  int i;

  for (i = 0; i < 1024; i++)
    *res++ = *src++ << 7;
}

int main(int argc,char* argv[]){
  init_memory(&ua[0], &ua[1024]);
      BENCH("Example23",  example23(usa,ua), Mi*8/1024*512, digest_memory(&usa[0], &usa[1024]));
  return 0;
}
