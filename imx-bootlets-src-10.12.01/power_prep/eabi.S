/*
 * Copyright 2009 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*
 * ARM EABI toolchain needs divide by zero handler to be implemented
 * externally.
 */
	.globl	__div0
__div0:
	mov	pc, lr
	.globl	__aeabi_unwind_cpp_pr0
__aeabi_unwind_cpp_pr0:
	mov	pc, lr
	.globl	raise
raise:
	mov	pc, lr
