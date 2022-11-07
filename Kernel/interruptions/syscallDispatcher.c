// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h>
#include <pipes.h>
#include <scheduler.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdint.h>
#include <syscalls.h>

static int syscallnumber;

void syscallsetter(int64_t syscall) { syscallnumber = syscall; }

int64_t syscallDispatcher(int64_t arg0, int64_t arg1, int64_t arg2,
                          int64_t arg3, int64_t arg4, int64_t arg5) {
  switch (syscallnumber) {
    case 1:
      return read((char *)arg0, (size_t)arg1);
    case 2:
      return write((const char *)arg0, (size_t)arg1);
    case 3:
      clear();
      break;
    case 5:
      inforeg((registersT *)arg0);
      break;
    case 6:
      return date((char)arg0);
      break;
    case 7:
      sleep((int)arg0);
      break;
    case 8:
      return getLast();
    case 9:
      snapshotRegs();
      break;
    case 10:
      return (int64_t)alloc((unsigned int)arg0);
      break;
    case 11:
      free((void *)arg0);
      break;
    case 12:
      memStatus((void *)arg0);
      break;
    case 13:
      return createProcess((uint64_t)arg0, (uint8_t)arg1, (uint64_t)arg2,
                           (char(*)[21])arg3, (fd *)arg4, (fd *)arg5);
      break;
    case 14:
      exitProcess();
      break;
    case 15:
      return getCurrentPID();
      break;
    case 16:
      getAllProcesses((char *)arg0);
      break;
    case 17:
      return killPid((uint32_t)arg0);
      break;
    case 18:
      return changeProcessPriority((uint32_t)arg0, (uint8_t)arg1);
      break;
    case 19:
      return changeProcessState((uint32_t)arg0);
      break;
    case 20:
      runScheduler();
      break;
    case 21:
      return (int64_t)semOpen((uint32_t)arg0, (int)arg1);
      break;
    case 22:
      return semClose((semPointer)arg0);
      break;
    case 23:
      return semPost((semPointer)arg0);
      break;
    case 24:
      return semWait((semPointer)arg0);
      break;
    case 25:
      getAllSems((char *)arg0);
      break;
    case 26:
      return createPipes((fd *)arg0, (fd *)arg1);
      break;
    case 27:
      getAllPipes((char *)arg0);
      break;
    case 28:
      return openPipeID((fd *)arg0, (uint32_t)arg1, (uint8_t)arg2);
      break;
    case 29:
      return pipeRead((fd *)arg0, (char *)arg1, 0);
      break;
    case 30:
      return pipeWrite((fd *)arg0, (char *)arg1);
      break;
    case 31:
      closeFd((fd *)arg0);
      break;
    default:
      return -1;
  }
  return -1;
}
