#ifndef _programs_
#define _programs_

#include <stdlib.h>

void fibo(int argc, char argv[ARG_QTY][ARG_LEN]);
void primos(int argc, char argv[ARG_QTY][ARG_LEN]);
void help(int argc, char argv[ARG_QTY][ARG_LEN]);
void date(int argc, char argv[ARG_QTY][ARG_LEN]);
void invalid(int argc, char argv[ARG_QTY][ARG_LEN]);
void infoRegisters(int argc, char argv[ARG_QTY][ARG_LEN]);
void printMemory(int argc, char argv[ARG_QTY][ARG_LEN]);
void opcodeProgram();
void divzeroProgram();
void clearProgram();
void memStatusProgram();
void psProgram();
void killProgram(int argc, char args[ARG_QTY][ARG_LEN]);
void niceProgram(int argc, char args[ARG_QTY][ARG_LEN]);
void blockProgram(int argc, char args[ARG_QTY][ARG_LEN]);
void semProgram(int argc, char args[ARG_QTY][ARG_LEN]);
void testsem(int argc, char argv[ARG_QTY][ARG_LEN]);
void writePipeProgram(int argc, char argv[ARG_QTY][ARG_LEN]);
void readPipeProgram(int argc, char argv[ARG_QTY][ARG_LEN]);
void pipeListProgram(int argc, char args[ARG_QTY][ARG_LEN]);
void catProgram(int argc, char args[ARG_QTY][ARG_LEN]);
void wcProgram(int argc, char args[ARG_QTY][ARG_LEN]);
void filterProgram(int argc, char args[ARG_QTY][ARG_LEN]);
void loopProgram(int argc, char args[ARG_QTY][ARG_LEN]);
void nothingProgram(int argc, char args[ARG_QTY][ARG_LEN]);
void shmProgram(int argc, char args[ARG_QTY][ARG_LEN]);
#endif
