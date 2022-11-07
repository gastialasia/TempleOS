// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <stdlib.h>
#include <syscallslib.h>

#define STDIN 1
#define LEFTSCREEN 2
#define RIGHTSCREEN 3
#define MAXBUFFER 100

int strlen(const char *str) {
  int len = 0;
  while (str[len] != 0) {
    len++;
  }
  return len;
}

void putchar(const char c) { sysWrite(&c, 1); }

void printf(const char *str) { sysWrite(str, strlen(str)); }

void inforeg(registersT *regs) { sysInforeg(regs); }

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base) {
  char *p = buffer;
  char *p1, *p2;
  uint32_t digits = 0;

  do {
    uint32_t remainder = value % base;
    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
    digits++;
  } while (value /= base);

  *p = 0;

  p1 = buffer;
  p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }

  return digits;
}

int itos(int value, char *target) {
  int digit;
  int sign = 1;
  int i = -1, j = 0;
  char aux[11];
  if (value < 0) {
    sign = 0;
    value *= -1;
  }
  do {
    i++;
    digit = value % 10;
    aux[i] = digit + '0';
    value /= 10;
  } while (value > 0);
  if (!sign) target[j++] = '-';
  while (i > -1) target[j++] = aux[i--];
  target[j] = 0;
  return j;
}

void printInt(int num) {
  char buffer[10];
  int len = uintToBase(num, buffer, 10);
  sysWrite(buffer, len);
}

void printInt64(unsigned long num) {
  char buffer[20];
  int len = uintToBase(num, buffer, 10);
  sysWrite(buffer, len);
}

void getchar(char *c) { sysRead(c, 1); }

int scanf(char *buffer) { return sysRead(buffer, -1); }

uint64_t hex2int(char *hex, int *ok) {
  uint64_t val = 0;
  int len = 0;
  while (*hex && len < 8) {
    uint8_t byte = *hex++;

    if (!(byte >= '0' && byte <= '9') && !(byte >= 'a' && byte <= 'f') &&
        !(byte >= 'A' && byte <= 'F')) {
      *ok = 0;
      return 0;
    }

    if (byte <= '9')
      byte = byte - '0';
    else if (byte >= 'a')
      byte = byte - 'a' + 10;
    else if (byte >= 'A' && byte <= 'F')
      byte = byte - 'A' + 10;
    val = (val << 4) | (byte & 0xF);
    len++;
  }
  return val;
}

void clear() { sysClear(); }

// Devuelve 0 si son iguales, 1 sino
int strcmp(const char *str1, const char *str2) {
  int pos = 0;
  while (str1[pos] && str2[pos]) {
    if (str1[pos] != str2[pos]) {
      return 1;
    }
    pos++;
  }
  if (str1[pos] || str2[pos]) {
    return 1;
  }
  return 0;
}

int strcpy(char *dest, const char *src) {
  int i;
  for (i = 0; src[i]; i++) {
    dest[i] = src[i];
  }
  dest[i] = 0;
  return i;
}

char *strcat(char *destination, const char *source) {
  char *ptr = destination + strlen(destination);

  while (*source != '\0') {
    *ptr++ = *source++;
  }

  *ptr = '\0';

  return destination;
}

char getDateComponent(int value) { return sysDate(value); }

void sleep(int secs) { sysSleep(secs); }

void takeSnapShot() { sysSnapshotRegs(); }

void printReg(const char *regName, uint64_t regValue) {
  char buffer[17];
  printf(regName);
  printf(": ");
  int digits = uintToBase(regValue, buffer, 16);
  for (int i = 1; i < 16 - digits; i++) printf("0");
  printf(buffer);
  putchar('\n');
}

void printMem(uint64_t pointer, char *buffer) {
  return sysPrintMem(pointer, buffer);
}

unsigned int is_delim(char c, char *delim) {
  while (*delim != '\0') {
    if (c == *delim) return 1;
    delim++;
  }
  return 0;
}

char *strtok(char *srcString, char *delim) {
  static char *backup_string;  
  if (!srcString) {
    srcString = backup_string;
  }
  if (!srcString) {
    return NULL;
  }
  while (1) {
    if (is_delim(*srcString, delim)) {
      srcString++;
      continue;
    }
    if (*srcString == '\0') {
      return NULL;
    }
    break;
  }
  char *ret = srcString;
  while (1) {
    if (*srcString == '\0') {
      backup_string = srcString;
      return ret;
    }
    if (is_delim(*srcString, delim)) {
      *srcString = '\0';
      backup_string = srcString + 1;
      return ret;
    }
    srcString++;
  }
}

void *malloc(unsigned int bytes) { return sysMalloc(bytes); }

void free(void *memToFree) { sysFree(memToFree); }

void memStatus(unsigned int *status) { return sysMemStatus(status); }

int createProcess(uint64_t ip, uint8_t priority, uint64_t argc,
                  char argv[ARG_QTY][ARG_LEN], fd *customStdin,
                  fd *customStdout) {
  return sysCreateProcess(ip, priority, argc, argv, customStdin, customStdout);
}

void exit() { sysExit(); }

int getpid() { return sysGetpid(); }

void ps() {
  static char buffer[3000];
  sysPs(buffer);
  printf(buffer);
  buffer[0] = 0;
}

int kill(uint32_t pid) { return sysKill(pid); }

int atoi(const char *S) {
  int num = 0;

  int i = 0;

  while (S[i] && (S[i] >= '0' && S[i] <= '9')) {
    num = num * 10 + (S[i] - '0');
    i++;
  }

  return num;
}

int nice(uint32_t pid, uint8_t newPriority) {
  return sysChangepriority(pid, newPriority);
}

int block(uint32_t pid) { return sysChangestate(pid); }

void yield() { sysYield(); }

Semaphore *semOpen(uint32_t id, int value) { return sysSemOpen(id, value); }

int semClose(Semaphore *sem) { return sysSemClose(sem); }

int semPost(Semaphore *sem) { return sysSemPost(sem); }

int semWait(Semaphore *sem) { return sysSemWait(sem); }

void getAllSems(char *buf) { sysGetAllSems(buf); }

int createPipe(fd *fd1, fd *fd2) { return sysCreatePipe(fd1, fd2); }

fd *createFd() { return (fd *)malloc(sizeof(fd)); }

void getAllPipes(char *buf) { sysGetAllPipes(buf); }

int openPipe(fd *user, uint32_t id, uint8_t permisions) {
  return sysOpenPipe(user, id, permisions);
}

int pipeRead(fd *userPipe, char *buffer) {
  return sysPipeRead(userPipe, buffer);
}

int pipeWrite(fd *userPipe, const char *string) {
  return sysPipeWrite(userPipe, string);
}

void closeFd(fd *user) { sysCloseFd(user); }
