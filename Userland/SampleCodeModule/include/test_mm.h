#ifndef _test_mm_
#define _test_mm_

#include <stdint.h>

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

char test_mm(); //test_mm(uint64_t argc, char *argv[])

#endif
