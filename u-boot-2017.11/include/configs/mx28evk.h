/*
 * (C) Copyright 2011 Freescale Semiconductor, Inc.
 * Author: Fabio Estevam <fabio.estevam@freescale.com>
 *
 * Based on m28evk.h:
 * Copyright (C) 2011 Marek Vasut <marek.vasut@gmail.com>
 * on behalf of DENX Software Engineering GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIGS_MX28EVK_H__
#define __CONFIGS_MX28EVK_H__

/* System configurations */
#define CONFIG_MX28				/* i.MX28 SoC */
#define CONFIG_MACH_TYPE	MACH_TYPE_MX28EVK

/* BEGIN: Added by wnavy, 2018/9/28 */
/*#define DEBUG*/
#define CONFIG_SYS_ICACHE_OFF  /*Do not enable instruction cache in U-Boot*/
#define CONFIG_SYS_DCACHE_OFF  /*Do not enable data cache in U-Boot*/
/*#define CONFIG_SYS_L2CACHE_OFF*/ /*Do not enable L2 cache in U-Boot*/
/* END:   Added by wnavy, 2018/9/28 */

/* Memory configuration */
#define CONFIG_NR_DRAM_BANKS		1		/* 1 bank of DRAM */
#define PHYS_SDRAM_1			0x40000000	/* Base address */
#define PHYS_SDRAM_1_SIZE		0x08000000	/* Max 1 GB RAM */
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1

/* Environment */
#ifndef CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SIZE			(16 * 1024)
#else
#define CONFIG_ENV_SIZE			(4 * 1024)
#endif
#define CONFIG_ENV_OVERWRITE

/* Environment is in MMC */
#if defined(CONFIG_CMD_MMC) && defined(CONFIG_ENV_IS_IN_MMC)
/* BEGIN: Modified by wnavy, 2018/9/28 */
/*#define CONFIG_ENV_OFFSET		(256 * 1024)*/
#define CONFIG_ENV_OFFSET		(1 * 1024) /*1KB*/
#ifdef CONFIG_ENV_SIZE
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE         (127 * 1024) /*127KB*/
#endif
/* END:   Modified by wnavy, 2018/9/28 */
#define CONFIG_SYS_MMC_ENV_DEV		0
#endif

/* Environment is in NAND */
#if defined(CONFIG_CMD_NAND) && defined(CONFIG_ENV_IS_IN_NAND)
#define CONFIG_ENV_SIZE_REDUND		CONFIG_ENV_SIZE
#define CONFIG_ENV_SECT_SIZE		(128 * 1024)
#define CONFIG_ENV_RANGE		(512 * 1024)
#define CONFIG_ENV_OFFSET		0x300000
#define CONFIG_ENV_OFFSET_REDUND	\
		(CONFIG_ENV_OFFSET + CONFIG_ENV_RANGE)
#endif

/* Environemnt is in SPI flash */
#if defined(CONFIG_CMD_SF) && defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_ENV_OFFSET		0x40000		/* 256K */
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
#define CONFIG_ENV_SECT_SIZE		0x1000
#define CONFIG_ENV_SPI_CS		0
#define CONFIG_ENV_SPI_BUS		2
#define CONFIG_ENV_SPI_MAX_HZ		24000000
#define CONFIG_ENV_SPI_MODE		SPI_MODE_0
#endif

/* UBI and NAND partitioning */
#ifdef CONFIG_CMD_NAND
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#endif

/* FEC Ethernet on SoC */
#ifdef	CONFIG_CMD_NET
#define CONFIG_FEC_MXC
#define CONFIG_MX28_FEC_MAC_IN_OCOTP
/* BEGIN: Added by wnavy, 2018/9/28 */
#define CONFIG_IPADDR           192.168.1.101
#define CONFIG_SERVERIP         192.168.1.100
#define CONFIG_GATEWAYIP        192.168.1.1
#define CONFIG_NETMASK          255.255.255.0
/* END:   Added by wnavy, 2018/9/28 */
#endif

/* RTC */
#ifdef	CONFIG_CMD_DATE
#define	CONFIG_RTC_MXS
#endif

/* USB */
#ifdef	CONFIG_CMD_USB
#define CONFIG_EHCI_MXS_PORT1
#define CONFIG_USB_MAX_CONTROLLER_COUNT	1
#endif

/* SPI */
#ifdef CONFIG_CMD_SPI
#define CONFIG_DEFAULT_SPI_BUS		2
#define CONFIG_DEFAULT_SPI_MODE		SPI_MODE_0

/* SPI Flash */
#ifdef CONFIG_CMD_SF
#define CONFIG_SF_DEFAULT_BUS		2
#define CONFIG_SF_DEFAULT_CS		0
/* this may vary and depends on the installed chip */
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
#define CONFIG_SF_DEFAULT_SPEED		24000000
#endif

#endif

/* Framebuffer support */
#ifdef CONFIG_VIDEO
#define CONFIG_VIDEO_LOGO
#define CONFIG_SPLASH_SCREEN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_GZIP
#define CONFIG_SYS_VIDEO_LOGO_MAX_SIZE	(512 << 10)
#endif

