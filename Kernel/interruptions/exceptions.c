// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscalls.h>
#include <tools.h>
#include <naiveConsole.h>

#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6
#define TIMEOUT 3000

extern void ncPrint(const char *);

static void zero_division();
static void opcode();

void exceptionDispatcher(int exception)
{
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	else if (exception == OPCODE_EXCEPTION_ID)
		opcode();
}

static void zero_division()
{
	ncPrint("ZERO DIVISION ERROR!\n");
}

static void opcode()
{
	ncPrint("OPCODE EXCEPTION!\n");
}

void rebootTerm()
{
	sleep(TIMEOUT);
}
