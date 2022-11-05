#include <test_util.h>
#include <stdlib.h>
#include <test_prio.h>

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

int test_prio(int argc, char argv[6][21])
{
  int64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  char aux[6][21];
  strcpy(aux[0], "_loop");

  printf("\nCREATING ");
  printInt(TOTAL_PROCESSES);
  printf(" PROCESSES...\n\n");

  sleep(1500);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = createProcess((uint64_t)endless_loop, 3, 1, aux, NULL, NULL);

  ps();
  putchar('\n');
  sleep(1500);

  printf("CHANGING PRIORITIES...\n\n");

  sleep(1500);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i], prio[i]);

  ps();
  putchar('\n');
  sleep(1500);

  printf("BLOCKING...\n\n");

  sleep(1500);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i]);

  ps();
  putchar('\n');
  sleep(1500);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n\n");

  sleep(1500);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i], MEDIUM);

  ps();
  putchar('\n');
  sleep(1500);

  printf("UNBLOCKING...\n\n");

  sleep(1500);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i]);

  ps();
  putchar('\n');
  sleep(2000);

  // bussy_wait(WAIT);
  printf("\nKILLING...\n\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    kill(pids[i]);

  ps();
  putchar('\n');

  exit();
  return 0;
}
