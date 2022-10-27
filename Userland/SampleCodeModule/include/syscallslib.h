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

#endif
