#include <scheduler.h>
#include <semaphore.h>
#include <tools.h>

#define NULL ((void *)0)
#define MAX_SEM 15
#define MAX_PROCESS 15

#define ID 0
#define VALUE 1
#define WAITING_PIDS 2

typedef struct Semaphore {
  uint32_t id;
  int value;
  pcb *queuqe[MAX_PROCESS];  // struct de procesos
  uint32_t waiting;
  uint8_t lock;
} Semaphore;

Semaphore *sems[MAX_SEM];
uint32_t size = 0;
uint8_t mutexSem = 0;

void normalizeTable(char *buf, char *data, int field);

Semaphore *semOpen(uint32_t id, int value) {
  while (_xchg(&mutexSem, 1) != 0)
    ;

  if (value < 0 || size == MAX_SEM) {
    return NULL;
  }

  int semIter = 0;
  while (semIter < size && sems[semIter]->id != id) {
    semIter++;
  }

  if (semIter == size) {
    sems[semIter] = (Semaphore *)alloc(sizeof(Semaphore));
    sems[semIter]->id = id;
    sems[semIter]->value = value;
    sems[semIter]->waiting = 0;
    sems[semIter]->lock = 0;
    _xchg(&mutexSem, 0);
    return sems[size++];
  }

  _xchg(&mutexSem, 0);
  return sems[semIter];
}

int semClose(Semaphore *semToClose) {
  if (semToClose->waiting > 0) {
    return -1;
  }

  while (_xchg(&mutexSem, 1) != 0)
    ;

  int semIter = 0;
  while (semIter < size && sems[semIter]->id != semToClose->id) {
    semIter++;
  }

  if (semIter == size) {
    _xchg(&mutexSem, 0);
    return -1;
  }
  while (semIter < size - 1) {
    sems[semIter] = sems[semIter + 1];
    semIter++;
  }

  size--;
  free(semToClose);

  _xchg(&mutexSem, 0);
  return 1;
}

int semPost(Semaphore *sem) {
  while (_xchg(&sem->lock, 1) != 0)
    ;

  sem->value++;

  if (sem->waiting == 0) {
    _xchg(&sem->lock, 0);
    return 0;
  }

  sem->queuqe[0]->state = 1;
  sem->waiting--;

  for (int i = 0; i < sem->waiting; i++) {
    sem->queuqe[i] = sem->queuqe[i + 1];
  }
  _xchg(&sem->lock, 0);
  return 0;
}

int semWait(Semaphore *sem) {
  while (_xchg(&sem->lock, 1) != 0)
    ;

  while (sem->value == 0) {
    if (sem->waiting == MAX_PROCESS) {
      return -1;
    }

    sem->queuqe[sem->waiting++] = blockCurrentProcess();

    _xchg(&sem->lock, 0);

    runScheduler();
  }

  sem->value--;

  _xchg(&sem->lock, 0);

  return 0;
}

void deleteProcessFromSem(int64_t pid) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < sems[i]->waiting; j++) {
      if (sems[i]->queuqe[j]->pid == pid) {
        sems[i]->waiting--;

        for (int k = j; k < sems[i]->waiting; k++) {
          sems[i]->queuqe[k] = sems[i]->queuqe[k + 1];
        }
        return;
      }
    }
  }
}

void normalizeTable(char *buf, char *data, int field) {
  static int fields[] = {6, 5, 65};
  int n = fields[field] - strlen(data);
  strcat(buf, data);
  for (int i = 0; i < n; i++) {
    strcat(buf, " ");
  }
  strcat(buf, "  ");  // Separate fields
}

void getAllSems(char *buffer) {
  strcat(buffer, "SEM_ID  Value  Blocked PIDs\n");
  char idStr[6];
  char valueStr[5];
  int j;
  for (int i = 0; i < size; i++) {
    uintToBase(sems[i]->id, idStr, 10);
    normalizeTable(buffer, idStr, ID);

    uintToBase(sems[i]->value, valueStr, 10);
    normalizeTable(buffer, valueStr, VALUE);

    for (j = 0; j < sems[i]->waiting; j++) {
      uintToBase(sems[i]->queuqe[j]->pid, idStr, 10);
      strcat(buffer, idStr);
      if (j != sems[i]->waiting - 1) {
        strcat(buffer, " - ");
      }
    }
    strcat(buffer, "\n");
  }
}
