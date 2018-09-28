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

#ifndef _HW_SUBGROUPS_H
#define _HW_SUBGROUPS_H

#include "groups.h"

#define HW_MMU_GROUP              (HW_GROUP|0x00000000)
#define HW_IRQ_GROUP              (HW_GROUP|0x00001000)
#define HW_CACHE_GROUP            (HW_GROUP|0x00002000)
#define HW_USB_PHY_GROUP          (HW_GROUP|0x00004000)
#define HW_ICOLL_GROUP            (HW_GROUP|0x00005000)
#define HW_PLL_GROUP              (HW_GROUP|0x00006000)
#define HW_TIMER_GROUP            (HW_GROUP|0x00007000)
#define HW_PWM_OUTPUT_GROUP       (HW_GROUP|0x00008000)
#define HW_UART_DEBUG_GROUP       (HW_GROUP|0x00009000)
#define HW_UARTAPP_GROUP          (HW_GROUP|0x0000A000)
#define HW_EMI_GROUP              (HW_GROUP|0x0000B000)
#define HW_SSP_GROUP              (HW_GROUP|0x0000E000)
#define HW_I2C_GROUP              (HW_GROUP|0x0000F000)
#define HW_DAC_GROUP              (HW_GROUP|0x00011000)
#define HW_ADC_GROUP              (HW_GROUP|0x00012000)
#define HW_RTC_GROUP              (HW_GROUP|0x00014000)
#define HW_ALARM_GROUP            (HW_GROUP|0x00015000)
#define HW_WATCHDOG_GROUP         (HW_GROUP|0x00016000)
#define HW_LRADC_GROUP            (HW_GROUP|0x00017000)
#define HW_GPMI_GROUP             (HW_GROUP|0x00018000)
#define HW_CLOCKS_GROUP           (HW_GROUP|0x0001A000)
#define HW_DRI_GROUP              (HW_GROUP|0x0001B000)
#define HW_HWECC_GROUP            (HW_GROUP|0x00019000)
#define HW_LCDIF_GROUP            (HW_GROUP|0x0000C000)
#define HW_DCDC_GROUP             (HW_GROUP|0x0000D000)
#define HW_DCP_GROUP              (HW_GROUP|0x0001C000)
#define HW_PINMUX_GROUP           (HW_GROUP|0x0001D000)

#endif//_HW_SUBGROUPS_H
