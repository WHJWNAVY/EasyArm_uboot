/*
 * Board specific setup info
 *
 * Copyright (C) 2010 Freescale Semiconductor, Inc.
 *
 * (C) Copyright 2003, ARM Ltd.
 * Philippe Robin, <philippe.robin@arm.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#define EASY283 1
#include <config.h>
#include <version.h>


.macro REG reg, val
	ldr r2, =\reg
	ldr r3, =\val
	str r3, [r2]
.endm

.macro REG8 reg, val
	ldr r2, =\reg
	ldr r3, =\val
	strb r3, [r2]
.endm
/* Set up the platform, once the cpu has been initialized */
.globl lowlevel_init
lowlevel_init:
	/*
	config pin AURT0_CTS/RTS as DUART.TX/TX,
	and config pin PWM0/1 as I2C1_SCL/SDA
	*/
#if EASY283 
	REG 0x80018168,0x000000F0
	REG 0x80018164,0x000000A0
	REG 0x80018178,0x0000000F
	REG 0x80018174,0x00000005
#endif

	/* All SDRAM settings are done by sdram_prep */
	mov pc, lr
