// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboard.h>
#include <naiveConsole.h>
#include <lib.h>
#include <syscalls.h>

unsigned char key = 0;
extern unsigned char last;

unsigned char readKey(){
	// Agregar chequeo de tecla
	unsigned char aux = key;
	key = 0;
	return aux;
}

void keyboard_handler() {
	key = ASCIITable[getKey()];
	if(key != 0)
		last = key;
}
