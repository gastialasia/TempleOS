#include "../include/MemoryManagerWrapper.h"
#include "../include/MemoryManagmentADT.h"

static MemoryManagmentADT mm;

void initMemManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory){
  mm = createMemoryManager(memoryForMemoryManager, managedMemory);
}

void * alloc(unsigned int memToAlloc){
  return memAlloc(mm, memToAlloc);
}

void free(void * memToFree){
  return freeMem(mm, memToFree);
}

void memStatus(unsigned int *status){
  status[0] = heapSize();
  status[1] = heapLeft(mm);
  status[2] = usedHeap(mm);
}

