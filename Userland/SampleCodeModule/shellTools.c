#include <programs.h>
#include <stdint.h>
#include <testMM.h>
#include <testProcesses.h>
#include <testPriorities.h>
#include <testSync.h>
#include <philosophers.h>
#include <testPriorities2.h>
#include <testPipe.h>
#include <shell.h>
#include <stdlib.h>
#include <shellTools.h>
#include <testArgs.h>

#define MAXBUFFER 100
#define F1_KEY 17
#define FOREGROUND 1
#define BACKGROUND 2

int pipeParser(const char *buffer, char commands[2][100]);
int tokenizeCommand(char command[100], char tokens[ARG_QTY][ARG_LEN]);

void parser(const char *buffer)
{
    char commands[2][100];
    int hasPipe = pipeParser(buffer, commands);

    int priority = FOREGROUND;

    char tokens1[ARG_QTY][ARG_LEN] = {{0}};
    int tokenQty1 = tokenizeCommand(commands[0], tokens1);

    functionType fun1, fun2;

    int isBuiltIn = 0;

    if (tokenQty1>6){
        printf("Error: Maximum 5 arguments\n");
        return;
    }

    fun1 = getFuncFromString(tokens1[0], &isBuiltIn);
    int tokenQty2;

    if (hasPipe)
    {
        char tokens2[ARG_QTY][ARG_LEN] = {{0}};
        tokenQty2 = tokenizeCommand(commands[1], tokens2);
        fun2 = getFuncFromString(tokens2[0], &isBuiltIn);

        if (tokenQty2>6){
        printf("Error: Maximum 5 arguments\n");
        return;
    }

        if (fun1 == invalid || fun2 == invalid)
        {
            invalid(0, NULL);
        }
        else
        {
            if (!strcmp(tokens1[tokenQty2 - 1], "&"))
            {
                priority = BACKGROUND;
                tokenQty2--;
            }

            fd *fd1 = createFd();
            fd *fd2 = createFd();
            createPipe(fd1, fd2);
            createProcess((uint64_t)fun1, BACKGROUND, 1, tokens1, NULL, fd2);
            createProcess((uint64_t)fun2, tokenQty2, 1, tokens2, fd1, NULL);
        }
    }
    else
    {
        if (!isBuiltIn)
        {

            if (!strcmp(tokens1[tokenQty1 - 1], "&"))
            {
                priority = BACKGROUND;
                tokenQty1--;
            }

            createProcess((uint64_t)fun1, priority, tokenQty1, tokens1, NULL, NULL);
        }
        else
        {
            fun1(tokenQty1, tokens1);
        }
    }
}

// receives buffer line and array to fill with commands separated and returns 1 if there was a pipe, 0 otherwise
int pipeParser(const char *buffer, char commands[2][100])
{
    int i = 0, j = 0, k = 0;
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
        else
        {
            commands[j][k++] = buffer[i];
        }
        i++;
    }
    commands[j][k] = 0;
    return flag;
}

int tokenizeCommand(char command[100], char tokens[ARG_QTY][ARG_LEN])
{
    int i = 0;
    strcpy(tokens[i++], strtok(command, " "));
    char *token = " ";
    while (token)
    {
        token = strtok(NULL, " ");
        if (token != NULL)
        {
            strcpy(tokens[i++], token);
        }
    }
    return i;
}

functionType getFuncFromString(char *str, int *isBuiltIn)
{
    functionType toRet;
    if (!strcmp("date", str))
    {
        toRet = &date;
        *isBuiltIn = 1;
    }
    else if (!strcmp("help", str))
    {
        toRet = &help;
        *isBuiltIn = 1;
    }
    else if (!strcmp("fibo", str) || !strcmp("fibonacci", str))
    {
        toRet = &fibo;
        *isBuiltIn = 1;
    }
    else if (!strcmp("primos", str))
    {
        toRet = &primos;
        *isBuiltIn = 1;
    }
    else if (!strcmp("clear", str))
    {
        toRet = &clearProgram;
        *isBuiltIn = 1;
    }
    else if (!strcmp("testmm", str))
    {
        toRet = &testMM;
    }
    else if (!strcmp("testproc", str))
    {
        toRet = &testProcesses;
    }
    else if (!strcmp("testprio", str))
    {
        toRet = &testPriorities;
    }
    else if (!strcmp("testsync", str))
    {
        toRet = &testSync;
    }
    else if (!strcmp("testnosync", str))
    {
        toRet = &testNoSync;
    }
    else if (!strcmp("mem", str))
    {
        toRet = &memStatusProgram;
        *isBuiltIn = 1;
    }
    else if (!strcmp("ps", str))
    {
        toRet = &psProgram;
        *isBuiltIn = 1;
    }
    else if (!strcmp("kill", str))
    {
        toRet = &killProgram;
        *isBuiltIn = 1;
    }
    else if (!strcmp("nice", str))
    {
        toRet = &niceProgram;
        *isBuiltIn = 1;
    }
    else if (!strcmp("block", str))
    {
        toRet = &blockProgram;
        *isBuiltIn = 1;
    }
    else if (!strcmp("sem", str))
    {
        toRet = &semProgram;
        *isBuiltIn = 1;
    }
    else if (!strcmp("pipe", str))
    {
        toRet = &pipeListProgram;
        *isBuiltIn = 1;
    }
    else if (!strcmp("wpipe", str))
    {
        toRet = &writePipeProgram;
    }
    else if (!strcmp("rpipe", str))
    {
        toRet = &readPipeProgram;
    }
    else if (!strcmp("cat", str))
    {
        toRet = &catProgram;
    }
    else if (!strcmp("wc", str))
    {
        toRet = &wcProgram;
    }
    else if (!strcmp("filter", str))
    {
        toRet = &filterProgram;
    }
    else if (!strcmp("loop", str))
    {
        toRet = &loopProgram;
    }
    else if (!strcmp("philo", str)||!strcmp("phylo", str))
    {
        toRet = &philosophersProgram;
    }
    else if (!strcmp("testprio2", str))
    {
        toRet = &testPriorities2;
    }
    else if (!strcmp("testpipe", str))
    {
        toRet = &testMM;
    } else if (!strcmp("testargs", str)) {
      toRet = &test_args;
    } else {
      toRet = &invalid;
      *isBuiltIn = 1;
    }
    return toRet;
}