#include <stdint.h>

typedef void (*functionType)(int argc, char argv[ARG_QTY][ARG_LEN]);

void parser(const char *buffer);
functionType getFuncFromString(char *str, int *);