#include <stdint.h>
#include "../include/MemoryManagerWrapper.h"
#include "../include/pipes.h"
#include "../include/tools.h"

#define MAX_PIPES 20
#define PIPE_SIZE 512 // byres de buffer del pipe

#define PIPEID 0
#define PERMISSIONS 1
#define BYTESTOREAD 2

typedef struct pipe{
  uint32_t id;
  char data[PIPE_SIZE];
  uint32_t readPos;
  uint32_t writePos;
  uint8_t readPermition;
  uint8_t writePermition;
  uint32_t bytesToRead;
  pcb * waitingProcess;
}pipe;

pipe * pipes[MAX_PIPES];
uint32_t pipeSize = 0;
uint32_t ids = 0; // for unique ids

void normalizePipes(char *buf, char *data, int field);

int createPipes(fd * pui1, fd * pui2){

  if(pipeSize == MAX_PIPES){
    return -1;
  }
  
  pipes[pipeSize] = (pipe *) alloc(sizeof(pipe));
  pipes[pipeSize]->readPos = 0;
  pipes[pipeSize]->writePos = 0;
  pipes[pipeSize]->readPermition = 1;
  pipes[pipeSize]->writePermition = 1;
  pipes[pipeSize]->bytesToRead = 0;
  pipes[pipeSize]->id = 100+ ids++;
  pipes[pipeSize]->waitingProcess = NULL;
  pui1->pipe = pipes[pipeSize];
  pui1->writable = 0;
  pui1->readable = 1;
  pui2->pipe = pipes[pipeSize];
  pui2->writable = 1;
  pui2->readable = 0;
  pipeSize++;
  
  return 1;
}

int openPipeID(fd * user,uint32_t id, uint8_t permisions){

  if(id < 100){
    return -1;
  }

  int i =0;

  while(i < pipeSize && pipes[i]->id != id){
    i++;
  }

  user->readable = permisions? 1:0;
  user->writable = permisions? 0:1;

  if(i == pipeSize){

    if(pipeSize == MAX_PIPES){
      return -1;
    }

    pipes[pipeSize] = (pipe *) alloc(sizeof(pipe));
    user->pipe = pipes[pipeSize++];
    user->pipe->id = id;
    user->pipe->readPos = 0;
    user->pipe->writePos = 0;
    user->pipe->bytesToRead = 0;
    user->pipe->readPermition = permisions? 1:0;
    user->pipe->writePermition = permisions? 0:1;

    return 0;
  }

  if((permisions && pipes[i]->readPermition) || (!permisions && pipes[i]->writePermition)){
    return -1;
  }

  if(permisions){
    user->pipe->readPermition = 1;
  }else {
    user->pipe->writePermition = 1;
  }
  user->pipe = pipes[i];
  return 1;
}

static void deletePipe(pipe * pipe){

  int pos = 0;
  
  while(pos < pipeSize && pipes[pos]->id != pipe->id){
    pos++;
  }

  if(pos == pipeSize){
    return;
  }

  for(;pos<pipeSize-1;pos++){
    pipes[pos] = pipes[pos+1];
  }
  pipeSize--;
}

void closeUserPipe(fd * user){
  
  if(user == NULL){
    return;
  }

  if(user->readable){
    user->pipe->readPermition = 0;
    if(!user->pipe->writePermition){
      deletePipe(user->pipe);
      free(user->pipe);
    }else if (user->pipe->waitingProcess != NULL) {
      user->pipe->waitingProcess->state = 1;
      user->pipe->waitingProcess = NULL;
    }
  }else if (user->writable) {
    user->pipe->writePermition = 0;
    if(!user->pipe->readPermition){
      deletePipe(user->pipe);
      free(user->pipe);
    }else if (user->pipe->waitingProcess != NULL) {
      user->pipe->waitingProcess->state = 1;
      user->pipe->waitingProcess = NULL;
    }
  }

  free(user);

}

int pipeWrite(fd * userPipe, char * string){

  if(!userPipe->writable || (!userPipe->pipe->readPermition && userPipe->pipe->bytesToRead == PIPE_SIZE)){
    return -1;
  }

  int i = 0;
  while(string[i]){
    
    if(userPipe->pipe->bytesToRead == PIPE_SIZE){
      userPipe->pipe->waitingProcess = blockCurrentProcess();
      runScheduler();
    }

    userPipe->pipe->data[userPipe->pipe->writePos++] = string[i++];
    if(userPipe->pipe->writePos == PIPE_SIZE){
      userPipe->pipe->writePos = 0;
    }
    userPipe->pipe->bytesToRead++;

  }

  if(userPipe->pipe->waitingProcess != NULL){
    userPipe->pipe->waitingProcess->state = 1;
    userPipe->pipe->waitingProcess = NULL;
  }
  return i;//cantidad de chars escritos

}

int pipeRead(fd * userPipe,char * buffer,int limit){

  if(!userPipe->readable){
    return -1;
  }

  if(userPipe->pipe->bytesToRead == 0){
    userPipe->pipe->waitingProcess = blockCurrentProcess();
    runScheduler();
  }

  int i = 0;
  while(userPipe->pipe->bytesToRead > 0 && i < limit){

    userPipe->pipe->bytesToRead--;
    
    if(userPipe->pipe->readPos == PIPE_SIZE){
      userPipe->pipe->readPos = 0;
    }

    buffer[i++] = userPipe->pipe->data[userPipe->pipe->readPos++];

  }

  if(userPipe->pipe->waitingProcess != NULL){
    userPipe->pipe->waitingProcess->state = 1;
    userPipe->pipe->waitingProcess = NULL;
  }

  return i;
}

void deleteProcessFromPipe(int64_t pid){

  for(int i = 0; i < pipeSize; i++){
    if(pipes[i]->waitingProcess->pid == pid)
      pipes[i]->waitingProcess = NULL;
  }

}

void getAllPipes(char * buf) {
  strcat(buf, "PipeID  Permissions  BytesToR  BlockedPIDs\n");
  char idStr[6];
  char permissions[3]={0};
  for(int i=0; i < pipeSize; i++){
    uintToBase(pipes[i]->id, idStr, 10);
    normalizePipes(buf, idStr, PIPEID);
    
    strcat(permissions, pipes[i]->readPermition?"r":"-");
    strcat(permissions, pipes[i]->writePermition?"w":"-");
    normalizePipes(buf, permissions, PERMISSIONS);

    uintToBase(pipes[i]->bytesToRead, idStr, 10);
    normalizePipes(buf, idStr, BYTESTOREAD);

    // for(j=0; j < pipes[i]->waiting; j++){
    //   uintToBase(sems[i]->queuqe[j]->pid, idStr, 10);
    //   strcat(buffer, idStr);
    //   if(j!=sems[i]->waiting-1){
    //     strcat(buffer, " - ");
    //   }
    // }
    
    strcat(buf, "\n");
  }
}

void normalizePipes(char *buf, char *data, int field){
  static int fields[]={6, 11, 8};
  int n = fields[field]-strlen(data);
  strcat(buf, data);
  for(int i=0; i<n; i++){
    strcat(buf, " ");
  }
  strcat(buf, "  ");
}




