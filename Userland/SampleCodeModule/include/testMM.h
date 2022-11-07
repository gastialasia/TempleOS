#ifndef _test_mm_
#define _test_mm_

#include <stdint.h>

typedef struct MM_rq
{
  void *address;
  uint32_t size;
} mm_rq;

int testMM(int argc, char argv[6][21]);

#endif
