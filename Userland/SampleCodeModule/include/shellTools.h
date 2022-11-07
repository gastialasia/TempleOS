#include <stdint.h>

typedef void (*functionType)(int argc, char argv[6][21]);

void parser(const char *buffer);
functionType getFuncFromString(char *str, int *);