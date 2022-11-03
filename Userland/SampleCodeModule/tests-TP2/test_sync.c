#include "../include/test_sync.h"
#include "../include/stdlib.h"
#include "../include/test_util.h"
#include <programs.h>
#include <stdint.h>
#include "../include/syscallslib.h"
static void incWithSem();

static void incWithoutSem();

static int processWrapper(char * name){
  char argv[6][21];
  argv[0][0] = 'i';
  argv[0][1] = 'n';
  argv[0][2] = 'c';
  argv[0][3] = 0;

  if(*name == 'i'){
    return createProcess(incWithSem,2, 2, argv, NULL, NULL);
  }else
    return createProcess(incWithoutSem,2, 2, argv, NULL, NULL);
}

#define TOTAL_PAIR_PROCESSES 2
#define SEM 50
#define N 100

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  uint32_t pid = getpid();
  int64_t aux = *p;
  aux += inc;
  for (int i = 0; i < 500000*(pid % 5 + 1); i++);
  yield();
  *p = aux;
}

void incWithSem(){
  uint64_t i;
  int64_t value = getpid() % 2 ? 1 : -1;

  Semaphore *sem = semOpen(SEM, 1);

  if (sem == NULL){
    printf("ERROR OPENING SEM\n");
    exit(0);
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
  printf("Final value: ");
  uintToBase(global, resultStr, 10);
  printf(resultStr);
  putchar('\n');
  semPost(sem);
  exit(0);
}

void incWithoutSem(){
  uint64_t i;
  int64_t value = getpid() % 2 ? 1 : -1;

  for (i = 0; i < N; i++) {
    slowInc(&global, value);
  }
  
  char resultStr[20];
  printf("Final value: ");
  uintToBase(global, resultStr, 10);
  printf(resultStr);
  putchar('\n');
  exit(0);
}

int test_sync(int argc, char argv[6][21]){
  uint64_t i;

  global = 0;

  Semaphore *sem = semOpen(SEM, 1);

  printf("CREATING PROCESSES...(WITH SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    processWrapper("i");
    processWrapper("i");
  }

  semClose(sem);
  exit(0);
}

int test_no_sync(int argc, char argv[6][21]){
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITHOUT SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    processWrapper("n");
    processWrapper("n");
  }
  exit(0);
}