/* Boot Linux */
#define CONFIG_BOOTFILE		"uImage"
#if 0
#define CONFIG_LOADADDR		0x42000000
#else
#define CONFIG_LOADADDR		0x41600000
#endif
#define CONFIG_SYS_LOAD_ADDR	CONFIG_LOADADDR

/* Extra Environment */
/* BEGIN: Modified by wnavy, 2018/9/28 */
#if 0
#define CONFIG_EXTRA_ENV_SETTINGS \
	"ubifs_file=filesystem.ubifs\0" \
	"update_nand_full_filename=u-boot.nand\0" \
	"update_nand_firmware_filename=u-boot.sb\0"	\
	"update_nand_firmware_maxsz=0x100000\0"	\
	"update_nand_stride=0x40\0"	/* MX28 datasheet ch. 12.12 */ \
	"update_nand_count=0x4\0"	/* MX28 datasheet ch. 12.12 */ \
	"update_nand_get_fcb_size="	/* Get size of FCB blocks */ \
		"nand device 0 ; " \
		"nand info ; " \
		"setexpr fcb_sz ${update_nand_stride} * ${update_nand_count};" \
		"setexpr update_nand_fcb ${fcb_sz} * ${nand_writesize}\0" \
	"update_nand_firmware_full=" /* Update FCB, DBBT and FW */ \
		"if tftp ${update_nand_full_filename} ; then " \
		"run update_nand_get_fcb_size ; " \
		"nand scrub -y 0x0 ${filesize} ; " \
		"nand write.raw ${loadaddr} 0x0 ${fcb_sz} ; " \
		"setexpr update_off ${loadaddr} + ${update_nand_fcb} ; " \
		"setexpr update_sz ${filesize} - ${update_nand_fcb} ; " \
		"nand write ${update_off} ${update_nand_fcb} ${update_sz} ; " \
		"fi\0" \
	"update_nand_firmware="		/* Update only firmware */ \
		"if tftp ${update_nand_firmware_filename} ; then " \
		"run update_nand_get_fcb_size ; " \
		"setexpr fcb_sz ${update_nand_fcb} * 2 ; " /* FCB + DBBT */ \
		"setexpr fw_sz ${update_nand_firmware_maxsz} * 2 ; " \
		"setexpr fw_off ${fcb_sz} + ${update_nand_firmware_maxsz};" \
		"nand erase ${fcb_sz} ${fw_sz} ; " \
		"nand write ${loadaddr} ${fcb_sz} ${filesize} ; " \
		"nand write ${loadaddr} ${fw_off} ${filesize} ; " \
		"fi\0" \
	"update_nand_kernel="		/* Update kernel */ \
		"mtdparts default; " \
		"nand erase.part kernel; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${image}; " \
		"nand write ${loadaddr} kernel ${filesize}\0" \
	"update_nand_fdt="		/* Update fdt */ \
		"mtdparts default; " \
		"nand erase.part fdt; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${fdt_file}; " \
		"nand write ${loadaddr} fdt ${filesize}\0" \
	"update_nand_filesystem="		/* Update filesystem */ \
		"mtdparts default; " \
		"nand erase.part filesystem; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${ubifs_file}; " \
		"ubi part filesystem; " \
		"ubi create filesystem; " \
		"ubi write ${loadaddr} filesystem ${filesize}\0" \
	"nandargs=setenv bootargs console=${console_mainline},${baudrate} " \
		"rootfstype=ubifs ubi.mtd=6 root=ubi0_0 ${mtdparts}\0" \
	"nandboot="		/* Boot from NAND */ \
		"mtdparts default; " \
		"run nandargs; " \
		"nand read ${loadaddr} kernel 0x00400000; " \
		"if test ${boot_fdt} = yes; then " \
			"nand read ${fdt_addr} fdt 0x00080000; " \
			"bootz ${loadaddr} - ${fdt_addr}; " \
		"else " \
			"if test ${boot_fdt} = no; then " \
				"bootz; " \
			"else " \
				"echo \"ERROR: Set boot_fdt to yes or no.\"; " \
			"fi; " \
		"fi\0" \
	"update_sd_firmware_filename=u-boot.sd\0" \
	"update_sd_firmware="		/* Update the SD firmware partition */ \
		"if mmc rescan ; then "	\
		"if tftp ${update_sd_firmware_filename} ; then " \
		"setexpr fw_sz ${filesize} / 0x200 ; "	/* SD block size */ \
		"setexpr fw_sz ${fw_sz} + 1 ; "	\
		"mmc write ${loadaddr} 0x800 ${fw_sz} ; " \
		"fi ; "	\
		"fi\0" \
	"script=boot.scr\0"	\
	"image=zImage\0" \
	"console_fsl=ttyAM0\0" \
	"console_mainline=ttyAMA0\0" \
	"fdt_file=imx28-evk.dtb\0" \
	"fdt_addr=0x41000000\0" \
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"mmcdev=0\0" \
	"mmcpart=2\0" \
	"mmcroot=/dev/mmcblk0p3 rw rootwait\0" \
	"mmcargs=setenv bootargs console=${console_mainline},${baudrate} " \
		"root=${mmcroot}\0" \
	"loadbootscript="  \
		"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; "	\
		"source\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"netargs=setenv bootargs console=${console_mainline},${baudrate} " \
		"root=/dev/nfs " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; "	\
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${image}; " \
		"if test ${boot_fdt} = yes; then " \
			"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi;" \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0"

