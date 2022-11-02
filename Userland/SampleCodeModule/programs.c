// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <programs.h>
#include <stdint.h>
#include "./include/stdlib.h">

#define LIMIT64 0b0111111111111111111111111111111111111111111111111111111111111111 //2^(63)-1

static char buff[300];

int help(int argc, char argv[6][21]){
    printf("-fibonacci\n-primos\n-date\n-opcode\n-divzero\n-help\n-clear\n-mem\n-ps\n-kill [pid])\n-nice [pid] [priority]\n-block [pid]\n-sem\n-cat [input]\n-wc [input]\n-filter [input]\n-pipe\n-phylo\n-testmm [max memory]\n-testproc [max processes]\n-testprio\n-testsync FALTA VER ARGUMENTOS\n-testsync FALTA VER ARGUMENTOS\n-testnosync\n");
    return 0;
}

int invalid(int argc, char argv[6][21]){
    printf("Invalid command: try 'help'\n");
    return 0;
}

int date(int argc, char argv[6][21]){
    int values[5] = {7,8,9,4,2}; //En orden: D, M, Y, H, M
    char buffer[3]; // Cada numero de la fecha no va a tener más de dos digitos, 3 contando el cero null terminated
    for (int i=0;i<5;i++){
        char num = getDateComponent(values[i]);
        uintToBase(num,buffer,16);
        if (num<0xA){
            // Si el numero es de un solo digito, lo muevo un lugar a la derecha y a la izquierda le pongo un cero
            char aux = buffer[0];
            buffer[0]='0';
            buffer[1]=aux;
        }
        buffer[2]=0; // Al string del numero le agrego un cero null terminated asi puedo usar strcpy
        printf(buffer);
        if (i<2){
            printf("/");
        } else if (i==3){
            printf(":");
        } else {
            printf(" ");
        }
    }
    printf("UTC\n");
    return 0;
}

int fibo(int argc, char argv[6][21]){
    // Deja en el buffer el numero convertido a string
    // Devuelve 1 si el programa sigue corriendo, 0 sino
    uint64_t aux=0, fibo1=0, fibo2=1;
    while(aux<LIMIT64){
        aux = fibo1;
        fibo1 = fibo2;
        fibo2 = aux+fibo1;
        uintToBase(aux,buff,10);
        printf(buff);
        putchar('\n');
        sleep(400);
    }
    return 0;
}

int primos(int argc, char argv[6][21]){ //Esta funcion es una criba de Eratosthenes casera
  // deja en el buffer el numeor convertido a string
  // devuelve 1 si el programa no termino, 0 si termino
    uint64_t j, limit, lastPrimo=0;
    int isPrimo;
    for(uint64_t i=lastPrimo+1; i <= LIMIT64; i++){
        limit = i/2;
        isPrimo=1;
        for(j=2; j <= limit && isPrimo; j++){
            if(i%j==0)
                isPrimo = 0;
        }
        if(isPrimo){
            lastPrimo = i;
            uintToBase(lastPrimo,buff,10);
            printf(buff);
            putchar('\n');
        }
        sleep(200);
    }
    return 0;
}

int infoRegisters(int argc, char argv[6][21]){
    registersT regs;
    inforeg(&regs);
    printReg("rax",regs.rax);
    printReg("rbx",regs.rbx);
    printReg("rcx",regs.rcx);
    printReg("rdx",regs.rdx);
    printReg("rbp",regs.rbp);
    printReg("rdi",regs.rdi);
    printReg("rsi",regs.rsi);
    printReg("r8",regs.r8);
    printReg("r9",regs.r9);
    printReg("r10",regs.r10);
    printReg("r11",regs.r11);
    printReg("r12",regs.r12);
    printReg("r13",regs.r13);
    printReg("r14",regs.r14);
    printReg("r15",regs.r15);
    return 0;
}

int printMemory(int argc, char argv[6][21]){
	char pos[8] ={0};
	printf("Ingrese 8 caracteres en hexa\n");
	scanf(pos);
	int ok = 1;
	uint64_t pointer = hex2int(pos,&ok);
	char buffer[32];
    char data[3];
    if(ok){
		printMem(pointer,buffer);
        for(int i=0; i < 32; i++){
            uintToBase(buffer[i],data,16);
            printf(data);
            putchar(' ');
        }
	}else {
	printf("Invalid dir");
	}
    putchar('\n');
	return 0;
}

int opcodeProgram(int argc, char argv[6][21]){
    opcode();
    return 0;
}

int divzeroProgram(int argc, char argv[6][21]){
    divzero();
    return 0;
}

int clearProgram(int argc, char argv[6][21]){
    clear();
    return 0;
}

int memStatusProgram(int argc, char argv[6][21]){
    unsigned int array[3];
    memStatus(array);
    printf("Total heap size: ");
    printInt(array[0]);
    printf("\n");
    printf("Heap left: ");
    printInt(array[1]);
    printf("\n");
    printf("Heap occupied: ");
    printInt(array[2]);
    printf("\n");
    return 0;
}

int psProgram(int argc, char argv[6][21]){
    ps();
    return 0;
}

int killProgram(int argc, char args[6][21]){
    if (argc!=2){
        printf("Invalid argunment quantity\n");
        return 0;
    }
    char * pidPtr = args[1];
    uint32_t pid = atoi(pidPtr);
    if (pid==1){
        printf("Shell can't be killed\n");
        return 0;
    }
    if (!kill(pid)) {
        printf("No such process with pid ");
        printf(pidPtr);
        putchar('\n');
    }
    return 0;
}

int niceProgram(int argc, char args[6][21]){
    if (argc!=3){
        printf("Invalid argunment quantity\n");
        return 0;
    }
    uint32_t pid = atoi(args[1]);

    uint8_t newPriority = (uint8_t)atoi(args[2]);

    if (pid==1){
        printf("Shell's priority can't be modified\n");
        return 0;
    }
    if (!nice(pid, newPriority)) {
        printf("Error on reasigning priority\n");
    }
    return 0;
}

int blockProgram(int argc, char args[6][21]){
    if (argc!=2){
        printf("Invalid argunment quantity\n");
        return 0;
    }
    uint32_t pid = atoi(args[1]);
    if (pid==1){
        printf("Shell can't be blocked\n");
        return 0;
    }
    if (!block(pid)) {
        printf("Error on changing process state\n");
    }
    return 0;
}

int semProgram(int argc, char args[6][21]){
    if(argc != 1){
        printf("sem program does not require arguments\n");
        return;
    }

    char buffer[2000]={0};
    getAllSems(buffer);
    printf(buffer);
}

//Creates a process which blocks waiting for sem with id 15
int testsem(int argc, char argv[6][21]){
    Semaphore * aux = semOpen(15, 0);
    if(aux == NULL){
        printf("Error on creating semaphore\n");
        exit();
    }
    semWait(aux);
}

//This function is supposed to write into readPipeTest process
void writePipeProgram(int argc, char argv[6][21]){
    while(1){
        printf("I'm writing into the pipe every 3s. I Hope there's someone reading in the other side\n\0");
        sleep(1000);
    }
}

void readPipeProgram(int argc, char argv[6][21]){
    char buf1[300];
    char buf2[300];
    int i, j;
    while(1){
        printf("Siiiiuuuuu\n");
        scanf(buf1);
        for(i=10, j=0; i<20; i++, j++)
            buf2[j]=buf1[i];
        buf2[j]=0;
        printf(buf2);
        sleep(1000);
    }
}