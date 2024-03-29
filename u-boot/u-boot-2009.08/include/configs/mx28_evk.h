/*
 * Copyright (C) 2010 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef __MX28_EVK_H
#define __MX28_EVK_H

#include <asm/arch/mx28.h>

/*
 * SoC configurations
 */
#define CONFIG_MX28				/* i.MX28 SoC */
#define CONFIG_MX28_TO1_2
#define CONFIG_SYS_HZ		1000		/* Ticks per second */
/* ROM loads UBOOT into DRAM */
#define CONFIG_SKIP_RELOCATE_UBOOT

/*
 * Memory configurations
 */
#define CONFIG_NR_DRAM_BANKS	1		/* 1 bank of DRAM */
#define PHYS_SDRAM_1		0x40000000	/* Base address */
#define PHYS_SDRAM_1_SIZE	0x08000000	/* 128 MB */
#define CONFIG_STACKSIZE	0x00020000	/* 128 KB stack */
#define CONFIG_SYS_MALLOC_LEN	0x00400000	/* 4 MB for malloc */
#define CONFIG_SYS_GBL_DATA_SIZE 128		/* Reserved for initial data */
#define CONFIG_SYS_MEMTEST_START 0x40000000	/* Memtest start address */
#define CONFIG_SYS_MEMTEST_END	 0x40400000	/* 4 MB RAM test */

/*
 * U-Boot general configurations
 */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_PROMPT	"MX28 U-Boot > "
