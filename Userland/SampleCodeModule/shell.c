// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/stdlib.h"
#include <shell.h>
#include <programs.h>
#include <stdint.h>
#include <test_mm.h>

#define LENGTH 100
#define MAXDIGITS 21
#define MAXBUFFER 100
#define F1_KEY 17
#define NULL ((void *) 0)
#define FOREGROUND 1
#define BACKGROUND 2

int power = 1; // 1 means running, 0 means shutting down the system

int pipeParser(const char *buffer, char commands[2][100]);
int tokenizeCommand(const char command[100], char tokens[5][50]);

void shell(void)
{
    clear();
    char buffer[LENGTH];
    while (power)
    {
        printf("User:$ ");
        int length = scanf(buffer);
        parser(buffer);
    }
}

void parser2(char *buffer)
{
    int i = 0, j = 0, k = 0;
    char commands[2][100];
    int flag = 0;
    while (buffer[i] != 0)
    {
        if (buffer[i] == '|')
        {
            commands[j][k] = 0;
            j++;
            k = 0;
            flag = 1;
        }
        else if (buffer[i] != ' ')
        {
            commands[j][k++] = buffer[i];
        }

        i++;
    }
    commands[j][k] = 0;
    if (flag == 0)
    {
        // No hay pipe
        simpleScreenWrapper(getFuncFromString(commands[0]));
    }
    else
    {
        // El flag es 1, entonces hay un pipe
        function_type prog1 = getFuncFromString(commands[0]);
        function_type prog2 = getFuncFromString(commands[1]);
        // openPipe(prog1, prog2); TO DO
    }
}

void parser(const char *buffer){
    char commands[2][100];
    int hasPipe = pipeParser(buffer, commands);

    char tokens1[5][50]={{0}};
    char tokens2[5][50]={{0}};
    int tokenQty1 = tokenizeCommand(commands[0], tokens1);

    function_type fun1, fun2;

    fun1 = getFuncFromString(tokens1[0]);

    int priority = FOREGROUND;

    if (!strcmp(tokens1[tokenQty1-1],"&")){
        priority = BACKGROUND;
        tokenQty1--; // Decremento nro de argumentos porque el & no cuenta
    }

    if (priority==FOREGROUND){
        printf("Lanzo proceso en foreground\n");
    } else {
        printf("Lanzo proceso en background\n");
    }

    fun1();

    //createProcess(fun1, priority, tokenQty1, tokens1, NULL, NULL);

    if(hasPipe){
        tokenizeCommand(commands[1], tokens2);
        fun2 = getFuncFromString(tokens2[1]);
    }
    
}

//receives buffer line and array to fill with commands separated and returns 1 if there was a pipe, 0 otherwise
int pipeParser(const char *buffer, char commands[2][100]){
    int i = 0, j = 0, k = 0;
    int flag = 0;
    while (buffer[i] != 0){
        if (buffer[i] == '|')
        {
            commands[j][k] = 0;
            j++;
            k = 0;
            flag = 1;
        }
        else
        {
            commands[j][k++] = buffer[i];
        }
        i++;
    }
    commands[j][k] = 0;
    return flag;
}

//Returns token count
int tokenizeCommand(const char command[100], char tokens[5][50]){
    int i = 0;
    strcpy(tokens[i++], strtok(command, " "));
    char *token=' ';
    while(token)
    {
        token = strtok(NULL, " ");
        if(token!=NULL){
            strcpy(tokens[i++], token);
        }
    }
    return i;
}


function_type getFuncFromString(char *str)
{
    function_type toRet;
    if (!strcmp("date", str))
    {
        toRet = &date;
    }
    else if (!strcmp("help", str))
    {
        toRet = &help;
    }
    else if (!strcmp("fibo", str) || !strcmp("fibonacci", str))
    {
        toRet = &fibo;
    }
    else if (!strcmp("primos", str))
    {
        toRet = &primos;
    }
    else if (!strcmp("opcode", str))
    {
        toRet = &opcodeProgram;
    }
    else if (!strcmp("divzero", str))
    {
        toRet = &divzeroProgram;
    }
    else if (!strcmp("clear", str))
    {
        toRet = &clearProgram;
    }
    else if (!strcmp("inforeg", str))
    {
        toRet = &infoRegisters;
    }
    else if (!strcmp("testmm", str))
    {
        toRet = &test_mm;
    }
    else if (!strcmp("printmem", str))
    {
        toRet = &printMemory;
    }
    else if (!strcmp("mem", str))
    {
        toRet = &memStatusProgram;
    }
    else if (!strcmp("exit", str))
    {
        clear();
        power = 0;
        toRet = &nullProgram;
    }
    else if (strlen(str) == 0 || !strcmp("null", str))
    {
        toRet = &nullProgram;
    }
    else
    {
        toRet = &invalid;
    }
    return toRet;
}

void simpleScreenWrapper(char (*fn)(void))
{
    // char buffer[MAXBUFFER];
    char isRunning = fn();
    // printf(buffer);
    while (isRunning)
    {
        // printf(buffer);
        sleep(300);
        if (getlast() == 'q')
        {
            returnToSingleScreen();
            return;
        }
        if(getlast() == F1_KEY){
            takeSnapShot();
        }
        isRunning = fn();
    }
}

void returnToSingleScreen()
{
    clear();
    reset_primo();
    reset_fibo();
    sleep(500);
}
