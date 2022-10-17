#include "scheduler.h"

int createPipes(pipeUserInfo * pui1, pipeUserInfo * pui2);
pipeUserInfo * createPipeUserInfo();
int openPipeID(pipeUserInfo * user,uint32_t id, uint8_t permisions);
void deletePipe(pipe * pipe);
void closeUserPipe(pipeUserInfo * user);
int pipeWrite(pipeUserInfo * userPipe, char * string);
int pipeRead(pipeUserInfo * userPipe,char * buffer,int limit);



