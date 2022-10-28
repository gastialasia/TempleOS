#ifndef _syscalls_
#define _syscalls_

#include <stdint.h>
#include <stddef.h>
#include <naiveConsole.h>
#include <keyboard.h>
#include <interrupts.h>
#include <time.h>
#include <lib.h>
#include <tools.h>
#include <scheduler.h>

int64_t write(const char * buffer, size_t count);

int64_t read(char * buffer, size_t count);

void clear();

void inforeg(registersT* regs);

int64_t date(char value);

void sleep(int ms);

int64_t getLast();

void saveRegisters(registersT* registers);

void loadBackup(registersT* regs, registersT* backup);

void snapshotRegs();

void printMem(uint64_t pointer, unsigned char * buf);

void loadBackupRegs(registersT* regs, registersT *backup);

void * malloc(unsigned int bytes);

void mfree(void * memToFree);

void mStatus(unsigned int * status);

int cProcess(uint64_t ip, uint8_t priority, uint64_t argc, char *argv, pipeUserInfo *customStdin, pipeUserInfo *customStdout);

void eProcess();

#endif
