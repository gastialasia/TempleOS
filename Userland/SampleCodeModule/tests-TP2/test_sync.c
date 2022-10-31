#include "../include/test_sync.h"
#include "../include/stdlib.h"
#include "../include/test_util.h"
#include <programs.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  uint64_t aux = *p;
  yield(); //This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char argv[6][21]){
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 4) exit();

  if ((n = satoi(argv[0])) <= 0) exit();
  if ((inc = satoi(argv[1])) == 0) exit();
  if ((use_sem = satoi(argv[2])) < 0) exit();

  if (use_sem)
    if (!semOpen(SEM_ID, 1)){
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++){
    if (use_sem) semWait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem) semPost(SEM_ID);
  }

  if (use_sem) semClose(SEM_ID);
  
  exit();
}

uint64_t test_sync(uint64_t argc, const char argv[6][21]){ //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3) exit();

  char * argvDec[] = {argv[1], "-1", argv[2], NULL};
  char * argvInc[] = {argv[1], "1", argv[2], NULL};

  global = 0;

  uint64_t i;
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    pids[i] = createProcess(my_process_inc, 3, 3, argvDec, NULL, NULL);
    pids[i + TOTAL_PAIR_PROCESSES] = createProcess(slowInc, 3, 3, argvInc, NULL, NULL);
  }

  // for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
  //   my_wait(pids[i]);
  //   my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  // }

  printf("Final value: ");
  printf(global);
  putchar('\n');

  exit();
}
