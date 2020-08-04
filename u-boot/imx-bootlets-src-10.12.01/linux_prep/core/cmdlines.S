/*
 * Kernel command lines placeholder code
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
#ifndef CMDLINES_FILE
#define CMDLINES_FILE "output-target/command_lines_stripped.txt"
#endif

	.section .cmdlines, #alloc
	.globl	cmdlines_start
cmdlines_start:
	.incbin	CMDLINES_FILE
	.globl	cmdlines_end
cmdlines_end:
