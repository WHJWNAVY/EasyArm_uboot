/*
 * STMP37XX specific definitions
 *
 * Vladislav Buzov <vbuzov@embeddedalley.com>
 *
 * Copyright 2008 SigmaTel, Inc
 * Copyright 2008 Embedded Alley Solutions, Inc
 * Copyright 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program  is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
#ifndef __37XX_PLATFORM_H
#define __37XX_PLATFORM_H

#if defined (BOARD_MX28_EVK)
#define	MACHINE_ID	2531
#else
#error "Allocate a machine ID for your board"
#endif

#define OCRAM_BASE	0x00000000
#define OCRAM_SIZE	0x00020000

#define FLASH_BASE	0x70000000
#define FLASH_SIZE	0x00080000

#define SDRAM_BASE	0x40000000

#define SDRAM_SIZE	0x08000000

#define REGS_BASE	0x80000000
#define REGS_SIZE	0x00100000

#define ATAGS_BASE_ADDRESS	(SDRAM_BASE + 0x100)
#define KERNEL_BASE_ADDRESS	(SDRAM_BASE + 0x8000)

/*
 * STMP 37xx RTC persistent register 1 bit 10 indicates
 * that system is being resumed from suspend mode
 */
#define RTC_BASE_ADDR		0x80056000
#define PERSISTENT_SLEEP_REG	0x80056070
#define PERSISTENT_SLEEP_BIT	10

#define SLEEP_STATE_FINGERPRINT	0xdeadbeef
#define FINGERPRINT		0x00		/* fingerprint offset */
#endif /* __37XX_PLATFROM_H */
