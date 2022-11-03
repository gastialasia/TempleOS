#include <stdlib.h>
#include <philosophers.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N
#define GENERALSEMID 100

void printTable();
 
int state[N];
 
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
        createProcess((uint64_t)philosopher, 3, 2, args, NULL, NULL);
    }
    exit();
    return 0;
}

void printTable(){
    int i;
    for(i=0; i<N; i++){
        if(state[i]==EATING){
            putchar('E');
        } else {
            putchar('.');
        }
    }
    putchar('\n');
}