/*
 * LRADC keypad driver definitions
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

#ifndef _LRADC_BUTTONS_H
#define _LRADC_BUTTONS_H
#include <types.h>
#include <lradc.h>

/*
 * This stcuture describes a mapping between LRADC
 * input values and button codes for LRADC keypad
 */
struct lradc_keycode {
	u32 raw;
	u32 kcode;
};

/*
 * LRADC keypad channel and mapping are platform specific
 * and should be defined for each board with LRADC keypad
 */
extern int lradc_keypad_ch;
extern int lradc_vddio_ch;
extern struct lradc_keycode lradc_keycodes[];

#endif /* _LRADC_BUTTONS_H */
