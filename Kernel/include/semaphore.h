#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include "MemoryManagerWrapper.h"

typedef struct Semaphore* semPointer;

semPointer sem_open(uint32_t id,int value);

extern uint8_t _xchg(uint8_t * lock,int value);

void deleteProcessFromSem(int64_t pid);

int sem_wait(Semaphore * sem);

int sem_post(Semaphore * sem);

int sem_close(Semaphore * semToClose);

#endif
