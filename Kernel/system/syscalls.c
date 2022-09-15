// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscalls.h>
#include <naiveConsole.h>

#define STDIN 1
#define LEFTSCREEN 2
#define RIGHTSCREEN 3
#define DEFAULT_RETVALUE -1

static char mayusc = 0;
unsigned char last;

static char screenMode = 1;
static char * runningPrograms[2] = {"null","null"};

void (*printCharPtr)(char) = ncPrintChar;
void (*printPtr)(const char*) = ncPrint;
void (*printHexPtr)(uint64_t) = ncPrintHex;
void (*printRegPtr)(const char *, uint64_t) = ncPrintReg;

registersT primary, secondary;
registersT *primaryBackup = &primary;
registersT *secondaryBackup = &secondary;

char * getScreenModePtr(){
	return &screenMode;
}

char ** getRunningProgramPtr(int index){
	return &runningPrograms[index];
}

int64_t write(int fd, const char *buffer, size_t count)
{
	for (int i = 0; i < count; i++)
	{
		printCharPtr(buffer[i]);
	}
	return DEFAULT_RETVALUE;	
}

int64_t read(int fd, char *buffer, size_t count)
{
	if (fd == STDIN)
	{
		int k = 0;
		unsigned char key = 0;

		while (key != '\n' && k < count)
		{
			_hlt();
			key = readKey();
			switch (key)
			{
			case 0:
				break;
			case 8: // Borrado
			{
				if (k > 0)
				{
					ncDeleteChar();
					k--;
				}
				break;
			}
			case 14:
			case 15:
				mayusc = 1;
				break;
			case 17:
        		loadBackupRegs(primaryBackup, secondaryBackup);
        		break;
			case 170:
			case 182:
				mayusc = 0;
				break;
			default:
			{
				if (mayusc)
					key = toMayusc(key);
				printCharPtr(key);
				buffer[k++] = key;
			}
			}
		}
		if (key != '\n')
		{
			ncNewline();
		}
		buffer[k] = 0;
		return k; // placeholder
	}
	return DEFAULT_RETVALUE;
}

void snapshotRegs(){
	loadBackupRegs(primaryBackup, secondaryBackup);
}

void printMem(uint64_t pointer, unsigned char * buf)
{
	uint8_t *start = (uint8_t *)pointer;
	for (int i = 0; i < 32; i++)
	{
		buf[i] = start[i];
	}
}

void clear()
{
	if(screenMode==1){ //Simple screen
		ncClear();
	}
}

int64_t date(char value)
{
	return rtcGetter(value);
}

void loadBackupRegs(registersT* regs, registersT *backup) {
  	regs->rax = backup->rax;
	regs->rbx = backup->rbx;
	regs->rcx = backup->rcx;
	regs->rdx = backup->rdx;
	regs->rbp = backup->rbp;
	regs->rdi = backup->rdi;
	regs->rsi = backup->rsi;
	regs->r8 = backup->r8;
	regs->r9 = backup->r9;
	regs->r10 = backup->r10;
	regs->r11 = backup->r11;
	regs->r12 = backup->r12;
	regs->r13 = backup->r13;
	regs->r14 = backup->r14;
	regs->r15 = backup->r15;
}

void saveBackup() {
  saveRegisters(secondaryBackup);
}

void inforeg(registersT* regs)
{	   					  
	loadBackupRegs(regs, primaryBackup); 
}

int64_t getLast(){
    return last;
}

void sleep(int ms)
{
	tSleep(ms);
}

void setScreenMode(int mode)
{
	screenMode = mode;
	switch (mode)
	{
	case 2:
		ncSplit();
		printCharPtr = ncPrintCharL;
		printPtr = ncPrintL;
		printHexPtr = ncPrintHexL;
		printRegPtr = ncPrintRegL;
		break;
	case 3:
		ncSplit();
		printCharPtr = ncPrintCharR;
		printPtr = ncPrintR;
		printHexPtr = ncPrintHexR;
		printRegPtr = ncPrintRegR;
		break;
	default:
		ncUnSplit();
		printCharPtr = ncPrintChar;
		printPtr = ncPrint;
		printHexPtr = ncPrintHex;
		printRegPtr = ncPrintReg;
	}
}

void storeProgram(char*p1, char*p2){
	runningPrograms[0] = p1;
	runningPrograms[1] = p2;
}

char * getProgram(int id){
	if (id>1||id<0){
		return 0;
	}
	return runningPrograms[id];
}

