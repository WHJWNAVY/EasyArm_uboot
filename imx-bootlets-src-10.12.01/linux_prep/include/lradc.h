/*
 * Common LRADC interface definitions
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
#ifndef _LRADC_H
#define _LRADC_H

#define LRADC_CH0		0
#define LRADC_CH1		1
#define LRADC_CH2		2
#define LRADC_CH3		3
#define LRADC_CH4		4
#define LRADC_CH5		5
#define LRADC_CH6		6
#define LRADC_CH7		7
#define LRADC_CH10		10
#define LRADC_TOUCH_X_PLUS	LRADC_CH2
#define LRADC_TOUCH_Y_PLUS	LRADC_CH3
#define LRADC_TOUCH_X_MINUS	LRADC_CH4
#define LRADC_TOUCH_Y_MINUS	LRADC_CH5
#define VDDIO_VOLTAGE_CH	LRADC_CH6

#define BATTERY_VOLTAGE_CH	LRADC_CH7

#define LRADC_CLOCK_6MHZ	0
#define LRADC_CLOCK_4MHZ	1
#define LRADC_CLOCK_3MHZ	2
#define LRADC_CLOCK_2MHZ	3

#define LRADC_DELAY_TRIGGER_BUTTON	0
#define LRADC_DELAY_TRIGGER_BATTERY	1
#define LRADC_DELAY_TRIGGER_TOUCHSCREEN	2

#endif /* _LRADC_H */
