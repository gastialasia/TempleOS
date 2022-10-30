#include "../include/test_util.h"
#include "../include/stdlib.h"
#include "../include/test_processes.h"

int64_t test_processes(uint64_t argc, char *argv[]){
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char * argvAux[] = {0};

  // printf("checkpoint1\n");

  if (argc != 2) return -1;

  // printf("checkpoint2\n");

  printf(argv[1]);
  sleep(10000);

  if ((max_processes = atoi(argv[1])) <= 0) return -1;

  // printf("checkpoint3\n");

  p_rq p_rqs[max_processes];

  while (1){

    printf("Iiiii loquita sapee\n");

    // Create max_processes processes
    for(rq = 0; rq < max_processes; rq++){
      p_rqs[rq].pid = createProcess(endless_loop, 3, 0, "loop", NULL, NULL);

      if (p_rqs[rq].pid == -1){
        printf("test_processes: ERROR creating process\n");
        return -1;
      }else{
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0){

      for(rq = 0; rq < max_processes; rq++){
        action = GetUniform(100) % 2; 

        switch(action){
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED){
              if (kill(p_rqs[rq].pid) == -1){  
                printf("test_processes: ERROR killing process\n");
                return -1;
              }
              p_rqs[rq].state = KILLED; 
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING){
              if(block(p_rqs[rq].pid) == -1){
                printf("test_processes: ERROR blocking process\n");
                return -1;
              }
              p_rqs[rq].state = BLOCKED; 
            }
            break;
        }
      }

      // Randomly unblocks processes
      for(rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2){
          if(block(p_rqs[rq].pid) == -1){
            printf("test_processes: ERROR unblocking process\n");
            return -1;
          }
          p_rqs[rq].state = RUNNING; 
        }
    } 
  }
}
