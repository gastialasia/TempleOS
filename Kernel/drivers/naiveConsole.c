// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h>
#include <tools.h>

#define WHITE_ON_BLACK 0x0F
#define GREEN_ON_BLACK 0x02

static char buffer[64] = {'0'};
static const uint32_t width = 80;
static const uint32_t height = 25;
static uint8_t *const video = (uint8_t *)0xB8000;
static uint8_t *currentVideo = (uint8_t *)0xB8000;
// static showCursor=0;

static char color = WHITE_ON_BLACK;

void ncTogglePrintColor()
{
	if (color == WHITE_ON_BLACK)
	{
		color = GREEN_ON_BLACK;
	}
	else
	{
		color = WHITE_ON_BLACK;
	}
}

// void ncToggleCursor(){
// 	showCursor = !showCursor;
// }

void ncPrint(const char *string)
{
	int i;
	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

// ncPrintChar (normal, left y right)

void ncPrintChar(char character)
{
	if (currentVideo == (video + (width * height) * 2))
	{
		ncMoveLines();
	}

	if (character == '\n')
	{
		ncNewline();
		return;
	}

	*currentVideo = character;
	currentVideo += 1;
	*currentVideo = color; // 1111=F blanco, 0000=0 negro -> 1er: fondo, 2do: letra
	currentVideo += 1;
	*currentVideo = ' ';
	*(currentVideo + 1) = 0xF0;
}

// ncDeleteChar(normal)

void ncDeleteChar()
{
	currentVideo -= 2;
	*currentVideo = ' ';
	*(currentVideo + 3) = 0x0F;
	*(currentVideo + 1) = 0xF0;
}

// ncMoveLines (normal, left y right)

void ncMoveLines()
{
	for (int i = 0; i < height * width * 2;)
	{
		if (i >= (height - 1) * width * 2)
		{
			video[i] = ' ';
			i += 2;
		}
		else
		{
			video[i] = video[i + width * 2];
			i++;
		}
	}
	currentVideo = currentVideo - (width * 2);
}

// ncNewLine (normal, left y right)

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	} while ((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

// ncPrintDec (normal, left y right)

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

// ncPrintHex (normal, left y right)

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

// ncPrintBin (normal, left y right)

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

// ncPrintBase (normal, left y right)

void ncPrintBase(uint64_t value, uint32_t base)
{
	uintToBase(value, buffer, base);
	ncPrint(buffer);
}

void ncPrintReg(const char *regName, uint64_t regValue)
{
	ncPrint(regName);
	ncPrint(": ");
	int digits = uintToBase(regValue, buffer, 16);
	for (int i = 1; i < 16 - digits; i++)
		ncPrint("0");
	ncPrint(buffer);
	ncPrint("\n");
}

// ncClear (normal)

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
	{
		video[i * 2] = ' ';
		video[(i * 2) + 1] = WHITE_ON_BLACK;
	}
	currentVideo = video;
}
