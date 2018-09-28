/*
 * High level part of keypad driver
 *
 * Manages all low-level keypad drivers (e.g. LRADC keypad
 * GPIO keypad)
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
#include <keys.h>

/* Default keys driver */
static u32 get_no_keys (void)
{
	return NO_KEY;
}

static struct keys_driver default_keys_driver = {
	.id		= KEYS_DRV_DEFAULT,
	.get_keys	= get_no_keys,
};

static struct keys_driver *keys_drivers[KEYS_DRV_NR] = {
	[0 ... KEYS_DRV_NR-1] = &default_keys_driver,
};

/*
 * External interface to detect pressed keys
 */
u32 get_pressed_keys(void)
{
	int drv;
	u32 keys = NO_KEY;

	/* Go over all available HW keypads to get their pressed keys */
	for (drv = KEYS_DRV_LRADC; drv < KEYS_DRV_NR; drv++)
		keys |= keys_drivers[drv]->get_keys();

	return keys;
}

/*
 * External interface to let HW drivers register their magic key
 * detection callbacks.
 */
void set_keys_driver (struct keys_driver *driver)
{
	keys_drivers[driver->id] = driver;
}