#define CONFIG_SYS_CBSIZE	1024		/* Console I/O buffer size */
#define CONFIG_SYS_PBSIZE \
	(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
						/* Print buffer size */
#define CONFIG_SYS_MAXARGS	16		/* Max number of command args */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE
						/* Boot argument buffer size */
#define CONFIG_VERSION_VARIABLE			/* U-BOOT version */
#define CONFIG_AUTO_COMPLETE			/* Command auto complete */
#define CONFIG_CMDLINE_EDITING			/* Command history etc */

#define CONFIG_SYS_64BIT_VSPRINTF

/*
 * Boot Linux
 */
#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define CONFIG_BOOTDELAY	0
#define CONFIG_BOOTFILE		"uImage"
//#define CONFIG_BOOTARGS		"console=ttyAM0,115200n8 "	/* modefy by zhuguojun		*/
#define CONFIG_BOOTCOMMAND	"run nand_boot"
//#define CONFIG_BOOTCOMMAND	"run nand_boot"
//#define CONFIG_LOADADDR		0x42000000
#define CONFIG_LOADADDR		0x41600000	/* modefy by zhuguojun 使用64M的DDRAM后，必须退回10M地址才能写入大于32M的固件
						本来在0x40000000也可以写入内核，但不能写文件系统，还不知何故*/
#define CONFIG_SYS_LOAD_ADDR	CONFIG_LOADADDR

#define UBOOT_IMAGE_SIZE        0x50000  				/* 设置u-boot的大小		*/
#define MTDIDS_DEFAULT "nand0=nandflash0"
#if 0
#define MTDPARTS_DEFAULT "mtdparts=nandflash0:1536k(bootloder),"\
                                                "512k(params),"\
                                                "12m(kernel),"\
                                                "-(rootfs)"

#else 
#define MTDPARTS_DEFAULT "mtdparts=nandflash0:1536k(bootloder),"\
                                                "512k(params),"\
                                                "18m(kernel),"\
                                                "-(rootfs)"
#endif

#if 1
#define CONFIG_MTD_DEVICE       1 
#define CONFIG_MTD_PARTITIONS   1 
#define CONFIG_CMD_MTDPARTS 
#define CONFIG_CMD_UBIFS
#define CONFIG_CMD_UBI
#define CONFIG_LZO              1
#define CONFIG_RBTREE           1 
#endif

#if 1
/*
 1 Extra Environments
 */
#define	CONFIG_EXTRA_ENV_SETTINGS \
	     "kernel=uImage\0"                                               	\
        "kernelsize=0x300000\0"                                         	\
        "rootfs=rootfs.ubifs\0"                                         	\
         "kerneladdr="       "0x00200000\0"                             	\
	"nfsroot=/home/notroot/nfs/rootfs\0" 					\
	"bootargs_nfs=setenv bootargs ${bootargs} root=/dev/nfs " 		\
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp " 		\
		"fec_mac=${ethaddr}\0" 						\
	"bootcmd_net=run bootargs_nfs; dhcp; bootm\0" 				\
	"bootcmd_mmc=run bootargs_mmc; " 					\
		"mmc read 0 ${loadaddr} 100 3000; bootm\0" 			\
	"bootargs_nand=gpmi=g console=ttyAM0,115200n8 console=tty0 ubi.mtd=1 root=ubi0:rootfs rootfstype=ubifs ip=192.168.12.180:192.168.12.48:192.68.12.1:255.255.255.0::eht0: fec_mac= ethact \0			"			\
	"bootargs_mmc=gpmi=g console=ttyAM0,115200n8 console=tty0 root=/dev/mmcblk0p3 rw ip=192.168.12.180:192.168.12.48:192.68.12.1:255.255.255.0::eht0: fec_mac= ethact console=tty0 root=/dev/mmcblk0p3 rw ip=192.168.12.180:192.168.12.48:192.168.12.1:255.255.255.0::eht0: fec_mac= console=tty0  \0"	\
	"bootargs=gpmi=g console=ttyAM0,115200n8  ubi.mtd=1 root=ubi0:rootfs rootfstype=ubifs fec_mac= ethact mem=64M \0" 															\
	"kernel=uImage\0"														\
        "upuboot=tftp $(loadaddr) $(serverip):imx28_ivt_uboot.sb;nand erase 0x0 0x100000; nand write $(loadaddr) 0x0 0x100000\0"      	\
        "upkernel="         "tftp $(loadaddr) $(serverip):$(kernel);"                           	\
                            "nand erase  $(kerneladdr) $(kernelsize);"                     		\
                            "nand write $(loadaddr) $(kerneladdr) $(kernelsize);\0"       		\
                            "setenv kernelsize $(filesize); saveenv\0"                          	\
         "uprootfs="         "mtdparts default;"                                                	\
                                "nand erase rootfs;"                                            	\
                                "ubi part rootfs;"                                              	\
                                "ubi create  rootfs;"                                           	\
                                "tftp $(loadaddr) $(rootfs);"                                   	\
                                "ubi write $(loadaddr) rootfs $(filesize)\0"            		\
        "tftp_boot=tftp $(loadaddr) $(serverip):uImage; bootm;\0"                               	\
        "nand_boot=nand read.jffs2 $(loadaddr) $(kerneladdr) $(kernelsize);"                    	\
                                                                "bootm $(loadaddr)\0"           	\
        "setnandboot=setenv bootcmd 'run  nand_boot';saveenv \0"                                	\
        "settftpboot=setenv bootcmd 'run  tftp_boot';saveenv \0"                                	\
        "upsystem=run upkernel;run uprootfs;reset \0"   						\
	"ethaddr=02:00:92:b3:c4:a8 \0"
#else
/*
 * Extra Environments
 */
#define	CONFIG_EXTRA_ENV_SETTINGS \
	     "kernel=uImage\0"                                               	\
        "kernelsize=0x300000\0"                                         	\
        "rootfs=rootfs.ubifs\0"                                         	\
         "kerneladdr="       "0x00200000\0"                             	\
	"nfsroot=/home/notroot/nfs/rootfs\0" 					\
	"bootargs_nfs=setenv bootargs ${bootargs} root=/dev/nfs " 		\
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp " 		\
		"fec_mac=${ethaddr}\0" 						\
	"bootcmd_net=run bootargs_nfs; dhcp; bootm\0" 				\
	"bootargs_mmc=setenv bootargs console=ttyAM0,115200n8 console=tty0 root=/dev/mmcblk0p3 " 	\
		"rw rootwait ip=dhcp fec_mac=${ethaddr}\0" 			\
	"bootcmd_mmc=run bootargs_mmc; " 					\
		"mmc read 0 ${loadaddr} 100 3000; bootm\0" 			\
	"bootargs=gpmi=g console=ttyAM0,115200n8 console=tty0 root=/dev/mmcblk0p3 rw ip=192.168.12.180:192.168.12.48:192.68.12.1:255.255.255.0::eht0: fec_mac= ethact console=tty0 root=/dev/mmcblk0p3 rw ip=192.168.12.180:192.168.12.48:192.168.12.1:255.255.255.0::eht0: fec_mac= console=tty0 \0" 	\
	"kernel=uImage\0"														\
        "upuboot=tftp $(loadaddr) $(serverip):imx28_ivt_uboot.sb;nand erase 0x0 0x100000; nand write $(loadaddr) 0x0 0x100000\0"      	\
        "upkernel="         "tftp $(loadaddr) $(serverip):$(kernel);"                           	\
                            "nand erase  $(kerneladdr) $(kernelsize);"                     		\
                            "nand write $(loadaddr) $(kerneladdr) $(kernelsize);\0"       		\
                            "setenv kernelsize $(filesize); saveenv\0"                          	\
         "uprootfs="         "mtdparts default;"                                                	\
                                "nand erase rootfs;"                                            	\
                                "ubi part rootfs;"                                              	\
                                "ubi create  rootfs;"                                           	\
                                "tftp $(loadaddr) $(rootfs);"                                   	\
                                "ubi write $(loadaddr) rootfs $(filesize)\0"            		\
        "bootargs=ubi.mtd=0 root=ubi0:rootfs rootfstype=ubifs console=ttymxc0,115200 mem=64M;\0" 	\
        "tftp_boot=tftp $(loadaddr) $(serverip):uImage; bootm;\0"                               	\
        "nand_boot=nand read.jffs2 $(loadaddr) $(kerneladdr) $(kernelsize);"                    	\
                                                                "bootm $(loadaddr)\0"           	\
        "setnandboot=setenv bootcmd 'run  nand_boot';saveenv \0"                                	\
        "settftpboot=setenv bootcmd 'run  tftp_boot';saveenv \0"                                	\
        "upsystem=run upkernel;run uprootfs;reset \0"   

#endif

/*
 * U-Boot Commands
 */
#define CONFIG_SYS_NO_FLASH
#include <config_cmd_default.h>
#define CONFIG_ARCH_CPU_INIT
#define CONFIG_DISPLAY_CPUINFO

/*
 * Serial Driver
 */
#define CONFIG_UARTDBG_CLK		24000000
#define CONFIG_BAUDRATE			115200		/* Default baud rate */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/*
 * FEC Driver
 */
#define CONFIG_MXC_FEC
#define CONFIG_GET_FEC_MAC_ADDR_FROM_IIM
#define CONFIG_FEC0_IOBASE		REGS_ENET_BASE
#define CONFIG_FEC0_PHY_ADDR		5				
#define CONFIG_NET_MULTI
#define CONFIG_ETH_PRIME
#define CONFIG_RMII
#define CONFIG_CMD_MII
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_PING
#define CONFIG_IPADDR			192.168.12.62				//modefy by 朱国军
#define CONFIG_SERVERIP			192.168.12.61				//modefy by 朱国军
#define CONFIG_NETMASK			255.255.255.0
/* Add for working with "strict" DHCP server */
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_DNS

/*
 * MMC Driver
 */
#define CONFIG_CMD_MMC

#ifdef CONFIG_CMD_MMC
	#define CONFIG_MMC
	#define CONFIG_IMX_SSP_MMC		/* MMC driver based on SSP */
	#define CONFIG_GENERIC_MMC
	#define CONFIG_DYNAMIC_MMC_DEVNO
	#define CONFIG_DOS_PARTITION
	#define CONFIG_CMD_FAT
	#define CONFIG_SYS_SSP_MMC_NUM 2
#endif

/*
 * GPMI Nand Configs
 */
//#ifndef CONFIG_CMD_MMC	/* NAND conflict with MMC */

#define CONFIG_CMD_NAND

#ifdef CONFIG_CMD_NAND
	#define CONFIG_NAND_GPMI
	#define CONFIG_GPMI_NFC_SWAP_BLOCK_MARK
	#define CONFIG_GPMI_NFC_V1

	#define CONFIG_GPMI_REG_BASE	GPMI_BASE_ADDR
	#define CONFIG_BCH_REG_BASE	BCH_BASE_ADDR

	#define NAND_MAX_CHIPS		8
	#define CONFIG_SYS_NAND_BASE		0x40000000
	#define CONFIG_SYS_MAX_NAND_DEVICE	1
#endif

/*
 * APBH DMA Configs
 */
#define CONFIG_APBH_DMA

#ifdef CONFIG_APBH_DMA
	#define CONFIG_APBH_DMA_V1
	#define CONFIG_MXS_DMA_REG_BASE ABPHDMA_BASE_ADDR
#endif

//#endif

/*
 * Environments
 */
#define CONFIG_FSL_ENV_IN_MMC
//#undef CONFIG_FSL_ENV_IN_MMC
//#define CONFIG_FSL_ENV_IN_NAND		//设置环境变量保存在NAND

#define CONFIG_CMD_ENV
#define CONFIG_ENV_OVERWRITE

#if defined(CONFIG_FSL_ENV_IN_NAND)
	#define CONFIG_ENV_IS_IN_NAND 1
//	#define CONFIG_ENV_OFFSET	0x1400000 /* Nand env, offset: 20M */
	#define CONFIG_ENV_OFFSET	0x100000 /* Nand env, offset: 1.8M  modefy by zhuguojun */
	#define CONFIG_ENV_SECT_SIZE    (128 * 1024)
	#define CONFIG_ENV_SIZE         CONFIG_ENV_SECT_SIZE
#elif defined(CONFIG_FSL_ENV_IN_MMC)
	#define CONFIG_ENV_IS_IN_MMC	1
	/* Assoiated with the MMC layout defined in mmcops.c */
	#define CONFIG_ENV_OFFSET               (0x400) /* 1 KB */
	#define CONFIG_ENV_SIZE                 (0x20000 - 0x400) /* 127 KB */
#else
	#define CONFIG_ENV_IS_NOWHERE	1
#endif

/* The global boot mode will be detected by ROM code and
 * a boot mode value will be stored at fixed address:
 * TO1.0 addr 0x0001a7f0
 * TO1.2 addr 0x00019BF0
 */
#ifndef MX28_EVK_TO1_0
 #define GLOBAL_BOOT_MODE_ADDR 0x00019BF0
#else
 #define GLOBAL_BOOT_MODE_ADDR 0x0001a7f0
#endif
#define BOOT_MODE_SD0 0x9
#define BOOT_MODE_SD1 0xa

#endif /* __MX28_EVK_H */
