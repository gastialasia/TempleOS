include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c ./standardLibrary/*.c ./tests/*.c)
SOURCES_ASM=$(wildcard ./standardLibrary/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)

$(MODULE): $(SOURCES) $(OBJECTS_ASM)
#$(LD) $(LDFLAGS) - sampleCodeModule.ld -o $(MODULE) $(OBJECTS) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld _loader.c $(SOURCES) $(OBJECTS_ASM) -o ../$(MODULE)

#%.o: %.c
#	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@
%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf ./standardLibrary/*.o ./tests/*.o

.PHONY: all clean
