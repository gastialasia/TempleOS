#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>


void ncPrintL(const char * string);
void ncPrintR(const char * string);

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncPrintCharR(char character);
void ncPrintCharL(char character);

void ncDeleteChar();

void ncPrintCharAt(char character, uint32_t x, uint32_t y);

void ncNewline(void);
void ncNewlineL(void);
void ncNewlineR(void);

void ncPrintDec(uint64_t value);
void ncPrintDecL(uint64_t value);
void ncPrintDecR(uint64_t value);

void ncPrintHex(uint64_t value);
void ncPrintHexL(uint64_t value);
void ncPrintHexR(uint64_t value);

void ncPrintBin(uint64_t value);
void ncPrintBinL(uint64_t value);
void ncPrintBinR(uint64_t value);

void ncPrintBase(uint64_t value, uint32_t base);
void ncPrintBaseL(uint64_t value, uint32_t base);
void ncPrintBaseR(uint64_t value, uint32_t base);

void ncPrintReg(const char *regName, uint64_t regValue);
void ncPrintRegL(const char *regName, uint64_t regValue);
void ncPrintRegR(const char *regName, uint64_t regValue);

void ncMoveLines();
void ncMoveLinesL();
void ncMoveLinesR();

void ncClear();

void ncSplit();
void ncUnSplit();

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

#endif
