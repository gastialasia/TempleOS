#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdint.h>

#define NULL ((void *) 0)

char isAlpha(char c);
char toMayusc(char c);
char isPipe(char c);
int strcmp(const char *str1, const char *str2);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
int strlen(const char * str);
char* strcat(char* destination, const char* source);

#endif
