// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscalls.h>
#include <stdint.h>
#include <stddef.h>
#include <syscalls.h>
#include <naiveConsole.h>
#include <scheduler.h>

static int syscallnumber;

void syscallsetter(int64_t syscall){
	syscallnumber = syscall;
}

int64_t syscallDispatcher(int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3, int64_t arg4, int64_t arg5) {
    switch (syscallnumber)
    {
    case 1:
        return read((char*) arg0, (size_t) arg1);
    case 2:
        return write((const char*) arg0, (size_t) arg1);
    case 3:
        clear();
        break;
    case 4:
	   	printMem((uint64_t) arg0,(unsigned char*)arg1);
		break;
    case 5:
        inforeg((registersT*)arg0);
        break;
    case 6:
        return date((char)arg0);
	case 7:
		sleep((int)arg0);
		break;
	case 8:
		return getLast();
    case 9:
        snapshotRegs();
        break;
    case 10:
        return malloc((unsigned int)arg0);
        break;
    case 11:
        mfree((void *)arg0);
        break;
    case 12:
        mStatus((void *)arg0);
        break;
    case 13:
        return cProcess((uint64_t)arg0, (uint8_t)arg1, (uint64_t)arg2, (char **)arg3, (pipeUserInfo *)arg4, (pipeUserInfo *)arg5);
        break;
    case 14:
        eProcess();
        break;
    case 15:
        return currentPID();
        break;
    case 16:
        getAllProcesses((char *)arg0);
        break;
    case 17:
        return pKill((uint32_t)arg0);
        break;
    case 18:
        return changeProcessPriority((uint32_t)arg0, (uint8_t)arg1);
        break;
    case 19:
        return changeProcessState((uint32_t)arg0);
        break;
    case 20:
        rScheduler();
        break;
    default:
        return -1;
    }
    return -1;
}
