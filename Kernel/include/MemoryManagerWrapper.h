#ifndef MEMORYMANAGERWRAPPER_H
#define MEMORYMANAGERWRAPPER_H

#define NULL ((void *) 0)


void initMemManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void * alloc(unsigned int memToAlloc);

void free(void *memToFree);

void memStatus(unsigned int * status);

#endif
