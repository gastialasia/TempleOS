#ifndef _test_prio_
#define _test_prio_

#include <stdint.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT      10000000 // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 2 //TODO: Change as required
#define MEDIUM 5 //TODO: Change as required
#define HIGHEST 9 //TODO: Change as required

int test_prio(int argc, char argv[6][21]);

#endif