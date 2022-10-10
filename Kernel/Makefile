include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c drivers/*.c interruptions/*.c system/*.c utils/*.c)
SOURCES_ASM=$(wildcard system/*.asm interruptions/*.asm drivers/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

MM=HEAP2

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(STATICLIBS) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^
%.o: %.c
	$(GCC) $(GCCFLAGS) -D$(MM) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf *.o *.bin system/*.o interruptions/*.o drivers/*.o *.elf utils/*.o

.PHONY: all clean
