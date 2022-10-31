#include "../include/semaphore.h"
#include "../include/scheduler.h"

#define MAX_SEM 10
#define MAX_PROCESS 10

typedef struct Semaphore{
  uint32_t id;
  int value;
  pcb * queuqe[MAX_PROCESS];// struct de procesos
  uint32_t waiting;
  uint8_t lock;
}Semaphore;

Semaphore * sems[MAX_SEM];
uint32_t size = 0;
uint8_t mutexSem = 0;


// acorde a la de c si no existe se crea uno nuevo, si no se retorna el existente que coincida con la id
Semaphore * sem_open(uint32_t id,int value){
   //agaarro el mutexSem para modificar las var globales
  while(_xchg(&mutexSem, 1) != 0);

  if(value < 0 || size == MAX_SEM){
    return NULL;
  }

  int semIter = 0;
  while(semIter < size && sems[semIter]->id != id){
    semIter++;
  }

  //si no se encontro se crea uno nuevo
  if(semIter == size){
    sems[semIter] = (Semaphore *) alloc(sizeof(Semaphore));
    sems[semIter]->id = id;
    sems[semIter]->value = value;
    //agregar pcb @TODO:@pato
    sems[semIter]->waiting = 0;
    sems[semIter]->lock = 0;//unlock
    _xchg(&mutexSem,0);
    return sems[size++];
  }

  _xchg(&mutexSem,0);
  return sems[semIter];

}

int sem_close(Semaphore * semToClose){
  
  if(semToClose->waiting > 0){
    return -1;
  }

  while(_xchg(&mutexSem, 1) != 0);

  int semIter = 0;
  while(semIter < size && sems[semIter]->id != semToClose->id){
    semIter++;
  }
  
  if(semIter == size){
    _xchg(&mutexSem, 0);
    return -1; // INVALID ID
  }
  //sejo a todo los sems en "orden", osea sin empacio para facilitar la busqueda mas adelante
  while(semIter < size-1){
    sems[semIter] = sems[semIter+1];
    semIter++;
  }

  size--;
  free(semToClose);

  _xchg(&mutexSem, 0);
  return 1;
}

int sem_post(Semaphore * sem){

  while (_xchg(&sem->lock,1) != 0);

  sem->value++;
  
  // no hay nadie esperando el semaforo
  if(sem->waiting == 0){
    _xchg(&mutexSem, 0);
    return 0;
  }

  sem->queuqe[0]->state = 1;
  sem->waiting--;

  for(int i = 0; i < sem->waiting; i++){
    sem->queuqe[i] = sem->queuqe[i+1];
  }
  _xchg(&sem->lock, 0);
  return 0;
}

int sem_wait(Semaphore * sem){

  while(_xchg(&sem->lock, 1) !=0);
  
  while (sem->value == 0) {
    
    if(sem->waiting == MAX_PROCESS){
      return -1;
    }

     sem->queuqe[sem->waiting++] = blockCurrentProcess();
    
    _xchg(&sem->lock, 0); // suelto el semaforo interno de cada semaforo
    //corre el scheduler hasta que se pueda ir el procesos
    runScheduler();
  }

  //si salgo del while significa que se libero un proceso

  sem->value--;

  //suelto el lock del semaforo por si paso de una
  _xchg(&sem->lock,0);

  return 0;
}


void deleteProcessFromSem(int64_t pid){

  for(int i = 0; i < size; i++){
    for(int j = 0 ; j < sems[i]->waiting;j++){
      if(sems[i]->queuqe[j]->pid == pid){
        sems[i]->waiting--;

        for(int k = j; k < sems[i]->waiting;k++){
          sems[i]->queuqe[k] = sems[i]->queuqe[k+1];
        }
        return;
      }
    }
  }


}





