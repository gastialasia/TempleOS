// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboard.h>
#include <stdint.h>
#include <time.h>

static void int_20();
static void int_21();

void (*fns[])(void) = {int_20, int_21};

void irqDispatcher(uint64_t irq) { fns[irq](); }

void int_20() { timer_handler(); }

void int_21() { keyboard_handler(); }
