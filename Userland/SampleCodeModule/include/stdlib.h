#ifndef _sdtlib_
#define _sdtlib_

#include <stdint.h>

typedef struct{
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


int strlen(const char *);

int strcmp(const char *str1, const char *str2);

void putchar(const char c);

void printf(const char *);

void inforeg(registersT * regs);

void printInt(int num);

char getchar();

int scanf(char * buffer);

void clear();

void printdate();

void sleep(int secs);

void divzero();

void opcode();

int strcpy(char *dest, const char *src);

char getDateComponent(int value);

unsigned char getlast();

int strcpy(char *dest, const char *src);

void takeSnapShot();

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

uint64_t hex2int(char *hex, int *ok);

void printReg(const char *regName, uint64_t regValue);

void printMem(uint64_t pointer, char*buffer);

void * malloc(unsigned int bytes);

void free(void * memToFree);

void memStatus(unsigned int * status);

#endif
