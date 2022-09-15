// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h>

static char buffer[64] = { '0' };
static const uint32_t width = 80;
static const uint32_t height = 25 ;
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static uint8_t * currentVideoL = (uint8_t*)0xB8000; //posicion inicial de la division de pantalla (el + 2 es para que este en la mitad mas 1 a la der)
static uint8_t * currentVideoR = (uint8_t*)0xB8000;   //posicion inicial izq de la division de pantalla
static int split = 0;

// ncPrint (normal, left y right)

void ncPrintParametric(const char * str, void (*fn)(char c)){
	int i;
	for (i = 0; str[i] != 0; i++)
		fn(str[i]);
}

void ncPrint(const char * string){
	ncPrintParametric(string, ncPrintChar);
}

void ncPrintL(const char * string){
	ncPrintParametric(string, ncPrintCharL);
}

void ncPrintR(const char * string){
	ncPrintParametric(string, ncPrintCharR);
}

// ncSplit

void ncSplit(){
	if (!split){
	ncClear();
	for(int i = 0; i < height; i++){
		ncPrintCharAt('|',width/2,i);
	}
	split = 1;
	}

}

void ncUnSplit(){
	currentVideoL=video;
	currentVideoR=video;
	split = 0;
}

// ncPrintChar (normal, left y right)

void ncPrintChar(char character)
{
	if(currentVideo==(video+(width*height)*2)){
		ncMoveLines();
	}

	if(character == '\n') {
		ncNewline();
		return;
	}

	*currentVideo = character;
	currentVideo += 2;
}

void ncPrintCharL(char character)
{
	if(currentVideoL==((video+(width*height)*2)-width)){
		ncMoveLinesL();
	}

	if(character == '\n') {
		ncNewlineL();
		return;
	}

	if (currentVideoL!=video && (video-currentVideoL)%width==0){
		// si llego al final del renglon, me salteo la mitad derecha y sigo escribiendo abajo
		currentVideoL+=width;
	}


	*currentVideoL = character;
	currentVideoL += 2;
}

void ncPrintCharR(char character)
{

	if(currentVideoR==(video+(width*height)*2)){
		ncMoveLinesR();
	}

	if(character == '\n') {
		ncNewlineR();
		return;
	}

	if ((video-currentVideoR)%(width*2)==0){
		currentVideoR+=width+2;
		//return;
	}

	*currentVideoR = character;
	currentVideoR += 2;
}

// ncDeleteChar(normal)

void ncDeleteChar(){
	currentVideo -=2;
	*currentVideo = ' ';
}

// ncMoveLines (normal, left y right)

void ncMoveLines(){
	for(int i=0;i<height*width*2;){
		if (i>=(height-1)*width*2){
			video[i]=' ';
			i+=2;
		} else {
			video[i]=video[i+width*2];
			i++;
		}
	}
	currentVideo=currentVideo-(width*2);
}

void ncMoveLinesL(){
	for(int i=0;i<height*width*2;){
		if (i&&i%(width)==0){
			i+=width;
		}
		if (i>=(height-1)*width*2){
			video[i]=' ';
			i+=2;
		} else {
			video[i]=video[i+width*2];
			i++;
		}
	}
	currentVideoL=currentVideoL-(width*2);
}

void ncMoveLinesR(){
	for(int i=width;i<height*width*2;){
		if (i%(width*2)==0){
			i+=width;
		}
		if (i>=(height-1)*width*2){
			video[i]=' ';
			i+=2;
		} else {
			video[i]=video[i+width*2];
			i++;
		}


	}
	currentVideoR=currentVideoR-(width);
	ncPrintCharR('|');
}

// ncPrintCharAt (normal)

void ncPrintCharAt(char character, uint32_t x, uint32_t y){
	// No tengo en cuenta currentVideo, simplemente inserto en char en el punto(x,y)
	video[(width*y+x)*2]=character;
}

// ncNewLine (normal, left y right)

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncNewlineL()
{
	do
	{
		ncPrintCharL(' ');
	}
	while((uint64_t)(currentVideoL - video) % (width) != 0);
}

void ncNewlineR()
{
	do
	{
		ncPrintCharR(' ');
	}
	while((uint64_t)(currentVideoR - video) % (width * 2) != 0);
}

// ncPrintDec (normal, left y right)

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintDecL(uint64_t value)
{
	ncPrintBaseL(value, 10);
}

void ncPrintDecR(uint64_t value)
{
	ncPrintBaseR(value, 10);
}

// ncPrintHex (normal, left y right)

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintHexL(uint64_t value)
{
	ncPrintBaseL(value, 16);
}

void ncPrintHexR(uint64_t value)
{
	ncPrintBaseR(value, 16);
}

// ncPrintBin (normal, left y right)

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBinL(uint64_t value)
{
	ncPrintBaseL(value, 2);
}

void ncPrintBinR(uint64_t value)
{
	ncPrintBaseR(value, 2);
}

// ncPrintBase (normal, left y right)

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncPrintBaseL(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrintL(buffer);
}

void ncPrintBaseR(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrintR(buffer);
}

void ncPrintRegParametric(const char*regName, uint64_t regValue, void (*fn)(const char*)){
	fn(regName);
	fn(": ");
	int digits = uintToBase(regValue, buffer, 16);
	for (int i = 1; i < 16 - digits; i++)
		fn("0");
	fn(buffer);
	fn("\n");
}

void ncPrintReg(const char *regName, uint64_t regValue)
{
	ncPrintRegParametric(regName, regValue, ncPrint);
}

void ncPrintRegL(const char *regName, uint64_t regValue)
{
	ncPrintRegParametric(regName, regValue, ncPrintL);
}

void ncPrintRegR(const char *regName, uint64_t regValue)
{
	ncPrintRegParametric(regName, regValue, ncPrintR);
}

// ncClear (normal)

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
