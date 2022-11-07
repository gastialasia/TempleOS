#include <stdint.h>
#include <stdlib.h>

// Random
static uint32_t mz = 362436069;
static uint32_t mw = 521288629;

uint32_t GetUint() {
  mz = 36969 * (mz & 65535) + (mz >> 16);
  mw = 18000 * (mw & 65535) + (mw >> 16);
  return (mz << 16) + mw;
}

uint32_t GetUniform(uint32_t max) {
  uint32_t u = GetUint();
  return (u + 1.0) * 2.328306435454494e-10 * max;
}

// Memory
uint8_t memcheck(void *start, uint8_t value, uint32_t size) {
  uint8_t *p = (uint8_t *)start;
  uint32_t i;

  for (i = 0; i < size; i++, p++)
    if (*p != value) return 0;

  return 1;
}

// Dummies
void bussyWait(uint64_t n) {
  uint64_t i;
  for (i = 0; i < n; i++)
    ;
}

void endlessLoop() {
  while (1)
    ;
}
