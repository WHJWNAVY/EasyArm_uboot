CURRENTDIR	= .
SRCDIR		= $(CURRENTDIR)
OUTPUTDIR	= $(CURRENTDIR)/output-target

INCLUDEDIR	= $(SRCDIR)/include
HWDIR		= $(SRCDIR)/hw
BOARDDIR	= $(SRCDIR)/board
COREDIR		= $(SRCDIR)/core
CMDLINESDIR	= $(SRCDIR)/cmdlines

# Linker script
BASE_ADDR	?= 0x00002000
BOOT_LAYOUT_IN	= $(SRCDIR)/linux_prep.ld.in
BOOT_LAYOUT_OUT	= $(OUTPUTDIR)/linux_prep.ld


# Output ELF image
LINUX_PREP	= $(OUTPUTDIR)/linux_prep

# Output binary image
LINUX_PREP_ROM	= $(OUTPUTDIR)/linux_prep.rom

CROSS_COMPILE ?= arm-linux-gnueabi-

AS	= $(CROSS_COMPILE)as
CC	= $(CROSS_COMPILE)gcc
LD	= $(CROSS_COMPILE)ld
CPP	= $(CROSS_COMPILE)cpp
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

LIBGCCDIR = $(dir $(shell $(CC) -print-libgcc-file-name))
CFLAGS 	= -Wall -I$(INCLUDEDIR) -nostdinc -fno-builtin -O
LDFLAGS = -static -nostdlib -T $(BOOT_LAYOUT_OUT) -L$(LIBGCCDIR) -lgcc

BOARD ?= stmp378x_dev

# Main board-specific source file
BOARD_OBJS = $(BOARD).o

# Sources for various HW modules. Define each module separately
# to make it possible to select different modules for different
# boards. Tha main purpose to handle some HW pieces in linux_prep
# is to receive magic keys from them.
#
# Note that the source files are located under hw/ directory.
LRADC_OBJS = lradc-buttons.o

# Text file with command lines
CMDLINES  ?= $(CMDLINESDIR)/$(BOARD).txt
CMDLINES_STRIP	= $(OUTPUTDIR)/command_lines_stripped.txt

# The board defines STMP CPU family and peripherial components
# So, set ARCH and HW_OBJS variables for each supported board
# separately.
ifeq ($(BOARD), stmp37xx_dev)
ARCH = 37xx
HW_OBJS = $(LRADC_OBJS)
CFLAGS += -DSTMP37XX -DBOARD_STMP37XX_DEV
endif
ifeq ($(BOARD), stmp378x_dev)
ARCH = mx23
HW_OBJS = $(LRADC_OBJS)
CFLAGS += -DSTMP378X -DBOARD_STMP378X_DEV
endif
ifeq ($(BOARD), iMX28_EVK)
ARCH = mx28
HW_OBJS = $(LRADC_OBJS)
CFLAGS += -DMX28 -DBOARD_MX28_EVK
endif

# Generic code
CORE_OBJS = entry.o resume.o cmdlines.o setup.o keys.o

LINUX_PREP_OBJS = $(addprefix $(COREDIR)/, $(CORE_OBJS)) \
		  $(addprefix $(BOARDDIR)/, $(BOARD_OBJS)) \
		  $(addprefix $(HWDIR)/, $(HW_OBJS))

# Default goal
.PHONY: all
all: build

#
# Pass a preprocessed command line file build it into linux_prep
# image. Also rebuild a linux_prep if command line has been changed
# since last build
#
$(COREDIR)/cmdlines.o: CFLAGS += -DCMDLINES_FILE=\"$(CMDLINES_STRIP)\"
$(COREDIR)/cmdlines.o: $(CMDLINES_STRIP)

#
# Define an implicit rule for assembler files
# to run them through C preprocessor
#
%.o: %.S
	$(CC) -c $(CFLAGS) -D__ASSEMBLY__ -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

#
# Make targets
#
.PHONY: build build_prep clean

build: build_prep $(CMDLINES_STRIP) $(LINUX_PREP_ROM)

build_prep:
	@(if [ -z $(ARCH) ] ; \
	then \
		echo WARNING: pass BOARD= value ; \
		exit 1 ; \
	else \
		mkdir -p $(OUTPUTDIR) ; \
		ln -sf $(ARCH) $(INCLUDEDIR)/arch ; \
	fi)

clean:
	@echo Cleaning...
	@echo Files:
	rm -rf $(LINUX_PREP_OBJS) $(BOOT_LAYOUT_OUT)
	@echo Build output:
	rm -rf $(OUTPUTDIR)
	@echo Build system:
	rm -rf $(INCLUDEDIR)/arch

#
# Rules to build linux_prep image
#
$(CMDLINES_STRIP): $(CMDLINES)
	$(call strip_cmdlines_file)

$(LINUX_PREP_ROM): $(LINUX_PREP)
	$(OBJCOPY) -R -S -O binary $< $@

$(LINUX_PREP): $(LINUX_PREP_OBJS) $(BOOT_LAYOUT_OUT)
	$(LD) -o $@ $(LINUX_PREP_OBJS) $(LDFLAGS)
	@nm -n $@ > $@.map

$(BOOT_LAYOUT_OUT): $(BOOT_LAYOUT_IN)
	$(CPP) -P -DBASE_ADDR=$(BASE_ADDR) -o $@ $<

#
# Preprocess a command lines file
# Preprocess a command lines file
# linux_prep is not the best place to handle text files so it expets
# that all command lines will be stored one by separated by '\0'
# characters. So, the initial command lines file created by user should
# be preprocessed to remove leading and trailing and extra whitespaces,
# comments and empty strings, and put '\0' character at the end of each
# command line.
#
define strip_cmdlines_file
	@cat $(CMDLINES) | \
	sed -e 's/\t/\ /g' | \
	sed -e 's/^\ *//g' | \
	sed -e 's/\ *$$//g' | \
	sed -e 's/\ \ */\ /g' | \
	grep -v '^#' | \
	grep -v '^$$' | tr '\n' '\0' > $(CMDLINES_STRIP)
endef
