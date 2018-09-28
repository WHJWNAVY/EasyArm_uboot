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


#ifndef _HW_POWER_ERRORDEFS_H
#define _HW_POWER_ERRORDEFS_H

#include "hw/hw_errordefs.h"


#define ERROR_HW_DCDC_GENERAL                                    (ERROR_HW_DCDC_GROUP)
#define ERROR_HW_DCDC_INVALID_INPUT_PARAM                        (ERROR_HW_DCDC_GROUP + 1)


#define ERROR_HW_POWER_GROUP                                      (ERROR_HW_DCDC_GROUP)
#define ERROR_HW_POWER_GENERAL                                    (ERROR_HW_POWER_GROUP)
#define ERROR_HW_POWER_INVALID_POWER_FET_SETTING                  (ERROR_HW_POWER_GROUP + 0x1)
#define ERROR_HW_POWER_INVALID_LINREG_STEP                        (ERROR_HW_POWER_GROUP + 0x2)
#define ERROR_HW_POWER_UNSAFE_VDDD_VOLTAGE                        (ERROR_HW_POWER_GROUP + 0x3)
#define ERROR_HW_POWER_INVALID_VDDD_VOLTAGE                       (ERROR_HW_POWER_GROUP + 0x4)
#define ERROR_HW_POWER_INVALID_VDDD_BO_LEVEL                      (ERROR_HW_POWER_GROUP + 0x5)
#define ERROR_HW_POWER_UNSAFE_VDDA_VOLTAGE                        (ERROR_HW_POWER_GROUP + 0x6)
#define ERROR_HW_POWER_INVALID_VDDA_VOLTAGE                       (ERROR_HW_POWER_GROUP + 0x7)
#define ERROR_HW_POWER_INVALID_VDDA_BO_LEVEL                      (ERROR_HW_POWER_GROUP + 0x8)
#define ERROR_HW_POWER_UNSAFE_VDDIO_VOLTAGE                       (ERROR_HW_POWER_GROUP + 0x9)
#define ERROR_HW_POWER_INVALID_VDDIO_VOLTAGE                      (ERROR_HW_POWER_GROUP + 0x10)
#define ERROR_HW_POWER_INVALID_VDDIO_BO_LEVEL                     (ERROR_HW_POWER_GROUP + 0x11)
#define ERROR_HW_POWER_AUTO_XFER_TO_DCDC_ENABLED                  (ERROR_HW_POWER_GROUP + 0x12)
#define ERROR_HW_POWER_INVALID_BATT_MODE                          (ERROR_HW_POWER_GROUP + 0x13)
#define ERROR_HW_POWER_INVALID_INPUT_PARAM                        (ERROR_HW_POWER_GROUP + 0x14)
#define ERROR_HW_POWER_BAD_ARGUMENT                               (ERROR_HW_POWER_GROUP + 0x15)
#define ERROR_HW_POWER_4P2_ALREADY_ON                             (ERROR_HW_POWER_GROUP + 0x16)

#endif//_HW_POWER_ERRORDEFS_H
