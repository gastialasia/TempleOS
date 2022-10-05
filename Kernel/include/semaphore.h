#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include "MemoryManagerWrapper.h"

typedef struct Semaphore* semPointer;

semPointer sem_open(uint32_t id,int value);

extern uint8_t _xchg(uint8_t * lock,int value);

#endif
