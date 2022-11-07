#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include <MemoryManagerWrapper.h>

typedef struct Semaphore* semPointer;

semPointer semOpen(uint32_t id,int value);

extern uint8_t _xchg(uint8_t * lock,int value);

void deleteProcessFromSem(int64_t pid);

int semWait(semPointer sem);

int semPost(semPointer sem);

int semClose(semPointer semToClose);

void getAllSems(char *buffer);

#endif
