#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "MemoryManagerWrapper.h"
#include "semaphore.h"

typedef struct pcb{
  uint32_t pid;
  uint8_t state;
  uint8_t priority;
  uint64_t stackPointer;
  uint64_t basePointer;
  uint64_t processMemory;
  //falta agregar mas cosas
}pcb;

#endif
