// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <tools.h>

#define DIFF 'A' - 'a'
#define MIN 'a'
#define MAX 'z'
#define SLASH_ASCII 125
#define PIPE_ASCII 124
#define SEVEN_ASCII 55
#define AMPERSAND_ASCII 38

char isPipe(char c);
char isAlpha(char c);
char isAmpersand(char c);

char toMayusc(char c)
{
    if (isAlpha(c))
    {
        return c + DIFF;
    }
    else if (isPipe(c))
    {
        return PIPE_ASCII;
    }
    else if (isAmpersand(c))
    {
        return AMPERSAND_ASCII;
    }
    return c;
}

char isAlpha(char c)
{
    return c >= MIN && c <= MAX;
}

char isPipe(char c)
{
    return c == SLASH_ASCII;
}

char isAmpersand(char c)
{
    return c == SEVEN_ASCII;
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

int strlen(const char *str)
{
    int len = 0;
    while (str[len] != 0)
    {
        len++;
    }
    return len;
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
