#include <stdlib.h>
#include <testPriorities.h>
#include <testUtil.h>

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void testPriorities(int argc, char argv[ARG_QTY][ARG_LEN]) {
  int64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  char aux[6][21];
  strcpy(aux[0], "_loop");

  printf("\nCREATING ");
  printInt(TOTAL_PROCESSES);
  printf(" PROCESSES...\n\n");

  sleep(1500);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = createProcess((uint64_t)endlessLoop, 3, 1, aux, NULL, NULL);

  ps();
  putchar('\n');
  sleep(1500);

  printf("CHANGING PRIORITIES...\n\n");

  sleep(1500);

  for (i = 0; i < TOTAL_PROCESSES; i++) nice(pids[i], prio[i]);

  ps();
  putchar('\n');
  sleep(1500);

  printf("BLOCKING...\n\n");

  sleep(1500);

  for (i = 0; i < TOTAL_PROCESSES; i++) block(pids[i]);

  ps();
  putchar('\n');
  sleep(1500);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n\n");

  sleep(1500);

  for (i = 0; i < TOTAL_PROCESSES; i++) nice(pids[i], MEDIUM);

  ps();
  putchar('\n');
  sleep(1500);

  printf("UNBLOCKING...\n\n");

  sleep(1500);

  for (i = 0; i < TOTAL_PROCESSES; i++) block(pids[i]);

  ps();
  putchar('\n');
  sleep(2000);

  // bussyWait(WAIT);
  printf("\nKILLING...\n\n");

  for (i = 0; i < TOTAL_PROCESSES; i++) kill(pids[i]);

  ps();
  putchar('\n');

  exit();
}
