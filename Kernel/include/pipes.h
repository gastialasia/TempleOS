#include "scheduler.h"

int createPipes(fd * pui1, fd * pui2);
fd * createfd();
int openPipeID(fd * user,uint32_t id, uint8_t permisions);
void closeUserPipe(fd * user);
int pipeWrite(fd * userPipe, char * string);
int pipeRead(fd * userPipe,char * buffer,int limit);
void deleteProcessFromPipe(int64_t pid);


