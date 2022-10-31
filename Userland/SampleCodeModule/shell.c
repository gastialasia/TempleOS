// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/stdlib.h"
#include <shell.h>
#include <programs.h>
#include <stdint.h>
#include <test_mm.h>
#include <test_processes.h>
#include <test_prio.h>
#include <test_sync.h>

#define LENGTH 100
#define MAXBUFFER 100
#define F1_KEY 17
#define NULL ((void *) 0)
#define FOREGROUND 1
#define BACKGROUND 2
#define ARG_LEN 21
#define ARG_QTY 6

int pipeParser(const char *buffer, char commands[2][100]);
int tokenizeCommand(const char command[100], char tokens[ARG_QTY][ARG_LEN]);

void shell(void)
{
    clear();
    char buffer[LENGTH];
    while (1)
    {
        printf("User:$ ");
        int length = scanf(buffer);
        if (length!=0){
            parser(buffer);
            sleep(1); //retraso de 1ms para evitar que el $user se imprima antes de blockear la shell
        }
    }
}

void parser(const char *buffer){
    char commands[2][100];
    int hasPipe = pipeParser(buffer, commands);

    char tokens1[ARG_QTY][ARG_LEN]={{0}};
    char tokens2[ARG_QTY][ARG_LEN]={{0}};
    int tokenQty1 = tokenizeCommand(commands[0], tokens1);

    function_type fun1, fun2;

    int isBuiltIn = 0;

    fun1 = getFuncFromString(tokens1[0], &isBuiltIn);

    if(!isBuiltIn){
        int priority = FOREGROUND;

        if (!strcmp(tokens1[tokenQty1-1],"&")){
            priority = BACKGROUND;
            tokenQty1--; // Decremento nro de argumentos porque el & no cuenta
        }
        createProcess(fun1, priority, tokenQty1, tokens1, NULL, NULL);
    } else {
        fun1(tokenQty1, tokens1);
    }

    isBuiltIn=0;

    if(hasPipe){
        tokenizeCommand(commands[1], tokens2);
        fun2 = getFuncFromString(tokens2[1], &isBuiltIn);
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
int tokenizeCommand(const char command[100], char tokens[ARG_QTY][ARG_LEN]){
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


function_type getFuncFromString(char *str, int * isBuiltIn)
{
    function_type toRet;
    if (!strcmp("date", str))
    {
        toRet = &date;
        *isBuiltIn=1;
    }
    else if (!strcmp("help", str))
    {
        toRet = &help;
        *isBuiltIn=1;
    }
    else if (!strcmp("fibo", str) || !strcmp("fibonacci", str))
    {
        toRet = &fibo;
        *isBuiltIn=1;
    }
    else if (!strcmp("primos", str))
    {
        toRet = &primos;
        *isBuiltIn=1;
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
        *isBuiltIn=1;
    }
    else if (!strcmp("inforeg", str))
    {
        toRet = &infoRegisters;
        *isBuiltIn=1;
    }
    else if (!strcmp("testmm", str))
    {
        toRet = &test_mm;
    }
    else if (!strcmp("testproc", str))
    {
        toRet = &test_processes;
    }
    else if (!strcmp("testprio", str))
    {
        toRet = &test_prio;
    }
    else if (!strcmp("testsync", str))
    {
        toRet = &test_sync;
    }
    else if (!strcmp("printmem", str))
    {
        toRet = &printMemory;
        *isBuiltIn=1;
    }
    else if (!strcmp("mem", str))
    {
        toRet = &memStatusProgram;
        *isBuiltIn=1;
    }
    else if (!strcmp("ps", str))
    {
        toRet = &psProgram;
        *isBuiltIn=1;
    }
    else if (!strcmp("kill", str))
    {
        toRet = &killProgram;
        *isBuiltIn=1;
    }
    else if (!strcmp("nice", str))
    {
        toRet = &niceProgram;
        *isBuiltIn=1;
    }
    else if (!strcmp("block", str))
    {
        toRet = &blockProgram;
        *isBuiltIn=1;
    }
    else
    {
        toRet = &invalid;
        *isBuiltIn=1;
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
    sleep(500);
}