#ifndef MEMORY_MANAGMENT_ADT_H
#define MEMORY_MANAGMENT_ADT_H

#include <tools.h>

#define NULL ((void *)0)
#define BYTE_ALIGMENT 8
#define MASK_BYTE_ALIGMENT 0x07
#define TOTAL_HEAP_SIZE 134217728 // 128MB
typedef struct MemoryManagmentCDT *MemoryManagmentADT;

MemoryManagmentADT createMemoryManagment(void *const restrict memForMemoryManagment, void *const restrict managedMem);
void *memAlloc(MemoryManagmentADT const memoryManager, unsigned int memToAllocate);
void freeMem(MemoryManagmentADT const memoryManager, void *block);
unsigned int heapSize();
unsigned int heapLeft(MemoryManagmentADT memoryManager);
unsigned int usedHeap(MemoryManagmentADT memoryManager);

#endif
