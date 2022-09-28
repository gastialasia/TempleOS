#ifdef HEAP2

#include "../include/MemoryManagmentADT.h"
#include <stdint.h>
// source:https://github.com/Infineon/freertos/blob/master/Source/portable/MemMang/heap_2.c


typedef struct MemBlock {
  struct MemBlock * nextMemBlock;
  unsigned int blockSize;
} MemBlock;

typedef struct MemoryManagmentCDT {
  MemBlock start;
  MemBlock end;
  unsigned int freeBytesRemaining;
}MemoryManagmentCDT;

static const uint16_t STRUCT_SIZE ((sizeof(MemBlock) +(BYTE_ALIGMENT -1)) & ~MASK_BYTE_ALIGMENT);

#define MINIMUM_BLOCK_SIZE ((size_t) (STRUCT_SIZE * 2))
#define NULL ((void *) 0)

MemoryManagmentADT createMemoryManagment(void * const restrict memForMemoryManagment, void * const restrict managedMem){
  
  MemoryManagmentADT memoryManagment = (MemoryManagmentADT) memForMemoryManagment;
  memoryManagment->freeBytesRemaining = TOTAL_HEAP_SIZE;
  
  MemBlock * startingBlock = (void *) managedMem;
  
  memoryManagment->start.nextMemBlock = (void *) startingBlock;
  memoryManagment->start.blockSize = (unsigned int) 0;

  memoryManagment->end.nextMemBlock = NULL;
  memoryManagment->end.blockSize = TOTAL_HEAP_SIZE;

  startingBlock->blockSize = TOTAL_HEAP_SIZE;
  startingBlock->nextMemBlock = &memoryManagment->end;

  return memoryManagment;

}



#endif
