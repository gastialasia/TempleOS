#include "../include/test_util.h"
#include "../include/stdlib.h"
#include "../include/test_processes.h"

int64_t test_processes(uint64_t argc, char argv[6][21]){
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char * argvAux[] = {0};

  if (argc != 2) return -1;

  if ((max_processes = satoi(argv[1])) <= 0) return -1;

  p_rq p_rqs[max_processes];

  char aux[6][21];
  strcpy(aux[0], "_loop");

  while (1){

    // Create max_processes processes
    for(rq = 0; rq < max_processes; rq++){
      p_rqs[rq].pid = createProcess(endless_loop, 3, 1, aux, NULL, NULL);

      if (p_rqs[rq].pid == -1){
        printf("test_processes: ERROR creating process\n");
        return -1;
      }else{
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    ps();

    sleep(3000);

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
      for(rq = 0; rq < max_processes; rq++){
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2){
          if(block(p_rqs[rq].pid) == -1){
            printf("test_processes: ERROR unblocking process\n");
            return -1;
          }
          p_rqs[rq].state = RUNNING; 
        }
      }

      ps();

        sleep(3000);
    } 
  }
}
