// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <testMM.h>
#include <stdlib.h>
#include <testUtil.h>
#include <programs.h>

#define MAX_BLOCKS 128

static void *myMemset(void *destination, int c, int len)
{
  uint8_t value = (uint8_t)c;
  char *origin = (char *)destination;
  while (len--)
  {
    origin[len] = value;
  }
  return destination;
}

void testMM(int argc, char argv[6][21])
{

  mm_rq mmrqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 2)
  {
    printf("Only one argument expected\n");
    exit();
  }

  if ((max_memory = atoi(argv[1])) <= 0)
  {
    printf("Invalid argument: please enter a valid integer\n");
    exit();
  }

  // max_memory=0.7*134217728;

  while (1)
  {
    rq = 0;
    total = 0;

    memStatusProgram();
    sleep(2000);

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory)
    {
      mmrqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mmrqs[rq].address = malloc(mmrqs[rq].size);

      if (mmrqs[rq].address)
      {
        total += mmrqs[rq].size;
      }
      rq++;
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mmrqs[i].address)
        myMemset(mmrqs[i].address, i, mmrqs[i].size);

    memStatusProgram();
    sleep(2000);

    // Check
    for (i = 0; i < rq; i++)
      if (mmrqs[i].address)
        if (!memcheck(mmrqs[i].address, i, mmrqs[i].size))
        {
          printf("testMM ERROR\n");
          exit();
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mmrqs[i].address)
        free(mmrqs[i].address);
  }

  exit();
}