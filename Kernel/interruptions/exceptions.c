// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscalls.h>
#include <tools.h>

#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6
#define TIMEOUT 3000

extern void (*printCharPtr)(char*);
extern void (*printPtr)(char*);
extern void (*printHexPtr)(char*);
extern char * getScreenModePtr();
extern char ** getRunningProgramPtr(int index);

void killRunningProgram();

static void zero_division();
static void opcode();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	else if(exception == OPCODE_EXCEPTION_ID)
		opcode();
}

static void zero_division() {
	printPtr("ZERO DIVISION ERROR!\n");
	killRunningProgram();

}

static void opcode(){
	printPtr("OPCODE EXCEPTION!\n");
	killRunningProgram();
}

void killRunningProgram(){
	char screenMode = *getScreenModePtr();
	if (screenMode>=2) {
		*getRunningProgramPtr(screenMode-2)="null";
	}

}

void rebootTerm(){
	if (*getScreenModePtr()==1||(*getScreenModePtr()==3 && strcmp(*getRunningProgramPtr(0), "null"))){
		sleep(TIMEOUT);
	}
}
