#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <MemoryManagerWrapper.h>
#include <semaphore.h>
#include <lib.h>

#define ARG_LEN 21
#define ARG_QTY 6

typedef struct fd{
  char readable;
  char writable;
  struct pipe * pipe;
}fd;


typedef struct pcb{
  char args[ARG_QTY][ARG_LEN];
  uint32_t pid;
  uint8_t state;
  uint8_t priority;
  uint8_t auxPriority;
  uint64_t stackPointer;
  uint64_t basePointer;
  uint64_t processMemory;
  fd * stdin;
  fd * stdout;
}pcb;

void initScheduler(); 
int createProcess(uint64_t ip,uint8_t priority, uint64_t argc, char argv[ARG_QTY][ARG_LEN], fd *customStdin, fd *customStdout);
void awakeKeyboardList();
void addToKeyboardList();
void exitCurrentProcess();
pcb * getCurrentProcess();
int getCurrentPID();
fd * getCurrentStdin();
fd * getCurrentStdout();
pcb * blockCurrentProcess();
void getAllProcesses(char * buf);
int changeProcessState(uint32_t pid);
int killPid(uint32_t pid);
int changeProcessPriority(uint32_t pid, uint8_t newPriority);

#endif 
