#include <testUtil.h>
#include <stdlib.h>
#include <testProcesses.h>

int testProcesses(int argc, char argv[6][21])
{
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t maxProcesses;

  if (argc != 2)
    return -1;

  if ((maxProcesses = atoi(argv[1])) <= 0)
    return -1;

  p_rq p_rqs[maxProcesses];

  char aux[6][21];
  strcpy(aux[0], "_loop");

  while (1)
  {

    // Create maxProcesses processes
    for (rq = 0; rq < maxProcesses; rq++)
    {
      p_rqs[rq].pid = createProcess((uint64_t)endlessLoop, 3, 1, aux, NULL, NULL);

      if (p_rqs[rq].pid == -1)
      {
        printf("testProcesses: ERROR creating process\n");
        return -1;
      }
      else
      {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    ps();
    putchar('\n');
    sleep(2000);

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0)
    {

      for (rq = 0; rq < maxProcesses; rq++)
      {
        action = GetUniform(100) % 2;

        switch (action)
        {
        case 0:
          if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED)
          {
            if (kill(p_rqs[rq].pid) == -1)
            {
              printf("testProcesses: ERROR killing process\n");
              return -1;
            }
            p_rqs[rq].state = KILLED;
            alive--;
          }
          break;

        case 1:
          if (p_rqs[rq].state == RUNNING)
          {
            if (block(p_rqs[rq].pid) == -1)
            {
              printf("testProcesses: ERROR blocking process\n");
              return -1;
            }
            p_rqs[rq].state = BLOCKED;
          }
          break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < maxProcesses; rq++)
      {
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2)
        {
          if (block(p_rqs[rq].pid) == -1)
          {
            printf("testProcesses: ERROR unblocking process\n");
            return -1;
          }
          p_rqs[rq].state = RUNNING;
        }
      }

      ps();
      putchar('\n');
      sleep(2000);
    }
  }
}
