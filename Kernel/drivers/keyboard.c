// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/keyboard.h"
#include "../include/lib.h"
#include "../include/syscall.h"
#include "../include/scheduler.h"

#define F2 18

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
	if(key != 0){
		last = key;
    awakeKeyboardList();
	if(key == F2)
		exitCurrentProcess();
  	}
}
