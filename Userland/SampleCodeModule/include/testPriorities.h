#ifndef _test_priorities_
#define _test_priorities_

#include <stdint.h>

#define MINOR_WAIT 1000000
#define WAIT 10000000

#define TOTAL_PROCESSES 3
#define LOWEST 2
#define MEDIUM 5
#define HIGHEST 9

void testPriorities(int argc, char argv[ARG_QTY][ARG_LEN]);

#endif