#define CONFIG_BOOTCOMMAND \
	"mmc dev ${mmcdev}; if mmc rescan; then " \
		"if run loadbootscript; then " \
			"run bootscript; " \
		"else " \
			"if run loadimage; then " \
				"run mmcboot; " \
			"else run netboot; " \
			"fi; " \
		"fi; " \
	"else run netboot; fi"
#else
#define CONFIG_EXTRA_ENV_SETTINGS \
    "kernel=uImage\0"                                                                           \
    "kernelsize=0x300000\0"                                                                     \
    "rootfs=rootfs.ubifs\0"                                                                     \
    "kerneladdr=0x00200000\0"                                                                   \
    "nfsroot=/home/notroot/nfs/rootfs\0"                                                        \
    "bootargs_nfs=setenv bootargs ${bootargs} root=/dev/nfs "                                   \
        "ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp "                                        \
        "fec_mac=${ethaddr}\0"                                                                  \
    "bootcmd_net=run bootargs_nfs; dhcp; bootm\0"                                               \
    "bootcmd_mmc=run bootargs_mmc; "                                                            \
        "mmc read 0 ${loadaddr} 100 3000; bootm\0"                                              \
    "bootargs_nand=gpmi=g console=ttyAM0,115200n8 console=tty0 ubi.mtd=1 "                      \
        "root=ubi0:rootfs rootfstype=ubifs "                                                    \
        "ip=192.168.1.101:192.168.1.100:192.68.1.1:255.255.255.0::eht0: "                     \
        "fec_mac= ethact \0          "                                                          \
    "bootargs_mmc=gpmi=g console=ttyAM0,115200n8 console=tty0 root=/dev/mmcblk0p3 "             \
        "rw ip=192.168.1.101:192.168.1.100:192.68.1.1:255.255.255.0::eht0: "                  \
        "fec_mac= ethact console=tty0 root=/dev/mmcblk0p3 rw "                                  \
        "ip=192.168.1.101:192.168.1.100:192.168.1.1:255.255.255.0::eht0: "                    \
        "fec_mac= console=tty0  \0"                                                             \
    "bootargs_sdcard=gpmi=g console=ttyAM0,115200n8 root=/dev/mmcblk0p3 "                       \
        "rw rootwait rootfstype=ext2 init=/sbin/init fec_mac= ethact mem=64M\0"                 \
    "bootargs=gpmi=g console=ttyAM0,115200n8 root=/dev/mmcblk0p3 "                              \
        "rw rootwait rootfstype=ext2 init=/sbin/init fec_mac= ethact mem=64M\0"                 \
    "upuboot=tftp ${loadaddr} ${serverip}:imx28_ivt_uboot.sb;nand erase 0x0 0x100000; "         \
        "nand write ${loadaddr} 0x0 0x100000\0"                                                 \
    "upkernel="     "tftp ${loadaddr} ${serverip}:${kernel};"                                   \
                    "nand erase  ${kerneladdr} ${kernelsize};"                                  \
                    "nand write ${loadaddr} ${kerneladdr} ${kernelsize};\0"                     \
                    "setenv kernelsize ${filesize}; saveenv\0"                                  \
    "uprootfs="     "mtdparts default;"                                                         \
                    "nand erase rootfs;"                                                        \
                    "ubi part rootfs;"                                                          \
                    "ubi create  rootfs;"                                                       \
                    "tftp $(loadaddr) ${rootfs};"                                               \
                    "ubi write ${loadaddr} rootfs ${filesize}\0"                                \
    "tftp_boot=tftp ${loadaddr} ${serverip}:uImage; bootm;\0"                                   \
    "nand_boot=nand read.jffs2 ${loadaddr} ${kerneladdr} ${kernelsize};"                        \
        "bootm ${loadaddr}\0"                                                                   \
    "sdcard_boot=fatload mmc 0:1 ${loadaddr} uImage;bootm\0"                                    \
    "setnandboot=setenv bootcmd 'run  nand_boot';saveenv \0"                                    \
    "settftpboot=setenv bootcmd 'run  tftp_boot';saveenv \0"                                    \
    "upsystem=run upkernel;run uprootfs;reset \0"

#define CONFIG_BOOTCOMMAND "run sdcard_boot"
#endif
/* END:   Modified by wnavy, 2018/9/28 */

/* The rest of the configuration is shared */
#include <configs/mxs.h>

#endif /* __CONFIGS_MX28EVK_H__ */
