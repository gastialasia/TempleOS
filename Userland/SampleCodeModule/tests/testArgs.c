#include <testArgs.h>

void test_args(int argc, char args[ARG_QTY][ARG_LEN]) {
  int i;
  for (i = 0; i < argc; i++) {
    printf(args[i]);
    putchar('\n');
  }
  exit();
}