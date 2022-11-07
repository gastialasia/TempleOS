// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <programs.h>
#include <stdint.h>
#include "./include/stdlib.h"

#define LIMIT64 0b0111111111111111111111111111111111111111111111111111111111111111 // 2^(63)-1

static char buff[300];

void help(int argc, char argv[6][21])
{
    printf("- fibonacci\n- primos\n- date\n- help\n- clear\n- mem\n- ps\n- kill [pid]\n- nice [pid] [priority]\n- block [pid]\n- sem\n- cat\n- wc\n- filter\n- pipe\n- phylo\n- testmm [max memory]\n- testproc [max processes]\n- testprio\n- testprio2 [prio1] [prio2] [prio3]\n- testsync\n- testnosync\n- testpipe\n");
}

void invalid(int argc, char argv[6][21])
{
    printf("Invalid command: try 'help'\n");
}

void date(int argc, char argv[6][21])
{
    int values[5] = {7, 8, 9, 4, 2}; // En orden: D, M, Y, H, M
    char buffer[3];                  // Cada numero de la fecha no va a tener más de dos digitos, 3 contando el cero null terminated
    for (int i = 0; i < 5; i++)
    {
        char num = getDateComponent(values[i]);
        uintToBase(num, buffer, 16);
        if (num < 0xA)
        {
            // Si el numero es de un solo digito, lo muevo un lugar a la derecha y a la izquierda le pongo un cero
            char aux = buffer[0];
            buffer[0] = '0';
            buffer[1] = aux;
        }
        buffer[2] = 0; // Al string del numero le agrego un cero null terminated asi puedo usar strcpy
        printf(buffer);
        if (i < 2)
        {
            printf("/");
        }
        else if (i == 3)
        {
            printf(":");
        }
        else
        {
            printf(" ");
        }
    }
    printf("UTC\n");
}

void fibo(int argc, char argv[6][21])
{
    uint64_t aux = 0, fibo1 = 0, fibo2 = 1;
    while (aux < LIMIT64)
    {
        aux = fibo1;
        fibo1 = fibo2;
        fibo2 = aux + fibo1;
        uintToBase(aux, buff, 10);
        printf(buff);
        putchar('\n');
        sleep(400);
    }
}

void primos(int argc, char argv[6][21])
{
    uint64_t j, limit, lastPrimo = 0;
    int isPrimo;
    for (uint64_t i = lastPrimo + 1; i <= LIMIT64; i++)
    {
        limit = i / 2;
        isPrimo = 1;
        for (j = 2; j <= limit && isPrimo; j++)
        {
            if (i % j == 0)
                isPrimo = 0;
        }
        if (isPrimo)
        {
            lastPrimo = i;
            uintToBase(lastPrimo, buff, 10);
            printf(buff);
            putchar('\n');
        }
        sleep(200);
    }
}

void infoRegisters(int argc, char argv[6][21])
{
    registersT regs;
    inforeg(&regs);
    printReg("rax", regs.rax);
    printReg("rbx", regs.rbx);
    printReg("rcx", regs.rcx);
    printReg("rdx", regs.rdx);
    printReg("rbp", regs.rbp);
    printReg("rdi", regs.rdi);
    printReg("rsi", regs.rsi);
    printReg("r8", regs.r8);
    printReg("r9", regs.r9);
    printReg("r10", regs.r10);
    printReg("r11", regs.r11);
    printReg("r12", regs.r12);
    printReg("r13", regs.r13);
    printReg("r14", regs.r14);
    printReg("r15", regs.r15);
}

void printMemory(int argc, char argv[6][21])
{
    char pos[8] = {0};
    printf("Ingrese 8 caracteres en hexa\n");
    scanf(pos);
    int ok = 1;
    uint64_t pointer = hex2int(pos, &ok);
    if (ok)
    {
        char buffer[32];
        char data[3];
        printMem(pointer, buffer);
        for (int i = 0; i < 32; i++)
        {
            uintToBase(buffer[i], data, 16);
            printf(data);
            putchar(' ');
        }
    }
    else
    {
        printf("Invalid dir");
    }
    putchar('\n');
}

