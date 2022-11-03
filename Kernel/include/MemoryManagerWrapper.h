#ifndef MEMORY_MANAGER_WRAPPER_H
#define MEMORY_MANAGER_WRAPPER_H

void initMemManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void * alloc(unsigned int memToAlloc);

void free(void *memToFree);

void memStatus(unsigned int * status);

#endif
