#ifndef _syscallslib_
#define _syscallslib_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

void sys_write(const char * buffer, size_t count);
int sys_read(char * buffer, size_t count);
void sys_clear();
char sys_date(char value);
void sys_sleep(int secs);
void sys_inforeg(registersT*);
void sys_snapshotRegs(void);
unsigned char sys_getLast();
void sys_printMem(uint64_t, char*);
void * sys_malloc(unsigned int bytes);
void sys_free(void * memToFree);
void sys_memStatus(unsigned int * status);
int sys_createProcess(uint64_t ip, uint8_t priority, uint64_t argc, char * argv, pipeUserInfo *customStdin, pipeUserInfo *customStdout);
void sys_exit();
int sys_getpid();
void sys_ps(char * buffer);

#endif
