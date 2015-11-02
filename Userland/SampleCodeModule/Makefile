include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard src/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_MAIN=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_MAIN=$(SOURCES_MAIN:.c=.o)


all: $(MODULE)

$(MODULE): $(OBJECTS_MAIN) $(OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld $(OBJECTS_MAIN) $(OBJECTS) $(OBJECTS_ASM) -o ../$(MODULE)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o asm/*.o src/*.o

.PHONY: all clean
