#ifndef _syscallslib_
#define _syscallslib_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

void sysWrite(const char *buffer, size_t count);
int sysRead(char *buffer, size_t count);
void sysClear();
char sysDate(char value);
void sysSleep(int secs);
void sysInforeg(registersT *);
void sysSnapshotRegs(void);
unsigned char sysGetLast();
void sysPrintMem(uint64_t, char *);
void *sysMalloc(unsigned int bytes);
void sysFree(void *memToFree);
void sysMemStatus(unsigned int *status);
int sysCreateProcess(uint64_t ip, uint8_t priority, uint64_t argc, char argv[ARG_QTY][ARG_LEN], fd *customStdin, fd *customStdout);
void sysExit();
int sysGetpid();
void sysPs(char *buffer);
int sysKill(uint32_t pid);
int sysChangepriority(uint32_t pid, uint8_t newPriority);
int sysChangestate(uint32_t pid);
void sysYield();
Semaphore *sysSemOpen(uint32_t id, int value);
int sysSemClose(Semaphore *sem);
int sysSemPost(Semaphore *sem);
int sysSemWait(Semaphore *sem);
void sysGetAllSems(char *buffer);
int sysCreatePipe(fd *fd1, fd *fd2);
void sysGetAllPipes(char *buf);
int sysOpenPipe(fd *user, uint32_t id, uint8_t permisions);
int sysPipeRead(fd *userPipe, char *buffer);
int sysPipeWrite(fd *userPipe, const char *string);
void sysCloseFd(fd *user);

#endif
