// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <MemoryManagerWrapper.h>
#include <MemoryManagmentADT.h>
#include <tools.h>

static MemoryManagmentADT memMang;

void initMemManager(void *const restrict memoryForMemoryManager,
                    void *const restrict managedMemory) {
  memMang = createMemoryManagment(memoryForMemoryManager, managedMemory);
}

void *alloc(unsigned int memToAlloc) { return memAlloc(memMang, memToAlloc); }

void free(void *memToFree) { return freeMem(memMang, memToFree); }

void memStatus(unsigned int *status) {
  status[0] = heapSize();
  status[1] = heapLeft(memMang);
  status[2] = usedHeap(memMang);
}
