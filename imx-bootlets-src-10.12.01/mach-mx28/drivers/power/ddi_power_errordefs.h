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

#ifndef _DDI_POWER_ERRORDEFS_H
#define _DDI_POWER_ERRORDEFS_H

#include "drivers/ddi_errordefs.h"


#define ERROR_DDI_POWER_GENERAL                                 (ERROR_DDI_POWER_GROUP)
#define ERROR_DDI_POWER_HCLK_SLOW_TOO_LOW                       (ERROR_DDI_POWER_GROUP + 1)
#define ERROR_DDI_POWER_HCLK_FAST_TOO_HIGH                      (ERROR_DDI_POWER_GROUP + 2)
#define ERROR_DDI_POWER_CANT_MEET_HCLK_CONSTRAINTS              (ERROR_DDI_POWER_GROUP + 3)
#define ERROR_DDI_POWER_PLL_OFF_BUT_NOT_BYPASSED                (ERROR_DDI_POWER_GROUP + 4)

#define ERROR_DDI_POWER_INVALID_PCLK_FREQ                       (ERROR_DDI_POWER_GROUP)
#define ERROR_DDI_POWER_INVALID_XCLK_FREQ                       (ERROR_DDI_POWER_GROUP + 1)
#define ERROR_DDI_POWER_INVALID_EMICLK_FREQ                     (ERROR_DDI_POWER_GROUP + 2)
#define ERROR_DDI_POWER_INVALID_POWER_SOURCE                    (ERROR_DDI_POWER_GROUP + 3)
#define ERROR_DDI_POWER_VDDD_PARAM_ADJUSTED                     (ERROR_DDI_POWER_GROUP + 4)
#define ERROR_DDI_POWER_VDDA_PARAM_ADJUSTED                     (ERROR_DDI_POWER_GROUP + 5)
#define ERROR_DDI_POWER_VDDIO_PARAM_ADJUSTED                    (ERROR_DDI_POWER_GROUP + 6)
#define ERROR_DDI_POWER_INVALID_PARAM                           (ERROR_DDI_POWER_GROUP + 7)
#define ERROR_DDI_FAILED_TO_START_4P2_RAIL                      (ERROR_DDI_POWER_GROUP + 8)
#define ERROR_DDI_FAILED_TO_PREPARE_4P2_RAIL                    (ERROR_DDI_POWER_GROUP + 9)
#define ERROR_DDI_POWER_BATT_TOO_LOW_FOR_HANDOFF                (ERROR_DDI_POWER_GROUP + 10)
#define ERROR_DDI_POWER_5V_LOST_DURING_4P2_CHARGE               (ERROR_DDI_POWER_GROUP + 11)
#endif//_DDI_POWER_ERRORDEFS_H




