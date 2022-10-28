#include "../include/scheduler.h"
#include "../include/interrupts.h"
#include <stdint.h>
#include <stdlib.h>


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
  uint8_t foregroundInUse;
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
  scheduler->foregroundInUse = 0;
  currentPid = 1;
  firstProcess = 1;
  
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


}
static char* copyString(char* destination, const char* source) {
  if (destination == NULL)
    return NULL;

  char *ptr = destination;

  while (*source != '\0') {
    *destination = *source;
    destination++;
    source++;
  }

  *destination = '\0';
  return ptr;
}

static ProcessNode * loadProcessData(ProcessNode * node ,uint32_t pid,uint8_t priority,int argc,char argv[6][21],pipeUserInfo * customStdin,pipeUserInfo * customStdout,uint64_t ip){

  if(node == NULL){
    ProcessNode * newNode = (ProcessNode *) alloc(sizeof(ProcessNode));
    newNode->process.pid = pid;
    newNode->process.priority = priority;
    newNode->process.auxPriority = priority;
    newNode->process.stdin = customStdin;
    newNode->process.stdout = customStdout;
    newNode->process.state = 1;
    for (int i = 0; i < argc; i++){
      copyString(newNode->process.args[i], argv[i]);
    }
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
  for (int i = 0; i < argc; i++){
      copyString(newNode->process.args[i], argv[i]);
  }
  uint64_t processMemory = (uint64_t) alloc(DEFAULT_PROG_MEM);
  uint64_t sp = initProcess(processMemory + DEFAULT_PROG_MEM, ip, argc, newNode->process.args);
  newNode->process.stackPointer = sp;
  newNode->process.basePointer = processMemory + DEFAULT_PROG_MEM;// no se si aca falta un -1
  newNode->process.processMemory = processMemory;

  return node;
}


int createProcess(uint64_t ip,uint8_t priority,uint64_t argc,char argv[6][21],pipeUserInfo * customStdin,pipeUserInfo * customStdout){
  
  if(priority == 1 && currentPid > 1){
    scheduler->foregroundInUse = 1;
  }
  scheduler->startList = loadProcessData(scheduler->startList, currentPid++,priority,argc, argv, customStdin,customStdout,ip);

  return currentPid-1;
}

int createProcessFormatter(uint64_t ip, uint8_t priority, uint64_t argc, char *argv, pipeUserInfo *customStdin, pipeUserInfo *customStdout) {
  int i = 0, j = 0;
  char args[6][21];
  while(i < argc) {
    int k = 0;
    while(argv[j]) {
      args[i][k] = argv[j];
      j++;
      k++;
    }
    args[i][k] = 0;
    j++;
    i++;
  }

  return createProcess(ip, priority, argc, args, customStdin, customStdout);
}



//@TODO:
/*
1)Funcion wrapper pára createProcess para que le pasen siempre un char[6][21] listo
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
    ProcessNode * auxNodeIter = scheduler->startList;
    while(auxNodeIter != NULL) {
      if(auxNodeIter->process.auxPriority > 1 && auxNodeIter->process.pid != scheduler->current->process.pid && auxNodeIter->process.state == 1)  
        auxNodeIter->process.auxPriority--;
      auxNodeIter = auxNodeIter->nextProcess;
    }
    scheduler->quantumForRaisingAuxPriority = QUANTUM_RAISING_PRIORITY;
  }
  
  scheduler->quantumForRaisingAuxPriority--;

  scheduler->current->process.stackPointer = sp;

  ProcessNode *nextProcessCandidate = scheduler->current->nextProcess;
  ProcessNode *auxNextProcess = nextProcessCandidate;
  while(auxNextProcess != NULL) {
    if(nextProcessCandidate->process.state != 1)
      nextProcessCandidate = auxNextProcess;
    if(auxNextProcess->process.auxPriority < nextProcessCandidate->process.auxPriority && auxNextProcess->process.state == 1)
      nextProcessCandidate = auxNextProcess;
    auxNextProcess = auxNextProcess->nextProcess;
  }

  ProcessNode * minReadyProcess = scheduler->startList;
  ProcessNode * auxMinReadyProcess = minReadyProcess;
  
  while(auxMinReadyProcess != NULL && auxMinReadyProcess->process.pid != scheduler->current->process.pid){
    if(auxMinReadyProcess->process.pid == 1 && scheduler->foregroundInUse){
      if(minReadyProcess->process.pid == 1){
        minReadyProcess = minReadyProcess->nextProcess;
      }
      auxMinReadyProcess = auxMinReadyProcess->nextProcess;
    }
    
    if(minReadyProcess->process.state != 1 && auxMinReadyProcess->process.pid !=1){
      minReadyProcess = auxMinReadyProcess;
    }

    if(auxMinReadyProcess->process.auxPriority < minReadyProcess->process.auxPriority && auxMinReadyProcess->process.state == 1){
      minReadyProcess = auxMinReadyProcess;
    }
    auxMinReadyProcess = auxMinReadyProcess->nextProcess;
  }
  //no hay proceso anterior que se pueda correr pero el proceso actual se puede vovler a corerr
  if(minReadyProcess->process.state != 1 && scheduler->current->process.state == 1 && scheduler->current->process.pid !=1){
    minReadyProcess = scheduler->current;
  }
  //proceso en foreground corriendo y me recomienda dummy, lo descarto
  if(scheduler->foregroundInUse && minReadyProcess->process.pid == 1){
    minReadyProcess = NULL;
  }
  
  if((nextProcessCandidate == NULL || nextProcessCandidate->process.state != 1) && (minReadyProcess == NULL || minReadyProcess->process.state != 1)){
    
    scheduler->current = starting;
    
    return scheduler->current->process.stackPointer;

  }else if (nextProcessCandidate == NULL || nextProcessCandidate->process.state != 1) {
    
    scheduler->current = minReadyProcess;
    
  }else if (minReadyProcess == NULL || minReadyProcess->process.state !=1) {
    
    scheduler->current = nextProcessCandidate;

  }else {
    if(minReadyProcess->process.auxPriority < nextProcessCandidate->process.auxPriority){
      scheduler->current = minReadyProcess;
    }else {
      scheduler->current = nextProcessCandidate;
    }
  }
  scheduler->current->process.auxPriority = scheduler->current->process.priority;
  
  return scheduler->current->process.stackPointer;
}

static ProcessNode * deleteProcessRec(ProcessNode * node,uint64_t pid){

  if(node == NULL){
    return node;
  }

  if(node->process.pid == pid){
    node->process.state = 2;
    ProcessNode * aux = node->nextProcess;
    deleteProcessFromSem(pid);
    deleteProcessFromPipe(pid);
    free((void *) node->process.processMemory);
    free((void *) node);
    return aux;
  }

  node->nextProcess = deleteProcessRec(node->nextProcess, pid);
  return node;
}

void exitCurrentProcess(){

  if(scheduler->current->process.priority == 1){
    scheduler->foregroundInUse = 0;
  }

  scheduler->startList = deleteProcessRec(scheduler->startList, scheduler->current->process.pid);
}




void addToKeyboardList(){
  if(waitKeyboard->size == MAX_WAITING_KEYBOARD){
    return;
  }
  scheduler->current->process.state = 0;
  waitKeyboard->tail->process = &scheduler->current->process;
  waitKeyboard->tail = waitKeyboard->tail->next;
  waitKeyboard->size++;
  runScheduler();
  return;
}

void awakeKeyboardList(){
  if(waitKeyboard->size == 0){
    return;
  }
  waitKeyboard->size--;
  waitKeyboard->current->process->state = 1;
  waitKeyboard->current = waitKeyboard->current->next;
}

pipeUserInfo * getCurrentStdin(){
  return scheduler->current->process.stdin;
}

pipeUserInfo * getCurrentStdout(){
  return scheduler->current->process.stdout;
}

pcb * getCurrentProcess(){
  return &scheduler->current->process;
}

pcb * blockCurrentProcess(){
  scheduler->current->process.state = 0;
  return &scheduler->current->process;
}