void clearProgram(int argc, char argv[6][21])
{
    clear();
}

void memStatusProgram(int argc, char argv[6][21])
{
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
}

void psProgram(int argc, char argv[6][21])
{
    ps();
}

void killProgram(int argc, char args[6][21])
{
    if (argc != 2)
    {
        printf("Invalid argunment quantity\n");
        return;
    }
    char *pidPtr = args[1];
    uint32_t pid = atoi(pidPtr);
    if (pid == 1)
    {
        printf("Shell can't be killed\n");
        return;
    }
    if (!kill(pid))
    {
        printf("No such process with pid ");
        printf(pidPtr);
        putchar('\n');
    }
}

void niceProgram(int argc, char args[6][21])
{
    if (argc != 3)
    {
        printf("Invalid argunment quantity\n");
        return;
    }
    uint32_t pid = atoi(args[1]);

    uint8_t newPriority = (uint8_t)atoi(args[2]);

    if (newPriority <= 1 || newPriority > 10)
    {
        printf("Invalid priority\n");
        return;
    }

    if (pid == 1)
    {
        printf("Shell's priority can't be modified\n");
        return;
    }

    if (nice(pid, newPriority) == 2)
    {
        printf("There is no process with ID ");
        printInt(pid);
        putchar('\n');
    }
}

void blockProgram(int argc, char args[6][21])
{
    if (argc != 2)
    {
        printf("Invalid argunment quantity\n");
        return;
    }
    uint32_t pid = atoi(args[1]);
    if (pid == 1)
    {
        printf("Shell can't be blocked\n");
        return;
    }
    if (!block(pid))
    {
        printf("Error on changing process state\n");
    }
}

void semProgram(int argc, char args[6][21])
{
    if (argc != 1)
    {
        printf("sem program does not require arguments\n");
        return;
    }
    char buffer[2000] = {0};
    getAllSems(buffer);
    printf(buffer);
}

void writePipeProgram(int argc, char argv[6][21])
{
    int i = 0;
    while (i < 3)
    {
        printf("I'm writing from the wpipe process\n");
        sleep(3000);
    }
    exit();
}

void readPipeProgram(int argc, char argv[6][21])
{
    char buf1[300];
    char buf2[300];
    int i, j;
    while (1)
    {
        scanf(buf1);
        for (i = 0, j = 0; i < 54; i++, j++)
            buf2[j] = buf1[i];
        strcat(buf2, " and I'm reading from rpipe process\n");
        printf(buf2);
        sleep(3000);
    }
    exit();
}

void pipeListProgram(int argc, char args[6][21])
{
    char buffer[2000] = {0};
    getAllPipes(buffer);
    printf(buffer);
}

void catProgram(int argc, char args[6][21])
{
    char buffer[100];
    while (1)
    {
        scanf(buffer);
        printf(buffer);
        putchar('\n');
    }
    exit();
}

void wcProgram(int argc, char args[6][21])
{
    char buffer[300];
    int lines = 0;
    while (scanf(buffer) > 0)
    {
        lines++;
    }
    printf("Input lines: ");
    printInt(lines);
    putchar('\n');
    exit();
}

void filterProgram(int argc, char args[6][21])
{
    char read[100];
    char res[100];
    scanf(read);
    int len = strlen(read);
    int k = 0;
    int i;
    for (i = 0; i < len; i++)
    {
        char c = read[i];
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' && c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U')
        {
            res[k++] = read[i];
        }
    }
    res[k] = 0;
    printf(res);
    putchar('\n');
    exit();
}

void loopProgram(int argc, char args[6][21])
{
    while (1)
    {
        printf("I'm the process with PID ");
        printInt(getpid());
        putchar('\n');
        sleep(2000);
    }
    exit();
}