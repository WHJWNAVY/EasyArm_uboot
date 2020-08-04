CURRENTDIR	= .
SRCDIR		= $(CURRENTDIR)
OUTPUTDIR	= $(CURRENTDIR)/output-target
COMMONSRCDIR	= $(CURRENTDIR)/../mach-$(ARCH)

INCLUDEDIRS	= -I$(COMMONSRCDIR) -I$(COMMONSRCDIR)/includes

# Linker script
BASE_ADDR	?= 0x00000000
BOOT_LAYOUT	= $(SRCDIR)/link.lds

# Output ELF image
POWER_PREP	= $(OUTPUTDIR)/power_prep

CROSS_COMPILE ?= arm-linux-gnueabi-

AS	= $(CROSS_COMPILE)as
CC	= $(CROSS_COMPILE)gcc
LD	= $(CROSS_COMPILE)ld
CPP	= $(CROSS_COMPILE)cpp
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

MEM_TYPE ?= MEM_DDR1

POWERFLAG = ?
ifeq ($(POWERFLAG),MXS_VBUS_CURRENT_DRAW)
CFLAGS += -DMXS_VBUS_CURRENT_DRAW
endif

CFLAGS 	+= -Wall $(INCLUDEDIRS) -I./ -O -DSTMP378x -D$(ARCH)
LDFLAGS = -static -gc-sections -nostdlib -T $(BOOT_LAYOUT)


#TOP_LEVEL_OBJS = power_prep.o

#POWER_PREP_OBJS += $(COMMONSRCDIR)/hw/libhw.a
#POWER_PREP_OBJS += $(COMMONSRCDIR)/drivers/libddi.a
hw_libs += $(COMMONSRCDIR)/hw/power/libhw_power.a
hw_libs += $(COMMONSRCDIR)/hw/lradc/libhw_lradc.a
hw_libs += $(COMMONSRCDIR)/hw/icoll/libhw_icoll.a
hw_libs += $(COMMONSRCDIR)/hw/digctl/libhw_digctl.a
hw_libs += $(COMMONSRCDIR)/hw/core/libhw_core.a
ddi_libs += $(COMMONSRCDIR)/drivers/power/libddi_power.a


POWER_PREP_OBJS = power_prep.o eabi.o

# Generic code
CORE_OBJS = debug.o

# Default goal
.PHONY: all
all: power_prep

%.o: %.S
	$(CC) -c $(CFLAGS) -D__ASSEMBLY__ -o $@ $<

%.o: %.arm
	$(CC) -c $(CFLAGS) -D__ASSEMBLY__ -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<



#
# Make targets
#
.PHONY: build build_prep clean
power_prep: libs $(BOOT_LAYOUT) $(POWER_PREP_OBJS) $(CORE_OBJS)
	$(LD) -o $@ $(POWER_PREP_OBJS) $(ddi_libs) $(hw_libs) \
	$(LDFLAGS) $(CORE_OBJS)
	@nm -n $@ > power_prep.map

libs:
	$(MAKE) -C $(COMMONSRCDIR)/hw
	$(MAKE) -C $(COMMONSRCDIR)/drivers

clean:
	@echo Cleaning...
	rm -f power_prep.map
	rm -f power_prep
	$(MAKE) clean -C $(COMMONSRCDIR)/hw
	$(MAKE) clean -C $(COMMONSRCDIR)/drivers
	@echo Files:
	rm -f $(POWER_PREP_OBJS)
	@echo Build output:

