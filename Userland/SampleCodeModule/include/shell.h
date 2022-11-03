#ifndef _shell_
#define _shell_

void shell(void);
void parser(const char *buffer);
function_type getFuncFromString(char*str, int *);
void returnToSingleScreen();
#endif
