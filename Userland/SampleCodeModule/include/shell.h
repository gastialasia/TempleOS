#ifndef _shell_
#define _shell_

typedef int (*function_type)(void);

void shell(void);
void parser(const char *buffer);
void simpleScreenWrapper(char(*fn)(void));
void SplitScreenWrapper(char(*fn1)(void),char(*fn2)(void));
function_type getFuncFromString(char*str, int *);
void returnToSingleScreen();
#endif
