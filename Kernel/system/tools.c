// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <tools.h>

#define DIFF 'A'-'a'
#define MIN 'a'
#define MAX 'z'
#define SLASH_ASCII 125
#define PIPE_ASCII 124
#define SEVEN_ASCII 55
#define AMPERSAND_ASCII 38

char isPipe(char c);
char isAlpha(char c);
char isAmpersand(char c);


char toMayusc(char c){
    if(isAlpha(c)){
        return c + DIFF;
    } else if (isPipe(c)) {
        return PIPE_ASCII;
    } else if (isAmpersand(c)) {
        return AMPERSAND_ASCII;
    }
    return c;
}

char isAlpha(char c){
    return c >= MIN && c <= MAX;
}

char isPipe(char c){
    return c==SLASH_ASCII;
}

char isAmpersand(char c){
    return c==SEVEN_ASCII;
}

// Devuelve 0 si son iguales, 1 sino
int strcmp(const char *str1, const char *str2){
    int pos=0;
    while(str1[pos]&&str2[pos]){
        if (str1[pos]!=str2[pos]){
            return 1;
        }
        pos++;
    }
    if(str1[pos]||str2[pos]){
        return 1;
    }
    return 0;
}
