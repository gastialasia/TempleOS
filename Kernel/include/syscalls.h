#ifndef _syscalls_
#define _syscalls_

#include <interrupts.h>
#include <keyboard.h>
#include <lib.h>
#include <naiveConsole.h>
#include <scheduler.h>
#include <stddef.h>
#include <stdint.h>
#include <tools.h>

int64_t write(const char* buffer, size_t count);

int64_t read(char* buffer, size_t count);

void clear();

void inforeg(registersT* regs);

void sleep(int ms);

int64_t date(char value);

int64_t getLast();

void saveRegisters(registersT* registers);

void loadBackup(registersT* regs, registersT* backup);

void snapshotRegs();

void loadBackupRegs(registersT* regs, registersT* backup);

void exitProcess();

#endif