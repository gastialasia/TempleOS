// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdlib.h>
#include <syscallslib.h>
#include <stdint.h>

#define STDIN 1
#define LEFTSCREEN 2
#define RIGHTSCREEN 3
#define MAXBUFFER 100

int strlen(const char *str)
{
    int len = 0;
    while (str[len] != 0)
    {
        len++;
    }
    return len;
}

void putchar(const char c)
{
    sys_write(&c, 1);
}

void printf(const char *str)
{
    sys_write(str, strlen(str));
}

void inforeg(registersT *regs)
{
    sys_inforeg(regs);
}

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    // Calculate characters for each digit
    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    // Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}

// Integer to string
int itos(int value, char *target)
{
    int digit;
    int sign = 1;
    int i = -1, j = 0;
    char aux[11];
    if (value < 0)
    {
        sign = 0;
        value *= -1;
    }
    do
    {
        i++;
        digit = value % 10;
        aux[i] = digit + '0';
        value /= 10;
    } while (value > 0);
    if (!sign)
        target[j++] = '-';
    while (i > -1)
        target[j++] = aux[i--];
    target[j] = 0;
    return j;
}

void printInt(int num)
{
    char buffer[10];
    int len = uintToBase(num, buffer, 10);
    sys_write(buffer, len);
}

void printInt64(unsigned long num)
{
    char buffer[20];
    int len = uintToBase(num, buffer, 10);
    sys_write(buffer, len);
}

void getchar(char *c)
{
    sys_read(c, 1);
}

int scanf(char *buffer)
{
    return sys_read(buffer, -1);
}

uint64_t hex2int(char *hex, int *ok)
{
    uint64_t val = 0;
    int len = 0;
    while (*hex && len < 8)
    {
        // get current character then increment
        uint8_t byte = *hex++;

        if (!(byte >= '0' && byte <= '9') && !(byte >= 'a' && byte <= 'f') && !(byte >= 'A' && byte <= 'F'))
        {
            *ok = 0;
            return 0;
        }
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte <= '9')
            byte = byte - '0';
        else if (byte >= 'a')
            byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <= 'F')
            byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
        len++;
    }
    return val;
}

void clear()
{
    sys_clear();
}

// Devuelve 0 si son iguales, 1 sino
int strcmp(const char *str1, const char *str2)
{
    int pos = 0;
    while (str1[pos] && str2[pos])
    {
        if (str1[pos] != str2[pos])
        {
            return 1;
        }
        pos++;
    }
    if (str1[pos] || str2[pos])
    {
        return 1;
    }
    return 0;
}

int strcpy(char *dest, const char *src)
{
    int i;
    for (i = 0; src[i]; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = 0;
    return i;
}

char *strcat(char *destination, const char *source)
{
    // make `ptr` point to the end of the destination string
    char *ptr = destination + strlen(destination);

    // appends characters of the source to the destination string
    while (*source != '\0')
    {
        *ptr++ = *source++;
    }

    // null terminate destination string
    *ptr = '\0';

    // the destination is returned by standard `strcat()`
    return destination;
}

char getDateComponent(int value)
{
    return sys_date(value);
}

void sleep(int secs)
{
    sys_sleep(secs);
}

void takeSnapShot()
{
    sys_snapshotRegs();
}

void printReg(const char *regName, uint64_t regValue)
{
    char buffer[17];
    printf(regName);
    printf(": ");
    int digits = uintToBase(regValue, buffer, 16);
    for (int i = 1; i < 16 - digits; i++)
        printf("0");
    printf(buffer);
    putchar('\n');
}

void printMem(uint64_t pointer, char *buffer)
{
    return sys_printMem(pointer, buffer);
}

unsigned int is_delim(char c, char *delim)
{
    while (*delim != '\0')
    {
        if (c == *delim)
            return 1;
        delim++;
    }
    return 0;
}

char *strtok(char *srcString, char *delim)
{
    static char *backup_string; // start of the next search
    if (!srcString)
    {
        srcString = backup_string;
    }
    if (!srcString)
    {
        // user is bad user
        return NULL;
    }
    // handle beginning of the string containing delims
    while (1)
    {
        if (is_delim(*srcString, delim))
        {
            srcString++;
            continue;
        }
        if (*srcString == '\0')
        {
            // we've reached the end of the string
            return NULL;
        }
        break;
    }
    char *ret = srcString;
    while (1)
    {
        if (*srcString == '\0')
        {
            /*end of the input string and
            next exec will return NULL*/
            backup_string = srcString;
            return ret;
        }
        if (is_delim(*srcString, delim))
        {
            *srcString = '\0';
            backup_string = srcString + 1;
            return ret;
        }
        srcString++;
    }
}

void *malloc(unsigned int bytes)
{
    return sys_malloc(bytes);
}

void free(void *memToFree)
{
    sys_free(memToFree);
}

void memStatus(unsigned int *status)
{
    return sys_memStatus(status);
}

int createProcess(uint64_t ip, uint8_t priority, uint64_t argc, char argv[6][21], fd *customStdin, fd *customStdout)
{
    return sys_createProcess(ip, priority, argc, argv, customStdin, customStdout);
}

void exit()
{
    sys_exit();
}

int getpid()
{
    return sys_getpid();
}

void ps()
{
    static char buffer[3000];
    sys_ps(buffer);
    printf(buffer);
    buffer[0] = 0;
}

int kill(uint32_t pid)
{
    return sys_kill(pid);
}

int atoi(const char *S)
{
    int num = 0;

    int i = 0;

    while (S[i] && (S[i] >= '0' && S[i] <= '9'))
    {
        num = num * 10 + (S[i] - '0');
        i++;
    }

    return num;
}

int nice(uint32_t pid, uint8_t newPriority)
{
    return sys_changepriority(pid, newPriority);
}

int block(uint32_t pid)
{
    return sys_changestate(pid);
}

void yield()
{
    sys_yield();
}

Semaphore *semOpen(uint32_t id, int value)
{
    return sys_semOpen(id, value);
}

int semClose(Semaphore *sem)
{
    return sys_semClose(sem);
}

int semPost(Semaphore *sem)
{
    return sys_semPost(sem);
}

int semWait(Semaphore *sem)
{
    return sys_semWait(sem);
}

void getAllSems(char *buf)
{
    sys_getAllSems(buf);
}

int createPipe(fd *fd1, fd *fd2)
{
    return sys_createPipe(fd1, fd2);
}

fd *createFd()
{
    return (fd *)malloc(sizeof(fd));
}

void getAllPipes(char *buf)
{
    sys_getAllPipes(buf);
}

int openPipe(fd *user, uint32_t id, uint8_t permisions){
    return sys_openPipe(user, id, permisions);
}

int pipeRead(fd *userPipe, char *buffer){
    return sys_pipeRead(userPipe, buffer
);
}

int pipeWrite(fd *userPipe, const char *string){
    return sys_pipeWrite(userPipe, string);
}

void closeFd(fd *user){
    sys_closeFd(user);
}
