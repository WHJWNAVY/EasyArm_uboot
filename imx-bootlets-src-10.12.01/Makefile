export CROSS_COMPILE
MEM_TYPE ?= MEM_DDR1
export MEM_TYPE

#DEV_IMAGE指向用户的zImage和u-boot所在的路径
#DEV_IMAGE=/home/proton/EasyARM-iM283/rootfs/rootfs/boot
DEV_IMAGE=./
TFTPBOOT=/home/tftpboot/

DFT_IMAGE=$(DEV_IMAGE)/zImage
DFT_UBOOT=$(DEV_IMAGE)/u-boot

BOARD ?= stmp378x_dev

ifeq ($(BOARD), stmp37xx_dev)
ARCH = 37xx
endif
ifeq ($(BOARD), stmp378x_dev)
ARCH = mx23
endif
ifeq ($(BOARD), iMX28_EVK)
ARCH = mx28
endif

all: build_prep gen_bootstream

build_prep:


gen_bootstream: linux_prep boot_prep power_prep linux.bd uboot.bd linux.bd uboot.bd updater.bd
	@echo "generating linux kernel boot stream image"

	@echo "by using the rootfs/boot/zImage"
	sed -i 's,[^ *]zImage.*;,\tzImage="$(DFT_IMAGE)";,' linux.bd
	sed -i 's,[^ *]zImage.*;,\tzImage="$(DFT_IMAGE)";,' linux_ivt.bd
	elftosb -c ./linux.bd -o i$(ARCH)_linux.sb
	elftosb -f imx28 -c ./linux_ivt.bd -o i$(ARCH)_ivt_linux.sb

	@echo "by using the rootfs/boot/u-boot"
	sed -i 's,[^ *]u_boot.*;,\tu_boot="$(DFT_UBOOT)";,' uboot.bd
	sed -i 's,[^ *]u_boot.*;,\tu_boot="$(DFT_UBOOT)";,' uboot_ivt.bd
	elftosb -c ./uboot.bd -o i$(ARCH)_uboot.sb
	elftosb -f imx28 -c ./uboot_ivt.bd -o i$(ARCH)_ivt_uboot.sb
	
	cp -av ./imx28_ivt_uboot.sb $(TFTPBOOT) -fr
	cp -av ./imx28_ivt_linux.sb $(TFTPBOOT) -fr
	#@echo "generating kernel bootstream file sd_mmc_bootstream.raw"
	#Please use cfimager to burn xxx_linux.sb. The below way will no
	#work at imx28 platform.
	#rm -f sd_mmc_bootstream.raw
	#dd if=/dev/zero of=sd_mmc_bootstream.raw bs=512 count=4
	#dd if=imx233_linux.sb of=sd_mmc_bootstream.raw ibs=512 seek=4 \
	#conv=sync,notrunc
	#@echo "To install bootstream onto SD/MMC card, type: sudo dd \
	#if=sd_mmc_bootstream.raw of=/dev/sdXY where X is the correct letter \
	#for your sd or mmc device (to check, do a ls /dev/sd*) and Y \
	#is the partition number for the bootstream"

# TODO
#	@echo "generating uuc boot stream image"

power_prep:
	@echo "build power_prep"
	$(MAKE) -C power_prep ARCH=$(ARCH) BOARD=$(BOARD)

boot_prep:
	@echo "build boot_prep"
	$(MAKE) -C boot_prep  ARCH=$(ARCH) BOARD=$(BOARD)

updater: linux_prep boot_prep power_prep
	@echo "Build updater firmware"
	elftosb -c ./updater.bd -o updater.sb
	elftosb -f imx28 -c ./updater_ivt.bd -o updater_ivt.sb
linux_prep:
ifneq "$(CMDLINE1)" ""
	@echo "by using environment command line"
	@echo -e "$(CMDLINE1)\n$(CMDLINE2)\n$(CMDLINE3)\n$(CMDLINE4)" \
		> linux_prep/cmdlines/$(BOARD).txt
else
	@echo "by using the pre-build command line"
endif
	# force building linux_prep
	$(MAKE) clean -C linux_prep
	@echo "cross-compiling linux_prep"
	$(MAKE) -C linux_prep ARCH=$(ARCH) BOARD=$(BOARD)
install:
	cp -f boot_prep/boot_prep  ${DESTDIR}

	cp -f power_prep/power_prep  ${DESTDIR}

	cp -f linux_prep/output-target/linux_prep ${DESTDIR}

	cp -f *.sb ${DESTDIR}
#	to create finial mfg updater.sb
#	cp -f elftosb ${DESTDIR}
	cp -f ./updater*.bd ${DESTDIR}
	cp -f ./create_updater.sh  ${DESTDIR}

distclean: clean
clean:
	-rm -rf *.sb
	rm -f sd_mmc_bootstream.raw
	$(MAKE) -C linux_prep clean ARCH=$(ARCH)
	$(MAKE) -C boot_prep clean ARCH=$(ARCH)
	$(MAKE) -C power_prep clean ARCH=$(ARCH)

.PHONY: all build_prep linux_prep boot_prep power_prep distclean clean

