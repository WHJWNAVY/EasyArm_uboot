/*
 * Linux resume code.
 * 
 * - Detects if CPU was in standby mode
 * - Resumes Linux kernel
 *
 * Vladislav Buzov <vbuzov@embeddedalley.com>
 *
 * Copyright 2008 SigmaTel, Inc
 * Copyright 2008 Embedded Alley Solutions, Inc
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program  is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */
#include <arch/platform.h>

#undef DEBUG

	.code 32

	.section ".start", "ax"
	.global try_to_resume

.macro	mov_imm		reg, imm
	mov	\reg, #(\imm & 0xFF000000)
	orr	\reg, \reg, #(\imm & 0x00FF0000)
	orr	\reg, \reg, #(\imm & 0x0000FF00)
	orr	\reg, \reg, #(\imm & 0x000000FF)
.endm

#ifdef DEBUG
.macro	printch		reg, out
	and \reg, \reg, #0xF
	cmp \reg, #9
	addls \reg, \reg, #'0'
	addhi \reg, \reg, #'A'
	str \reg, [\out]
.endm

.macro	printreg 	reg

	mov	r6,	#0x80000000
	orr	r6, 	r6, 	#0x70000

	mov 	r5, 	\reg,	 lsr #28
	printch	r5, r6
	mov 	r5, 	\reg,	 lsr #24
	printch	r5, r6
	mov 	r5, 	\reg,	 lsr #20
	printch	r5, r6
	mov 	r5, 	\reg,	 lsr #16
	printch	r5, r6
	mov 	r5, 	\reg,	 lsr #12
	printch	r5, r6
	mov 	r5, 	\reg,	 lsr #8
	printch	r5, r6
	mov 	r5, 	\reg,	 lsr #4
	printch	r5, r6
	mov 	r5, 	\reg,	 lsr #0
	printch	r5, r6
.endm

#else

.macro 	printch		reg, out
.endm

.macro printreg		reg
.endm

#endif	/* DEBUG */

try_to_resume:

	mov_imm	r1, RTC_BASE_ADDR
	ldr 	r2, [r1]
	tst	r2, #0xC0000000		@ check for SFTRST
	beq	2f			@ no reset bit? ok, continue
	bic	r2, #0xC0000000		@ clear both SFTRST and CLKGATE
	str	r2, [r1]		@ ... and save
1:
	ldr	r2, [r1]		@ check again
	tst	r2, #0xC0000000		@ still set?
	bne	1b			@ then loop

2:
	printreg r2

	/**
	   The code below checks the following conditions:
	   1. bit PERSISTENT_SLEEP_BIT in PERSISTEN_SLEEP_REG should be set
	   2. data at [SDRAM_BASE] should point to address within SDRAM
	   3. there is correct fingerprint at [[SDRAM_BASE] + 0x548]
	   If all of the conditions above are met, we assume resuming from
	   sleep mode - and jump to address at SDRAM_BASE+0x4. Else normal
	   boot process will continue.
        */
	mov	r0, 	#'0'

	/* Check if we are resuming from standby mode */
	mov_imm	r1, PERSISTENT_SLEEP_REG
	ldr	r2, [r1, #0]
	tst	r2, #1 << PERSISTENT_SLEEP_BIT
	printreg r2
	bxeq	lr			@ no PERSISTENT_SLEEP_BIT, return
	
	bic	r2, #1 << PERSISTENT_SLEEP_BIT
	str	r2, [r1, #0]		@ clear this bit

	ldr	r1, =SDRAM_BASE		@ RAM start
	ldr	r2, [r1, #0]		@ saved state

	add	r0, r0, #1
	cmp	r2, #SDRAM_BASE		@ some sanity check
	bxlt	lr			@ less than SDRAM_BASE ? baaad. return
	
	add	r0, r0, #1
	ldr	r1, [r2, #FINGERPRINT]	@ fingerprint offset
	ldr	r3, =SLEEP_STATE_FINGERPRINT
	cmp	r1, r3
	bxne	lr			

	ldr	r1, =SDRAM_BASE
	orr	r1, r1, #0x4
	ldr	pc, [r1, #0]

	.pool
