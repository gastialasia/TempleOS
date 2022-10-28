#include "scheduler.h"

int createPipes(pipeUserInfo * pui1, pipeUserInfo * pui2);
pipeUserInfo * createPipeUserInfo();
int openPipeID(pipeUserInfo * user,uint32_t id, uint8_t permisions);
void closeUserPipe(pipeUserInfo * user);
int pipeWrite(pipeUserInfo * userPipe, char * string);
int pipeRead(pipeUserInfo * userPipe,char * buffer,int limit);
void deleteProcessFromPipe(int64_t pid);


