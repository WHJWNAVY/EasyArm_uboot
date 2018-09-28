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

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "hw/lradc/hw_lradc.h"
#include "registers/regs.h"
#include "registers/regslradc.h"
#include "hw/power/hw_power.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////////
bool bHwLradcInitialized = false;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_Init(bool bEnableOnChipGroundRef,  hw_lradc_ClockFreq_t eFreq)
{
    if( bHwLradcInitialized == true )
    {
        return;
    }

    // Clear the Soft Reset for normal operation
    BF_CLR(LRADC_CTRL0, SFTRST);

    // Clear the Clock Gate for normal operation
    BF_CLR(LRADC_CTRL0, CLKGATE);      // use bitfield clear macro

    // Set on-chip ground ref
    if(bEnableOnChipGroundRef)
    {
        // Enable the onchip ground ref of LRADC conversions
        BF_SET( LRADC_CTRL0, ONCHIP_GROUNDREF);
    }
    else
    {
        // Disable the onchip ground ref of LRADC conversions
        BF_CLR( LRADC_CTRL0, ONCHIP_GROUNDREF);
    }

    // Set LRADC conversion clock frequency
    BF_WR(LRADC_CTRL3, CYCLE_TIME, eFreq);

    // Make sure the LRADC channel-6 selected for VDDIO
    BF_WR(LRADC_CTRL4, LRADC6SELECT, VDDIO_VOLTAGE_CH);

    // Make sure the LRADC channel-7 selected for Battery
    BF_WR(LRADC_CTRL4, LRADC7SELECT, BATTERY_VOLTAGE_CH);

    bHwLradcInitialized = true;
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
