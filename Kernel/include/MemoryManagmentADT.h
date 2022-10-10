#ifndef MEMORY_MANAGMENT_H
#define MEMORY_MANAGMENT_H

#define NULL ((void *) 0)
#define BYTE_ALIGMENT 8
#define MASK_BYTE_ALIGMENT 0x07
#define TOTAL_HEAP_SIZE 134217728 // 128MB 
typedef struct MemoryManagmentCDT * MemoryManagmentADT;


MemoryManagmentADT createMemoryManager(void *const memoryForMemoryManager,void *const managedMemory);
void * memAlloc(MemoryManagmentADT const memoryManager, unsigned int memToAllocate); 
void freeMem(MemoryManagmentADT const memoryManager, void * block);
unsigned int heapSize();
unsigned int heapLeft(MemoryManagmentADT memoryManager);
unsigned int usedHeap(MemoryManagmentADT memoryManager);

#endif
