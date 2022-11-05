#ifndef _sdtlib_
#define _sdtlib_

#include <stdint.h>

#define NULL ((void *)0)

typedef struct
{
  uint64_t rax;
  uint64_t rbx;
  uint64_t rcx;
  uint64_t rdx;
  uint64_t rbp;
  uint64_t rdi;
  uint64_t rsi;
  uint64_t r8;
  uint64_t r9;
  uint64_t r10;
  uint64_t r11;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
} registersT;

typedef struct fd
{
  char readable;
  char writable;
  struct pipe *pipe;
} fd;

typedef struct Semaphore
{
  uint32_t id;
  int value;
} Semaphore;

int strlen(const char *);

int strcmp(const char *str1, const char *str2);

char *strcat(char *destination, const char *source);

void putchar(const char c);

void printf(const char *);

void inforeg(registersT *regs);

void printInt(int num);

void getchar(char *c);

int scanf(char *buffer);

void clear();

void printdate();

void sleep(int secs);

void divzero();

void opcode();

int strcpy(char *dest, const char *src);

char getDateComponent(int value);

int strcpy(char *dest, const char *src);

void takeSnapShot();

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

uint64_t hex2int(char *hex, int *ok);

void printReg(const char *regName, uint64_t regValue);

void printMem(uint64_t pointer, char *buffer);

char *strtok(char *srcString, char *delim);

void *malloc(unsigned int bytes);

void free(void *memToFree);

void memStatus(unsigned int *status);

int createProcess(uint64_t ip, uint8_t priority, uint64_t argc, char argv[6][21], fd *customStdin, fd *customStdout);

void exit();

int getpid();

int kill(uint32_t pid);

int block(uint32_t pid);

void yield();

void ps();

int nice(uint32_t pid, uint8_t newPriority);

Semaphore *semOpen(uint32_t id, int value);

int semClose(Semaphore *sem);

int semPost(Semaphore *sem);

int semWait(Semaphore *sem);

void getAllSems(char *buf);

int createPipe(fd *fd1, fd *fd2);

fd *createFd();

void getAllPipes(char *buf);

int openPipe(fd *user, uint32_t id, uint8_t permisions);

int pipeRead(fd *userPipe, char *buffer);

int pipeWrite(fd *userPipe, const char *string);

void closeFd(fd *user);

int atoi(const char *S);

int itos(int value, char *target);

#endif
