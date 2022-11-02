#ifndef _programs_
#define _programs_

int fibo(int argc, char argv[6][21]);
int primos(int argc, char argv[6][21]);
int help(int argc, char argv[6][21]);
int date(int argc, char argv[6][21]);
int invalid(int argc, char argv[6][21]);
int infoRegisters(int argc, char argv[6][21]);
int printMemory(int argc, char argv[6][21]);
int opcodeProgram();
int divzeroProgram();
int clearProgram();
int memStatusProgram();
int psProgram();
int killProgram(int argc, char args[6][21]);
int niceProgram(int argc, char args[6][21]);
int blockProgram(int argc, char args[6][21]);
void semProgram(int argc, char args[6][21]);
int testsem(int argc, char argv[6][21]);
void writePipeProgram(int argc, char argv[6][21]);
void readPipeProgram(int argc, char argv[6][21]);
void pipeListProgram(int argc, char args[6][21]);

#endif
