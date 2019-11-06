#include <sys/time.h>
#include <stdio.h>
#include <stdbool.h>

#define N 512
#define M 32
#define K 4
#define L 32
#define ALIGNED16 __attribute__((aligned(16)))

typedef int aint __attribute__ ((__aligned__(16)));

// Warmup and then measure.
#define BENCH(NAME, RUN_LINE, ITER, DIGEST_LINE) {\
  struct timeval Start, End;\
  RUN_LINE;\
  gettimeofday(&Start, 0);for (int i=0; i < (ITER); ++i) RUN_LINE;\
  gettimeofday(&End, 0);\
  unsigned r = DIGEST_LINE;\
  long mtime, s,us;\
  s = End.tv_sec  - Start.tv_sec;\
  us = End.tv_usec - Start.tv_usec;\
  mtime = (s*1000 + us/1000.0)+0.5;\
  if (print_times)\
    printf("%ld",mtime);\
}
#define Mi 1048576 //1<<20
#define print_times true//argc > 1;
void init_memory(void *start, void* end);
void init_memory_float(float *start, float* end);
unsigned digest_memory(void *start, void* end);
void atimer(const char* title, bool print);
