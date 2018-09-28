#
# Makefile for the iMX23 digctl peripheral
#

COMMONSRCDIR    = ../../.
SRCDIR		= ./src
OUTPUTDIR	?= .
LIBNAME		?= hw_digctl

INCLUDEDIRS	= -I$(COMMONSRCDIR)/includes -I$(COMMONSRCDIR)

CROSS_COMPILE ?= arm-linux-gnueabi-

AS	= $(CROSS_COMPILE)as
CC	= $(CROSS_COMPILE)gcc
LD	= $(CROSS_COMPILE)ld
CPP	= $(CROSS_COMPILE)cpp
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)o

CFLAGS 	= -Wall -ffunction-sections $(INCLUDEDIRS) -I./ -O -DSTMP378x

lib_objs := src/hw_digctl.o

# Default goal
.PHONY: all
all: lib$(LIBNAME)

%.o: %.S
	$(CC) -c $(CFLAGS) -D__ASSEMBLY__ $< -o $@

%.o: %.arm
	$(CC) -c $(CFLAGS) -D__ASSEMBLY__ $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@



#
# Make targets
#
.PHONY: build build_prep clean

lib$(LIBNAME): $(lib_objs)
	ar rs lib$(LIBNAME).a $(lib_objs)
#	ranlib lib$(LIBNAME).a

clean:
	@echo Cleaning...
	@echo Files:
	rm -f $(lib_objs)
	@echo Build output:
	rm -f $(OUTPUTDIR)/lib$(LIBNAME).a

