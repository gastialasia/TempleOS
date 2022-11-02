#include "../include/scheduler.h"
#include "../include/interrupts.h"
#include "../include/tools.h"
#include <stdint.h>
#include <stdlib.h>
#include <naiveConsole.h>


#define QUANTUM 1
#define DEFAULT_PROG_MEM 4096
#define MAX_WAITING_KEYBOARD 15
#define QUANTUM_RAISING_PRIORITY 5

//Column fields
#define NAME 0
#define ID 1
#define PRIORITY 2
#define RSP 3
#define RBP 4
#define FOREGROUND 5
#define STATE 6

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
uint32_t currentPid;

void normalizeSpaces(char *buf, char *data, int field);

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
static char* strcpy(char* destination, const char* source) {
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

static ProcessNode * loadProcessData(ProcessNode * node ,uint32_t pid,uint8_t priority,int argc,char argv[ARG_QTY][ARG_LEN],fd * customStdin,fd * customStdout,uint64_t ip){

  if(node == NULL){
    ProcessNode * newNode = (ProcessNode *) alloc(sizeof(ProcessNode));
    newNode->process.pid = pid;
    newNode->process.priority = priority;
    newNode->process.auxPriority = priority;
    newNode->process.stdin = customStdin;
    newNode->process.stdout = customStdout;
    newNode->process.state = 1;
    for (int i = 0; i < argc; i++){
      strcpy(newNode->process.args[i], argv[i]);
    }
    uint64_t processMemory = (uint64_t) alloc(DEFAULT_PROG_MEM);
    uint64_t sp = initProcess(processMemory + DEFAULT_PROG_MEM, ip, argc, newNode->process.args);
    newNode->process.stackPointer = sp;
    newNode->process.basePointer = processMemory + DEFAULT_PROG_MEM -1;// no se si aca falta un -1
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
      strcpy(newNode->process.args[i], argv[i]);
  }
  uint64_t processMemory = (uint64_t) alloc(DEFAULT_PROG_MEM);
  uint64_t sp = initProcess(processMemory + DEFAULT_PROG_MEM, ip, argc, newNode->process.args);
  newNode->process.stackPointer = sp;
  newNode->process.basePointer = processMemory + DEFAULT_PROG_MEM -1;// no se si aca falta un -1
  newNode->process.processMemory = processMemory;
  return node;
}


int createProcess(uint64_t ip,uint8_t priority,uint64_t argc,char argv[ARG_QTY][ARG_LEN],fd * customStdin,fd * customStdout){
  if(priority == 1 && currentPid > 1){
    scheduler->foregroundInUse = 1;
  }
  scheduler->startList = loadProcessData(scheduler->startList, currentPid++,priority,argc, argv, customStdin,customStdout,ip);

  return currentPid-1;
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
      continue;
    }
    
    if(minReadyProcess->process.state != 1 &&(!scheduler->foregroundInUse || auxMinReadyProcess->process.pid !=1)){
      minReadyProcess = auxMinReadyProcess;
    }

    if(auxMinReadyProcess->process.auxPriority < minReadyProcess->process.auxPriority && minReadyProcess->process.state == 1){
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

static ProcessNode * deleteProcessRec(ProcessNode * node,uint64_t pid, int * found){

  if(node == NULL){
    return node;
  }

  if(node->process.pid == pid){
    *found = 1;
    node->process.state = 2;
    ProcessNode * aux = node->nextProcess;
    deleteProcessFromSem(pid);
    deleteProcessFromPipe(pid);
    free((void *) node->process.processMemory);
    free((void *) node);
    return aux;
  }

  node->nextProcess = deleteProcessRec(node->nextProcess, pid, found);
  return node;
}

void exitCurrentProcess(){

  if(scheduler->current->process.priority == 1){
    scheduler->foregroundInUse = 0;
  }
  int found;
  scheduler->startList = deleteProcessRec(scheduler->startList, scheduler->current->process.pid, &found);
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
  waitKeyboard->current->process->auxPriority = 1;
  waitKeyboard->current = waitKeyboard->current->next;
}

fd * getCurrentStdin(){
  return scheduler->current->process.stdin;
}

fd * getCurrentStdout(){
  return scheduler->current->process.stdout;
}

pcb * getCurrentProcess(){
  return &scheduler->current->process;
}

int getCurrentPID(){
  return scheduler->current->process.pid;
}

pcb * blockCurrentProcess(){
  scheduler->current->process.state = 0;
  return &scheduler->current->process;
}

void getAllProcesses(char * buf) {
  strcat(buf, "Name        ID  Priority  RSP       RBP       Foreground  State\n");
  ProcessNode * current = scheduler->startList;
  while(current != NULL){
    normalizeSpaces(buf, current->process.args[0], NAME);

    uint32_t pid = current->process.pid;
    char pidStr[6];
    uintToBase(pid, pidStr, 10);
    normalizeSpaces(buf, pidStr, ID);

    uint32_t priority = current->process.priority;
    char priorityStr[3];
    uintToBase(priority, priorityStr, 10);
    normalizeSpaces(buf, priorityStr, PRIORITY);

    uint32_t rsp = current->process.stackPointer;
    char rspStr[10];
    uintToBase(rsp, rspStr, 16);
    normalizeSpaces(buf, rspStr, RSP);

    uint32_t rbp = current->process.basePointer;
    char rbpStr[10];
    uintToBase(rbp, rbpStr, 16);
    normalizeSpaces(buf, rbpStr, RBP);

    normalizeSpaces(buf, priority==1?"yes":"no", FOREGROUND);

    uint8_t state = current->process.state;
    normalizeSpaces(buf, state==1?"running":"blocked", STATE);

    strcat(buf, "\n");
    current = current->nextProcess;
  }
}

void normalizeSpaces(char *buf, char *data, int field){
  static int fields[]={10, 2, 8, 8, 8, 10, 7};
  int n = fields[field]-strlen(data);
  strcat(buf, data);
  for(int i=0; i<n; i++){
    strcat(buf, " ");
  }
  strcat(buf, "  ");
}

static pcb *getPCB(ProcessNode *node, uint32_t pid){
  if(node == NULL)
    return NULL;

  if(node->process.pid == pid)
    return &node->process;
  
  return getPCB(node->nextProcess, pid);
}

int changeProcessState(uint32_t pid){
  if(pid<1)
    return 0 ;

  pcb * process = getPCB(scheduler->startList, pid);
  if(process == NULL)
    return 0;

  if(process->state == 1){
    process->state = 0;
    
    if(scheduler->current->process.pid == pid)
      runScheduler();
  } else if (process->state == 0) {
    process->state = 1;
  }
  return 1;
}

int killPid(uint32_t pid){
  int found = 0;
  if (pid>1){
    scheduler->startList = deleteProcessRec(scheduler->startList, pid, &found);
  }
  return found;

}

int changeProcessPriority(uint32_t pid, uint8_t newPriority){
  if(pid<1)
    return 1; //Error: invalid parameter
  
  //Reordenar la lista
  ProcessNode * auxList = scheduler->startList;
  while(auxList->nextProcess!=NULL && auxList->nextProcess->process.pid != pid){
    auxList = auxList->nextProcess;
  }

  if(auxList->nextProcess == NULL){
    return 2; //Error: node with that pid was not found in the list
  }

  //guardamos el nodo y lo removemos de la lista
  ProcessNode *chosenOne = auxList->nextProcess;
  auxList->nextProcess = chosenOne->nextProcess;

  chosenOne->process.priority = newPriority;
  chosenOne->process.auxPriority = newPriority;

  //Insertamos el nodo ordenado con su nueva prioridad
  auxList = scheduler->startList;
  while(auxList->nextProcess != NULL && newPriority >= auxList->nextProcess->process.priority){
    auxList = auxList->nextProcess;
  }
  chosenOne->nextProcess = auxList->nextProcess;
  auxList->nextProcess = chosenOne;
}
