#include <stdint.h>

typedef int (*function_type)(int argc, char argv[6][21]);

void parser(const char *buffer);
function_type getFuncFromString(char *str, int *);