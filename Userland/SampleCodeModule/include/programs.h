#ifndef _programs_
#define _programs_

int fibo(void);
int primos(void);
int help(void);
int date(void);
int invalid(void);
int infoRegisters(void);
int printMemory(void);
int opcodeProgram();
int divzeroProgram();
int clearProgram();
int memStatusProgram();
int psProgram();
int killProgram(int argc, char args[6][21]);
int niceProgram(int argc, char args[6][21]);
int blockProgram(int argc, char args[6][21]);
void semProgram(int argc, char args[6][21]);
void testsem();

#endif
