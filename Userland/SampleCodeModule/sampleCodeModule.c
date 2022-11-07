// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <shell.h>
#include <stdlib.h>

void welcome();

int main() {
  welcome();
  sleep(1000);
  shell();
  return 0;
}

void welcome() {
  clear();
  printf("[Kernel Main]");
  putchar('\n');
  printf("IDT loaded succesfully!");
  putchar('\n');
  putchar('\n');
  putchar('\n');
  printf("                             WELCOME TO TEMPLE OS");
  putchar('\n');
  putchar('\n');
  putchar('\n');
  printf("~");
  printf("                                     +");
  putchar('\n');
  printf("                                     |");
  putchar('\n');
  printf("                                    /o\\");
  putchar('\n');
  printf("                                    |-|");
  putchar('\n');
  printf("                                 ,  | |  ,");
  putchar('\n');
  printf("                                 /\\/ _ \\/\\");
  putchar('\n');
  printf("                           .-.-.-|| (_) ||-.-.-.");
  putchar('\n');
  printf("                           | # # ||  _  || # # |");
  putchar('\n');
  printf("                          _|     || ||| ||     |_");
  putchar('\n');
  printf("                            ********===********");
  putchar('\n');
  printf("                                    ===");
  putchar('\n');
  printf("~");
}
