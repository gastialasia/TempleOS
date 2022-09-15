// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdlib.h>
#include <shell.h>
#include <programs.h>
#include <stdint.h>

#define LENGTH 100
#define MAXDIGITS 21
#define MAXBUFFER 100

static char command1[MAXBUFFER] = "null";
static char command2[MAXBUFFER] = "null";

int power = 1; // 1 means running, 0 means shutting down the system

void shell(void)
{

    // Chequeo si había un programa corriendo antes
    char *prevCommand1 = getProgram(0);
    char *prevCommand2 = getProgram(1);

    if (strcmp(prevCommand1,"null") || strcmp(prevCommand2,"null"))
    {
        function_type prog1 = getFuncFromString(prevCommand1);
        function_type prog2 = getFuncFromString(prevCommand2);
        SplitScreenWrapper(prog1, prog2);
    }
    setScreenMode(1);
    clear();
    char buffer[LENGTH];
    while (power)
    {
        printf("User:$ ");
        int length = scanf(buffer);
        buffer[length - 1] = 0;
        parser(buffer);
        // Acá va el procesamiento de lo que recibio la funcion para ejecutar un programita
    }
}

void parser(char *buffer)
{
    int i = 0, j = 0, k = 0;
    char commands[2][100];
    int flag = 0;
    while (buffer[i] != 0)
    { // faltan chequeos
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
        strcpy(command1, commands[0]);
        strcpy(command2, commands[1]);
        storeProgram(command1, command2);
        function_type prog1 = getFuncFromString(commands[0]);
        function_type prog2 = getFuncFromString(commands[1]);
        SplitScreenWrapper(prog1, prog2);
    }
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
    else if (!strcmp("printmem", str))
    {
        toRet = &printMemory;
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
        if(getlast() == 17){ //F1
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

void SplitScreenWrapper(char (*fn1)(void), char (*fn2)(void))
{
    char isRunning1 = 1;
    char isRunning2 = 1;
    while (isRunning1 || isRunning2)
    {
        if (getlast() == 'q')
        {
            setScreenMode(1);
            returnToSingleScreen();
            storeProgram("null", "null");
            return;
        }
        if(getlast() == 17){ //F1
            takeSnapShot();
        }
        sleep(300);
        if (isRunning1)
        {
            setScreenMode(2);
            isRunning1 = fn1();
            if (isRunning1 == 0)
            {
                storeProgram("null", command2); // En caso de ya haber terminado el programa, lo borramos de los comandos guardados en el kernel
            }
            if (getlast() == 'l')
            {
                isRunning1 = 0;
            }
            sleep(300);
            // Hay que chequear porque entra una vez mas de las que deberia
        }
        if (isRunning2)
        {
            setScreenMode(3);
            isRunning2 = fn2();
            if (getlast() == 'r')
            {
                isRunning2 = 0;
            }
        }
    }
    printf("Presione 'q' para volver a la shell");
    while (getlast() != 'q');
    setScreenMode(1);
    returnToSingleScreen();
    storeProgram("null", "null");
}
