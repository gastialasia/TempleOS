#include <programs.h>
#include <stdint.h>
#include <stdlib.h>
#include <syscallslib.h>
#include <testSync.h>
#include <testUtil.h>
static void incWithSem(int argc, char argv[6][21]);
static void incWithoutSem(int argc, char argv[6][21]);

static int processWrapper(char *name) {
  char argv[6][21];
  argv[0][0] = 'i';
  argv[0][1] = 'n';
  argv[0][2] = 'c';
  argv[0][3] = 0;

  if (*name == 'i') {
    return createProcess((uint64_t)incWithSem, 2, 2, argv, NULL, NULL);
  } else
    return createProcess((uint64_t)incWithoutSem, 2, 2, argv, NULL, NULL);
}

#define TOTAL_PAIR_PROCESSES 2
#define SEM 50
#define N 100

int64_t global;  // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint32_t pid = getpid();
  int64_t aux = *p;
  aux += inc;
  for (int i = 0; i < 500000 * (pid % 5 + 1); i++)
    ;
  yield();
  *p = aux;
}

void incWithSem(int argc, char argv[6][21]) {
  uint64_t i;
  int64_t value = (getpid() % 2) ? 1 : -1;

  Semaphore *sem = semOpen(SEM, 1);

  if (sem == NULL) {
    printf("ERROR OPENING SEM\n");
    exit();
    return;
  }

  for (i = 0; i < N; i++) {
    semWait(sem);
    slowInc(&global, value);
    semPost(sem);
  }
  semWait(sem);
  printf("soy el inc:");
  printInt(getpid());
  putchar('\n');

  char resultStr[20];
  printf("value: ");
  itos(global, resultStr);
  printf(resultStr);
  putchar('\n');
  semPost(sem);
  semClose(sem);
  exit();
}

void incWithoutSem(int argc, char argv[6][21]) {
  uint64_t i;
  int64_t value = (getpid() % 2) ? 1 : -1;

  for (i = 0; i < N; i++) {
    slowInc(&global, value);
  }

  char resultStr[20];
  printf("value: ");
  itos(global, resultStr);
  printf(resultStr);
  putchar('\n');
  exit();
}

void testSync(int argc, char argv[6][21]) {
  global = 0;

  uint64_t i;

  Semaphore *sem = semOpen(SEM, 1);
  printf("After ");
  printInt(TOTAL_PAIR_PROCESSES * 2);
  printf(" prints final value should be 0\n");

  printf("CREATING PROCESSES (WITH SEM)... Please wait\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    processWrapper("i");
    processWrapper("i");
  }

  semClose(sem);
  exit();
}

void testNoSync(int argc, char argv[6][21]) {
  printf("After ");
  printInt(TOTAL_PAIR_PROCESSES * 2);
  printf(" prints the final value should be different than 0\n");

  global = 0;

  uint64_t i;

  printf("CREATING PROCESSES (WITHOUT SEM)... Please wait\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    processWrapper("n");
    processWrapper("n");
  }
  exit();
}
