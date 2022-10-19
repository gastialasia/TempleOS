#include "../include/scheduler.h"
#include "../include/interrupts.h"
#include <stdint.h>


#define QUANTUM 1
#define DEFAULT_PROG_MEM 4096
#define MAX_WAITING_KEYBOARD 15
#define QUANTUM_RAISING_PRIORITY 5

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
  uint32_t quantumForRaisingAuxPriority;
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
  scheduler->quantumForRaisingAuxPriority = QUANTUM_RAISING_PRIORITY;
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
  starting->process.auxPriority = 1;
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


ProcessNode * loadProcessData(ProcessNode * node ,uint32_t pid,uint8_t priority,int argc,char argv[6][21],pipeUserInfo * customStdin,pipeUserInfo * customStdout,uint64_t ip){

  if(node == NULL){
    ProcessNode * newNode = (ProcessNode *) alloc(sizeof(ProcessNode));
    newNode->process.pid = pid;
    newNode->process.priority = priority;
    newNode->process.auxPriority = priority;
    newNode->process.stdin = customStdin;
    newNode->process.stdout = customStdout;
    newNode->process.state = 1;
    //@TODO : un string copy para copiar los argumentos y tmb para parsear lo que te pasa a uno x 6x21
    
    uint64_t processMemory = (uint64_t) alloc(DEFAULT_PROG_MEM);
    uint64_t sp = initProcess(processMemory + DEFAULT_PROG_MEM, ip, argc, newNode->process.args);
    newNode->process.stackPointer = sp;
    newNode->process.basePointer = processMemory + DEFAULT_PROG_MEM;// no se si aca falta un -1
    newNode->process.processMemory = processMemory;
    return newNode;
  }

  if(node->nextProcess != NULL && priority >= node->nextProcess->process.priority){
    node->nextProcess = loadProcessData(node->nextProcess,pid,priority,argc,argv,customStdin,customStdout,ip);
    return node;
  }
  

  //Caso donde la prioridad del proceso a añadir es menor
  ProcessNode * newNode = (ProcessNode *) alloc(sizeof(ProcessNode));

  newNode->nextProcess = node->nextProcess;
  node->nextProcess = newNode;

  newNode->process.pid = pid;
  newNode->process.priority = priority;
  newNode->process.auxPriority = priority;
  newNode->process.stdin = customStdin;
  newNode->process.stdout = customStdout;
  newNode->process.state = 1;
    //@TODO : un string copy para copiar los argumentos y tmb para parsear lo que te pasa a uno x 6x21
    
  uint64_t processMemory = (uint64_t) alloc(DEFAULT_PROG_MEM);
  uint64_t sp = initProcess(processMemory + DEFAULT_PROG_MEM, ip, argc, newNode->process.args);
  newNode->process.stackPointer = sp;
  newNode->process.basePointer = processMemory + DEFAULT_PROG_MEM;// no se si aca falta un -1
  newNode->process.processMemory = processMemory;

  return node;
}


int createProcess(uint64_t ip,uint8_t priority,uint64_t argc,char argv[6][21],pipeUserInfo * customStdin,pipeUserInfo * customStdout){
  
  scheduler->startList = loadProcessData(scheduler->startList, currentPid++,priority,argc, argv, customStdin,customStdout,ip);

  return currentPid-1;
}
//@TODO:
/*
1)Funcion wrapper pára createProcess para que le pasen siempre un char[6][21]
2)Hacer el Context Switching
3)cambiar la syscall de read
*/
uint64_t contextSwitching(uint64_t sp){

  if(firstProcess){
    firstProcess = 0;
    scheduler->current = scheduler->startList;
    return scheduler->startList->process.stackPointer;
  }

  if(scheduler->quantum > 0){
    scheduler->quantum--;
    return 0;
  }
  scheduler->quantum = QUANTUM -1;

  if(scheduler->quantumForRaisingAuxPriority == 0){
    //raise auxpriority y resetear la var del scheduler
  }
  
  scheduler->quantumForRaisingAuxPriority--;

  scheduler->current->process.stackPointer = sp;

  //aca decidir cual es el prox process en correr


}

