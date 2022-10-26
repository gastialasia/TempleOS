include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c ./standard_C_Library/*.c ./tests-TP2/*.c)
SOURCES_ASM=$(wildcard ./standard_C_Library/*.asm)
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
	rm -rf ./standard_C_Library/*.o

.PHONY: all clean
