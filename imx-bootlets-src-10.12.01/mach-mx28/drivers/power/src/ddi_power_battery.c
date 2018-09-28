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


//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "error.h"                  // Common SigmaTel Error Codes

#include "hw/power/hw_power.h"

#include "drivers/power/ddi_power.h"       // Driver API
#include "drivers/power/ddi_power_errordefs.h"

////////////////////////////////////////////////////////////////////////////////
// Globals & Variables
////////////////////////////////////////////////////////////////////////////////

//! This global variable contains a code that represents the "safe" battery
//! voltage. As long as the battery voltage is above this level, we believe it
//! is probably safe to perform any normal system function.
//!
//! If the battery voltage ever dips down to or below this level, we believe
//! that some future operation has an unacceptable probability of causing the
//! system to brown out and power down.
//!
//! In normal operation, if the battery voltage dips down to or below this
//! level, and the 5V supply is not present, then the FIQ handler will catch a
//! brown out interrupt and send a message to the PMI state machine.
//!
//! The actual voltage represented here depends on the battery mode. See
//! "Battery Level Monitor Register Description" in the data sheet for details.
uint8_t g_ddi_power_SafeBatteryVoltageCode = 23;


//! This global variable contains a code that represents the "brown out" battery
//! voltage. In normal operation, if the battery voltage dips down to or below
//! this level, and the 5V supply is not present, then the hardware will
//! immediately cut off the power.
//!
//! The actual voltage represented here depends on the battery mode. See
//! "Battery Level Monitor Register Description" in the data sheet for details.
uint8_t g_ddi_power_BatteryBrownOutVoltageCode = 14;



////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Battery Charger Status
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Battery Voltage
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Report the voltage across the battery.
//!
//! \fntype Function
//!
//! This function reports the voltage across the battery.
//!
//! \retval The voltage across the battery, in mV.
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t  ddi_power_GetBattery(void)
{
    // Should return a value in range ~3000 - 4200 mV
    return hw_power_GetBatteryVoltage();
}


////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
