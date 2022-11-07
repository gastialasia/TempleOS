#ifndef _test_processes_
#define _test_processes_

#include <stdint.h>

enum State { RUNNING, BLOCKED, KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

void testProcesses(int argc, char argv[ARG_QTY][ARG_LEN]);

#endif