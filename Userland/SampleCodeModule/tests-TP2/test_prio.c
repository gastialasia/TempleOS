#include "../include/test_util.h"
#include "../include/stdlib.h"
#include "../include/test_prio.h"

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio(){
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {0};
  uint64_t i;

  char aux[6][21];
  strcpy(aux[0], "_loop");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = createProcess(endless_loop, 3, 1, aux, NULL, NULL);

  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i], prio[i]);

  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i], MEDIUM);

  printf("UNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i]);

  bussy_wait(WAIT);
  printf("\nKILLING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    kill(pids[i]);

  exit();
}
