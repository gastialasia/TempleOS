#include <stdlib.h>

int test_args(int argc, char args[6][21]) {
  int i;
  for (i = 0; i < argc; i++) {
    printf(args[i]);
    putchar('\n');
  }
  exit();
  return 0;
}