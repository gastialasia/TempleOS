#include "../include/MemoryManagerWrapper.h"
#include "../include/shm.h"

#define SHM_SIZE 4096
#define MAX_SHM 10
#define NULL ((void *) 0)

void * shm[SHM_SIZE];


void initSHM(){

  for (int i = 0; i < SHM_SIZE; i++) {
    shm[i] = NULL;
  }

}


void * shmAttach(int id){

 if(id < 0 || id > 10){
    return NULL;
  }

  if(shm[id]){
    return shm[id];
  }


  shm[id] = alloc(SHM_SIZE);
  return shm[id];
}



