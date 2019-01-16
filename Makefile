# File:		Makefile
# Author:	Zeb Barry
# Date: 	16 Jan 2019
# Descr:	Makefile for countdown clock

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I./modules -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: countdown.out


# Compile: create object files from C source files.
countdown.o: countdown.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/system.h ../../drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
countdown.out: countdown.o system.o pio.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean:
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: countdown.out
	$(OBJCOPY) -O ihex countdown.out countdown.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash countdown.hex; dfu-programmer atmega32u2 start
