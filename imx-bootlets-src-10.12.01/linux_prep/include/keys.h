/*
 * Common keypad driver definitions
 *
 * Vladislav Buzov <vbuzov@embeddedalley.com>
 *
 * Copyright 2008 SigmaTel, Inc.
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
#ifndef __KEYS_H
#define __KEYS_H
#include <types.h>

enum keys_driver_id {
	KEYS_DRV_LRADC = 0,
	KEYS_DRV_GPIO,
	/* Add other keypad driver ids here */
	KEYS_DRV_NR,
	/* Fake ID, doesn't correspond any real driver */
	KEYS_DRV_DEFAULT,
};

struct keys_driver {
	enum keys_driver_id id;
	u32 (*get_keys) (void);
};

/* Register a keys driver */
void set_keys_driver(struct keys_driver *driver);

/* Get keys pressed */
u32 get_pressed_keys(void);

/*
 * KEY codes. Only 32 keys supported which should be
 * enough.
 */
#define KEY(n)		(1 << n)
#define KEY0		KEY(0)
#define KEY1		KEY(1)
#define KEY2		KEY(2)
#define KEY3		KEY(3)
#define KEY4		KEY(4)
#define KEY5		KEY(5)
#define KEY6		KEY(6)
#define KEY7		KEY(7)
#define KEY8		KEY(8)
#define KEY9		KEY(9)
#define KEY10		KEY(10)
#define KEY11		KEY(11)
#define KEY12		KEY(12)
#define KEY13		KEY(13)
#define KEY14		KEY(14)
#define KEY15		KEY(15)
#define KEY16		KEY(16)
#define KEY17		KEY(17)
#define KEY18		KEY(18)
#define KEY19		KEY(19)
#define KEY20		KEY(20)
#define KEY21		KEY(21)
#define KEY22		KEY(22)
#define KEY23		KEY(23)
#define KEY24		KEY(24)
#define KEY25		KEY(25)
#define KEY26		KEY(26)
#define KEY27		KEY(27)
#define KEY28		KEY(28)
#define KEY29		KEY(39)
#define KEY30		KEY(30)
#define KEY31		KEY(31)

#define NO_KEY		0

#endif /* _KEYS_H */
