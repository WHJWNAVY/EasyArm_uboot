/*
 * Special macro to mark a function to be called during linux_prep
 * initialization stage. Every HW driver should define such funtion,
 * so it will be initialized if enabled in current linux_prep
 * configuration.
 *
 * Vladislav Buzov <vbuzov@embeddedalley.com>
 *
 * Copyright 2008 SigmaTel, Inc
 * Copyright 2008 Embedded Alley Solutions, Inc
 * Copyright 2009 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program  is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
#ifndef __INIT_H
#define __INIT_H

typedef void (*hwinitcall_t)(void);

#define hw_initcall(fn) \
	static hwinitcall_t __hwinitcall_##fn __attribute__((__used__)) \
	__attribute__((__section__(".hwinitcall"))) = fn

#endif /* __INIT_H */
