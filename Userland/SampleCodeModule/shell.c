// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdlib.h>
#include <shell.h>
#include <shellTools.h>

int pipeParser(const char *buffer, char commands[2][100]);
int tokenizeCommand(char command[100], char tokens[ARG_QTY][ARG_LEN]);

void shell(void)
{
    clear();
    char buffer[COMMAND_LEN];
    while (1)
    {
        printf("~User~:$ ");
        int length = scanf(buffer);
        if (length != 0)
        {
            parser(buffer);
            sleep(1); // Retraso de 1ms para evitar que el $user se imprima antes de bloquear la shell
        }
    }
}
