#ifdef BUDDY

#include <MemoryManagmentADT.h>
#include <stdint.h>
#include <tools.h>

#define MINIMUM_BLOCK_SIZE 512

typedef struct MemBlock {
  unsigned char free;
  int history;
  struct MemBlock *nextMemBlock;
  unsigned int blockSize;
} MemBlock;

typedef struct MemoryManagmentCDT {
  MemBlock start;
  MemBlock end;
  unsigned int freeBytesRemaining;
} MemoryManagmentCDT;

static const uint16_t STRUCT_SIZE =
    ((sizeof(MemBlock) + (BYTE_ALIGMENT - 1)) & ~MASK_BYTE_ALIGMENT);

MemoryManagmentADT createMemoryManagment(
    void *const restrict memForMemoryManagment,
    void *const restrict managedMem) {
  MemoryManagmentADT memoryManagment =
      (MemoryManagmentADT)memForMemoryManagment;
  memoryManagment->freeBytesRemaining = TOTAL_HEAP_SIZE;

  MemBlock *startingBlock = (void *)managedMem;

  memoryManagment->start.nextMemBlock = (void *)startingBlock;
  memoryManagment->start.blockSize = (unsigned int)0;

  memoryManagment->end.nextMemBlock = NULL;
  memoryManagment->end.blockSize = TOTAL_HEAP_SIZE;

  startingBlock->blockSize = TOTAL_HEAP_SIZE;
  startingBlock->nextMemBlock = &memoryManagment->end;

  startingBlock->free = 1;
  startingBlock->history = 0;

  return memoryManagment;
}

static void removeBlockFromList(MemoryManagmentADT memoryManager,
                                MemBlock *toDelete) {
  MemBlock *iter = &memoryManager->start;

  while (iter != NULL && iter->nextMemBlock != toDelete) {
    iter = iter->nextMemBlock;
  }
  if (iter != NULL) {
    iter->nextMemBlock = iter->nextMemBlock->nextMemBlock;
  }
}

static int insertBlockIntoFreeList(MemoryManagmentADT memoryManager,
                                   MemBlock *blockToInsert,
                                   unsigned char merge) {
  if (blockToInsert->blockSize == TOTAL_HEAP_SIZE) {
    memoryManager->start.nextMemBlock = blockToInsert;
    blockToInsert->nextMemBlock = &memoryManager->end;
    blockToInsert->free = 1;
    return blockToInsert->blockSize;
  }

  MemBlock *buddy = NULL;
  int auxMerge = 0;

  if (merge) {
    if ((blockToInsert->history & 0x1) == 1) {  // bloque derecho
      buddy = (MemBlock *)((uint64_t)blockToInsert - blockToInsert->blockSize);
      if (buddy->free && buddy->blockSize == blockToInsert->blockSize) {
        buddy->blockSize *= 2;
        auxMerge = 1;
        blockToInsert = buddy;
      }
    } else {
      buddy = (MemBlock *)((uint64_t)blockToInsert + blockToInsert->blockSize);
      if (buddy->free && buddy->blockSize == blockToInsert->blockSize) {
        blockToInsert->blockSize *= 2;
        auxMerge = 1;
      }
    }
  }

  if (auxMerge == 1) {
    blockToInsert->history = blockToInsert->history >> 1;
    removeBlockFromList(memoryManager, buddy);
    return insertBlockIntoFreeList(memoryManager, blockToInsert, 1);
  }

  MemBlock *iter = &memoryManager->start;
  int blockSize = blockToInsert->blockSize;

  while (iter->nextMemBlock->blockSize < blockSize) {
    iter = iter->nextMemBlock;
  }

  blockToInsert->nextMemBlock = iter->nextMemBlock;
  iter->nextMemBlock = blockToInsert;
  blockToInsert->free = 1;
  return blockToInsert->blockSize;
}

void *memAlloc(MemoryManagmentADT const memoryManager,
               unsigned int memToAllocate) {
  MemBlock *currentBlock, *previousBlock;
  void *blockToReturn = NULL;

  if (memToAllocate == 0) {
    return NULL;
  }
  // Increase size so that it can contain a MemBlock
  memToAllocate += STRUCT_SIZE;

  // Byte aligment
  if ((memToAllocate & MASK_BYTE_ALIGMENT) != 0) {
    memToAllocate += (BYTE_ALIGMENT - (memToAllocate & MASK_BYTE_ALIGMENT));
  }

  if (memToAllocate > TOTAL_HEAP_SIZE) {
    return NULL;
  }

  previousBlock = &memoryManager->start;
  currentBlock = memoryManager->start.nextMemBlock;

  while ((currentBlock->blockSize < memToAllocate) &&
         (currentBlock->nextMemBlock != NULL)) {
    previousBlock = currentBlock;
    currentBlock = currentBlock->nextMemBlock;
  }

  if (currentBlock == &memoryManager->end) {
    return NULL;
  }
  blockToReturn =
      (void *)(((uint8_t *)previousBlock->nextMemBlock) + STRUCT_SIZE);

  previousBlock->nextMemBlock = currentBlock->nextMemBlock;

  while (currentBlock->blockSize / 2 >= MINIMUM_BLOCK_SIZE &&
         currentBlock->blockSize / 2 >= memToAllocate) {
    currentBlock->blockSize /= 2;
    currentBlock->history = currentBlock->history << 1;
    MemBlock *newBlock =
        (void *)(((uint64_t)currentBlock) + currentBlock->blockSize);
    newBlock->blockSize = currentBlock->blockSize;
    newBlock->free = 1;
    newBlock->history = currentBlock->history | 0x1;  // marco el bloque derecho
    insertBlockIntoFreeList(memoryManager, newBlock, 0);
  }

  memoryManager->freeBytesRemaining -= currentBlock->blockSize;
  currentBlock->free = 0;

  return blockToReturn;
}

void freeMem(MemoryManagmentADT const memoryManager, void *block) {
  if (block == NULL) {
    return;
  }

  uint8_t *memToFree = ((uint8_t *)block);
  MemBlock *blockToFree;

  memToFree -= STRUCT_SIZE;

  blockToFree = (void *)memToFree;

  unsigned int aux = blockToFree->blockSize;

  insertBlockIntoFreeList(memoryManager, ((MemBlock *)blockToFree), 1);
  memoryManager->freeBytesRemaining += aux;
}

unsigned int heapSize() { return TOTAL_HEAP_SIZE; }

unsigned int heapLeft(MemoryManagmentADT memoryManager) {
  return memoryManager->freeBytesRemaining;
}

unsigned int usedHeap(MemoryManagmentADT memoryManager) {
  return heapSize() - heapLeft(memoryManager);
}

#endif
