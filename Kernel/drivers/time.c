// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>
#include <interrupts.h>

#define TICKSPERSECOND 18

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / TICKSPERSECOND;
}

void tSleep(int ms){
	int final_ticks = ticks + ms/55;///;secs*TICKSPERSECOND
	while(ticks <= final_ticks)
		_hlt();
}
