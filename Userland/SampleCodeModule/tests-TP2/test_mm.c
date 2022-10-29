#include "../include/test_mm.h"
#include "../include/stdlib.h"
#include "../include/test_util.h"
#include <programs.h>

#define MAX_BLOCKS 128

static void * myMemset(void * destination, int c, int len){
  uint8_t value = (uint8_t) c;
  char * origin = (char *) destination;
  while(len--){
    origin[len] = value;
  }
  return destination;
}

char test_mm(){ //test_mm(uint64_t argc, char *argv[])

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  //if (argc != 1) return -1;

  //if ((max_memory = satoi(argv[0])) <= 0) return -1;
  max_memory=127506842;

  // int pid = getpid();
  // printInt(pid);
  // putchar('\n');

  ps();
  sleep(10000);

  while (1) {
    rq = 0;
    total = 0;

    memStatusProgram();

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < max_memory){
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = malloc(mm_rqs[rq].size);

      if(mm_rqs[rq].address){
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        myMemset(mm_rqs[i].address, i, mm_rqs[i].size);


    memStatusProgram();

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)){
          printf("test_mm ERROR\n");
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        free(mm_rqs[i].address);
  }

  return 0;

}
