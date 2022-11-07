#ifndef _programs_
#define _programs_

void fibo(int argc, char argv[6][21]);
void primos(int argc, char argv[6][21]);
void help(int argc, char argv[6][21]);
void date(int argc, char argv[6][21]);
void invalid(int argc, char argv[6][21]);
void infoRegisters(int argc, char argv[6][21]);
void printMemory(int argc, char argv[6][21]);
void opcodeProgram();
void divzeroProgram();
void clearProgram();
void memStatusProgram();
void psProgram();
void killProgram(int argc, char args[6][21]);
void niceProgram(int argc, char args[6][21]);
void blockProgram(int argc, char args[6][21]);
void semProgram(int argc, char args[6][21]);
void testsem(int argc, char argv[6][21]);
void writePipeProgram(int argc, char argv[6][21]);
void readPipeProgram(int argc, char argv[6][21]);
void pipeListProgram(int argc, char args[6][21]);
void catProgram(int argc, char args[6][21]);
void wcProgram(int argc, char args[6][21]);
void filterProgram(int argc, char args[6][21]);
void loopProgram(int argc, char args[6][21]);
void nothingProgram(int argc, char args[6][21]);
#endif
