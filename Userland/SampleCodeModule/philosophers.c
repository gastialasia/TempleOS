#include "./include/stdlib.h"
#include <philosophers.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + n-1) % n
#define RIGHT (phnum + 1) % n
#define GENERALSEMID 100
#define MAX_PHILOSOPHERS 10

void printTable();
 
int state[MAX_PHILOSOPHERS];
int pids[MAX_PHILOSOPHERS];
int n=N;

Semaphore *mutex;
Semaphore *chopsticks[N];
 
void test(int phnum)
{
    if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[phnum] = EATING;
        printTable();
        sleep(2);
        semPost(chopsticks[phnum]);
    }
}

void take_fork(int phnum) {

    semWait(mutex);

    state[phnum] = HUNGRY;

    test(phnum);

    semPost(mutex);

    semWait(chopsticks[phnum]);

    sleep(1);
}
 
void put_fork(int phnum) {
 
    semWait(mutex);
 
    state[phnum] = THINKING;
 
    test(LEFT);
    test(RIGHT);
 
    semPost(mutex);
}
 
void* philosopher(int argc, char argv[6][21])
{
    if(argc != 2){
        exit();
    }

    int num = atoi(argv[1]);
    while (1) {

        sleep(1000);
 
        take_fork(num);
 
        sleep(1000);
 
        put_fork(num);

    }
}
 
int philosophersProgram(){
    int i;
    mutex = semOpen(GENERALSEMID, 1);
    
    for(i=0; i<N; i++){
        chopsticks[i]=semOpen(i, 1);
    }
 
    char args[6][21];
    char num[3];
    strcpy(args[0],"philo");
    for (i = 0; i < N; i++) {
        uintToBase(i, num, 10);
        strcpy(args[1], num);
        pids[i] = createProcess((uint64_t)philosopher, 3, 2, args, NULL, NULL);
    }
    
    int c;
    while(1){
        getchar(&c);
        switch (c){
            case 'a':   
            case 'A':
                if(n==MAX_PHILOSOPHERS){
                    printf("The table is full, no new philosophers can join\n");
                } else {
                    printf("A philosopher just came and sat on the table\n");
                    uintToBase(n, num, 10);
                    strcpy(args[1], num);
                    chopsticks[n] = semOpen(n, i);
                    if(chopsticks[n] != NULL)
                        pids[n++] = createProcess((uint64_t)philosopher, 3, 2, args, NULL, NULL);
                }
                break;
            case 'r':
            case 'R':
                printf("A philosopher left the rift\n");
                break;
            default:
                break;
        }
    }
    exit();
    return 0;
}

void printTable(){
    int i;
    for(i=0; i<n; i++){
        if(state[i]==EATING){
            putchar('E');
        } else {
            putchar('.');
        }
    }
    putchar('\n');
}