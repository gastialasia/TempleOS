#ifndef KEYBOARD_H_
#define KEYBOARD_H_

static const unsigned char ASCIITable[] = {
     0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', 0, 8,  
     9, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0,  '+', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '{', 0, 14,  '}', // 14 es shift izquierdo pressed
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',','.', '-',  15, '*', 0, ' ', 11, 17, // 15 Shift derecho pressed, 11 bloq mayus, 17 F1 que representa device control 1 en tabla ascii
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0xAA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xB6, 0, 0, 0, 0, 0, // 0xAA y 0xB6 shift izquierdo y derecho release
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_handler();

unsigned char getKey(); 

unsigned char readKey(); 

#endif
