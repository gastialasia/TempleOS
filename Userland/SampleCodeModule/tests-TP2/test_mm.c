#include "../include/test_mm.h"
#include "../include/stdlib.h"
#include "../include/test_util.h"

#define MAX_BLOCKS 128

char test_mm(){ //test_mm(uint64_t argc, char *argv[])

  // mm_rq mm_rqs[MAX_BLOCKS];
  // uint8_t rq;
  // uint32_t total;
  // uint64_t max_memory;

  // //if (argc != 1) return -1;

  // //if ((max_memory = satoi(argv[0])) <= 0) return -1;
  // max_memory=120795955;

  // while (1){
  //   rq = 0;
  //   total = 0;

  //   // Request as many blocks as we can
  //   while(rq < MAX_BLOCKS && total < max_memory){
  //     mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
  //     mm_rqs[rq].address = malloc(mm_rqs[rq].size);

  //     if(mm_rqs[rq].address){
  //       total += mm_rqs[rq].size;
  //       rq++;
  //     }
  //   }

  //   // Set
  //   uint32_t i;
  //   for (i = 0; i < rq; i++)
  //     if (mm_rqs[i].address)
  //       memset(mm_rqs[i].address, i, mm_rqs[i].size);

  //   // Check
  //   for (i = 0; i < rq; i++)
  //     if (mm_rqs[i].address)
  //       if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)){
  //         printf("test_mm ERROR\n");
  //         return -1;
  //       }

  //   // Free
  //   for (i = 0; i < rq; i++)
  //     if (mm_rqs[i].address)
  //       free(mm_rqs[i].address);
  // }
  // unsigned int bytes = 128;
  // test_mm(bytes);
  unsigned int vec[3];
  //memStatus(vec);
  uint64_t aux = vec[0];
  char buff[10];
  uintToBase(aux,buff,10);
  printf(buff);
  printf("Siuu\n");
  sleep(5000);
  return 0;
}
