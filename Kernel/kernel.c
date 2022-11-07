// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <MemoryManagerWrapper.h>
#include <idtLoader.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <scheduler.h>
#include <stdint.h>
#include <syscalls.h>
#include <tools.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;
static void *const heapModuleAddress = (void *)0x600000;
static void *const memManagerAddress = (void *)0x50000;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize) {
  memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
  return (void *)((uint64_t)&endOfKernel +
                  PageSize * 8        // The size of the stack itself, 32KiB
                  - sizeof(uint64_t)  // Begin at the top of the stack
  );
}

void *initializeKernelBinary() {
  char buffer[10];

  ncPrint("[x64BareBones]");
  ncNewline();

  ncPrint("CPU Vendor:");
  ncPrint(cpuVendor(buffer));
  ncNewline();

  ncPrint("[Loading modules]");
  ncNewline();
  void *moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};

  loadModules(&endOfKernelBinary, moduleAddresses);
  ncPrint("[Done]");
  ncNewline();
  ncNewline();

  ncPrint("[Initializing kernel's binary]");
  ncNewline();

  clearBSS(&bss, &endOfKernel - &bss);

  ncPrint("  text: 0x");
  ncPrintHex((uint64_t)&text);
  ncNewline();
  ncPrint("  rodata: 0x");
  ncPrintHex((uint64_t)&rodata);
  ncNewline();
  ncPrint("  data: 0x");
  ncPrintHex((uint64_t)&data);
  ncNewline();
  ncPrint("  bss: 0x");
  ncPrintHex((uint64_t)&bss);
  ncNewline();

  ncPrint("[Done]");
  ncNewline();
  ncNewline();
  return getStackBase();
}

void runShell() { ((EntryPoint)sampleCodeModuleAddress)(); }

int main() {
  initMemManager(memManagerAddress, heapModuleAddress);
  initScheduler();

  char argv[6][21];
  argv[0][0] = 'S';
  argv[0][1] = 'h';
  argv[0][2] = 'e';
  argv[0][3] = 'l';
  argv[0][4] = 'l';
  argv[0][5] = 0;

  createProcess((uint64_t)sampleCodeModuleAddress, 1, 1, argv, NULL, NULL);

  loadIDT();

  return 0;
}
