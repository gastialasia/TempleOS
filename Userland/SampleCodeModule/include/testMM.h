#ifndef _testmm_
#define _testmm_

#include <stdint.h>

typedef struct MM_rq
{
  void *address;
  uint32_t size;
} mm_rq;

void testMM(int argc, char argv[ARG_QTY][ARG_LEN]);

#endif
