#
# Makefile for the iMX23 digctl peripheral
#

COMMONSRCDIR    = ../../.
SRCDIR		= ./src
OUTPUTDIR	?= .
LIBNAME		?= hw_core

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

#lib_objs+=src/hw_core_c_interrupt.o
#lib_objs+=src/hw_core_dbguart.o
lib_objs = src/hw_core_asm_interrupt.o
#lib_objs+=src/hw_core_asm_handlers.o
#lib_objs+=src/hw_core_mmu_funcs.o
#lib_objs+=src/hw_core_mmu_support.o
#lib_objs+=src/hw_core_mmu_support_set_pagetable_base.o
#lib_objs+=src/hw_core_mmu_support_lockitlbmva.o
#lib_objs+=src/hw_core_mmu_support_LockDTLBMVA.o
#lib_objs+=src/hw_core_mmu_support_InvalidateAndCleanDcachePageMVA.o
#lib_objs+=src/hw_core_mmu_support_InvalidateDcachePageMVA.o
#lib_objs+=src/hw_core_mmu_support_set_CP15_control_reg_1.o
#lib_objs+=src/hw_core_mmu_support_set_FCSE.o
#lib_objs+=src/hw_core_mmu_support_InvalidateIcachePageMVA.o


# Default goal
.PHONY: all
all: lib$(LIBNAME)

%.o: %.S
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

