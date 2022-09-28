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

static void insertBlockIntoFreeList(MMemoryManagmentADT memoryManager, MemBlock * blockToInsert){

  MemBlock * blockIterator= &memoryManager->start;
  unsigned int blockSizeToinsert = blockToInsert->blockSize;

  while (blockIterator->nextMemBlock->blockSize < blockSizeToinsert) {
    
    blockIterator = blockIterator->nextMemBlock;

  }

  blockToInsert->nextMemBlock = blockIterator->nextMemBlock;
  blockIterator->nextMemBlock = blockToInsert;
  
}

void * memAlloc(MemoryManagmentADT const memoryManager, unsigned int memToAllocate){

  MemBlock * currentBlock,* previousBlock;
  void* blockToReturn = NULL;

  if(memToAllocate == 0){
    return NULL;
  }
  //Increase size so that it can contain a MemBlock
  memToAllocate += STRUCT_SIZE;
  
  //byte aligment
  if((memToAllocate & MASK_BYTE_ALIGMENT) != 0){
    memToAllocate +=(BYTE_ALIGMENT - (memToAllocate & MASK_BYTE_ALIGMENT))
  }
  
  if(memToAllocate < TOTAL_HEAP_SIZE){

    previousBlock = &memoryManager->start;
    currentBlock = &memoryManager->start.nextMemBlock;

    while((currentBlock->blockSize < memToAllocate) && (currentBlock->nextMemBlock != NULL)){
      
      previousBlock = currentBlock;
      currentBlock = currentBlock->nextMemBlock;

    }

    if(currentBlock = &memoryManager->end){
      
      blockToReturn = (void *) (((uint8_t *) previousBlock->nextMemBlock) + STRUCT_SIZE);

      previousBlock->nextMemBlock = currentBlock->nextMemBlock;

      if((currentBlock->blockSize - memToAllocate) > MINIMUM_BLOCK_SIZE){
        MemBlock *newBlock = (void *) (((uint8_t *) currentBlock) + memToAllocate);

        newBlock->blockSize = currentBlock->blockSize - memToAllocate;
        currentBlock->blockSize = memToAllocate;

        insertBlockIntoFreeList(newBlock);
      }

      memoryManager->freeBytesRemaining -=currentBlock->blockSize;

    }

    return blockToReturn;

  }

}

void freeMem(MemoryManagmentADT const memoryManager, void * block){

  if(block == NULL){
    return;
  }

  uint8_t * memToFree =((uint8_t *) block);
  MemBlock * blockToFree;

  memToFree -= STRUCT_SIZE;

  blockToFree = (void *) memToFree;

  insertBlockIntoFreeList(memoryManager,((MemBlock *) blockToFree));
  memoryManager->freeBytesRemaining += blockToFree->blockSize;

}

unsigned int heapSize(){
  return TOTAL_HEAP_SIZE;
}

unsigned int heapLeft(MemoryManagmentADT memoryManager){
  return memoryManager->freeBytesRemaining;
}

unsigned int usedHeap(MemoryManagmentADT memoryManager){
  return heapSize() - usedHeap(memoryManager);
}


#endif
