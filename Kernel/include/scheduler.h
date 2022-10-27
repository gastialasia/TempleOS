#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "MemoryManagerWrapper.h"
#include "semaphore.h"
#include "lib.h"

typedef struct pipeUserInfo{
  char readable;
  char writable;
  struct pipe * pipe;
}pipeUserInfo;


typedef struct pcb{
  char args[6][21];
  uint32_t pid;
  uint8_t state;
  uint8_t priority;
  uint8_t auxPriority;
  uint64_t stackPointer;
  uint64_t basePointer;
  uint64_t processMemory;
  pipeUserInfo * stdin;
  pipeUserInfo * stdout;
}pcb;

void initScheduler(); 
int createProcess(uint64_t ip,uint8_t priority, uint64_t argc, char argv[6][21], pipeUserInfo *customStdin, pipeUserInfo *customStdout);
int createProcessFormatter(uint64_t ip, uint8_t priority, uint64_t argc, char *argv, pipeUserInfo *customStdin, pipeUserInfo *customStdout);
void awakeKeyboardList();
void addToKeyboardList();
pcb * getCurrentProcess();
pipeUserInfo * getCurrentStdin();
pipeUserInfo * getCurrentStdout();
pcb * blockCurrentProcess();

#endif 
