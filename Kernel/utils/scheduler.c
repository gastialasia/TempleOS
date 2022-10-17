
#include "../include/scheduler.h"
#include "../include/interrupts.h"


#define QUANTUM 1
#define DEFAULT_PROG_MEM 4096
#define MAX_WAITING_KEYBOARD 15

typedef struct ProcessNode{
  pcb process;
  struct ProcessNode *nextProcess;
}ProcessNode;

typedef struct WaitingNode{
  pcb * process;
  struct WaitingNode * next;
} WaitingNode;

typedef struct WaitingKeyboardList{
  WaitingNode * current;
  WaitingNode * tail;
  uint8_t size;
}WaitingKeyboardList;

typedef struct Scheduler{
  uint64_t quantum;
  uint8_t mutex;
  ProcessNode * current;
  ProcessNode * startList;
}Scheduler;

Scheduler * scheduler;
WaitingKeyboardList * waitKeyboard;
uint8_t firstProcess;// como no tengo que guardar el sp del primer procesos necesito saber si es el primer procesos
ProcessNode * starting; // primer proceso q lo unico que va a ser es un hlt y nada mas
uint64_t currentPid;

static void startingProcess(){
  while (1) {
    _hlt();
  }
}

void initScheduler(){
  scheduler = (Scheduler *) alloc(sizeof(Scheduler));
  scheduler->quantum = QUANTUM -1;
  scheduler->current = NULL;
  scheduler->startList = NULL;
  scheduler->mutex = 0;
  currentPid = 1;
  firstProcess = 1;
  
  //@TODO: Inicialisar la WaitingKeyboardList entera

  uint64_t startingProcessMem = (uint64_t) alloc(2048);

  uint64_t sp = initProcess(startingProcessMem +2048,(uint64_t)&startingProcess,0,NULL);
  
  starting = (ProcessNode *) alloc(sizeof(ProcessNode));
  starting->process.pid = 0;
  starting->process.stackPointer = sp;
  starting->process.processMemory = startingProcessMem;
  starting->process.state = 1; // hacer un enum mejor
  starting->process.priority = 1;
  starting->nextProcess = scheduler->startList;

  waitKeyboard = (WaitingKeyboardList *) alloc(sizeof(WaitingKeyboardList));
  waitKeyboard->current = (WaitingNode *) alloc(sizeof(WaitingNode));
  waitKeyboard->current->process = NULL;
  waitKeyboard->current->next = NULL;
  waitKeyboard->tail = waitKeyboard->current;
  waitKeyboard->size = 0;

  WaitingNode * auxNode = waitKeyboard->current;
  for (int i= 0; i < MAX_WAITING_KEYBOARD; i++) {
    WaitingNode * newNode = (WaitingNode *) alloc(sizeof(WaitingNode));
    newNode->process = NULL;
    newNode->next = NULL;
    auxNode->next = newNode;
    auxNode = auxNode->next;
  }

}
