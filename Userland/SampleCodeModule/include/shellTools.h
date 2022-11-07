#include <stdint.h>

typedef int (*functionType)(int argc, char argv[6][21]);

void parser(const char *buffer);
functionType getFuncFromString(char *str, int *);