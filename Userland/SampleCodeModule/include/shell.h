#ifndef _shell_
#define _shell_

typedef int (*function_type)(int argc, char argv[6][21]);

void shell(void);
void parser(const char *buffer);
function_type getFuncFromString(char*str, int *);
void returnToSingleScreen();
#endif
