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
// Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "error.h"

#include "hw/power/hw_power.h"
#include "hw/lradc/hw_lradc.h"
#include "hw/power/hw_power_errordefs.h"
#include "registers/regspower.h"

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
#pragma ghs section text=".static.text"
////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetBatteryBrownoutCode( uint16_t BoRegCode )
{
    BF_WR(POWER_BATTMONITOR, BRWNOUT_LVL, BoRegCode );
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetBatteryBrownoutCode( void )
{
    return BF_RD(POWER_BATTMONITOR, BRWNOUT_LVL);
}
#pragma ghs section text=default

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_SetBatteryBrownoutValue(int16_t i16BatteryBrownout_mV)
{
    uint16_t BattBoVolt;

    // Type cast to unsigned integer.  A negative brownout voltage is
    // not valid is this system.
    BattBoVolt = (uint16_t) i16BatteryBrownout_mV;

    // Set the value with the new API.
    hw_power_SetBatteryBrownoutVoltage( BattBoVolt );

    // Return success because we aren't going to do as much error checking
    // in the hw layer anymore.  (in the ddi_layer instead)
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetBatteryBrownoutValue(void)
{
    return hw_power_GetBatteryBrownoutVoltage();
}


////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetBatteryBrownoutVoltage( uint16_t BoVolt )
{
    uint16_t BoRegCode;
    uint16_t MaxBoRegCode;

    BoRegCode = hw_power_ConvertBattBoToSetting( BoVolt);

    // Verify the value is valid and set it.  Note that an invalid value
    // will not be set to the register.

    MaxBoRegCode = BATT_BRWNOUT_MAX_REG_CODE_378x;

    if ( BoRegCode <= MaxBoRegCode )
    {
        hw_power_SetBatteryBrownoutCode( BoRegCode );
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetBatteryBrownoutVoltage(void)
{
    uint16_t BoRegCode;

    // Collect parameters for the conversion.
    BoRegCode = hw_power_GetBatteryBrownoutCode();

    // Return the converted value.
    return hw_power_ConvertSettingToBattBo( BoRegCode);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetBatteryVoltage(void)
{
    uint16_t    u16BattVolt;

    // Get the raw result of battery measurement
    u16BattVolt = HW_POWER_BATTMONITOR.B.BATT_VAL;

    // Adjust for 8-mV LSB resolution and return
    return (u16BattVolt * BATT_VAL_FIELD_RESOL);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetBatteryMonitorVoltage(uint16_t u16BattVolt)
{
    uint16_t u16BattValue;

    // Adjust for 8-mV LSB resolution
    u16BattValue = u16BattVolt/BATT_VAL_FIELD_RESOL;

    // Write to register
    BF_WR(POWER_BATTMONITOR, BATT_VAL, u16BattValue);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_SetMaxBatteryChargeCurrent(uint16_t u16Current)
{
    uint16_t   u16OldSetting;
    uint16_t   u16NewSetting;
    uint16_t   u16ToggleMask;

    if(u16Current > 780)
        u16Current = 780;

    // Get the old setting.
    u16OldSetting = BF_RD(POWER_CHARGE, BATTCHRG_I);

    // Convert the new threshold into a setting.
    u16NewSetting = hw_power_ConvertCurrentToSetting(u16Current);

    // Compute the toggle mask.
    u16ToggleMask = u16OldSetting ^ u16NewSetting;

    // Write to the toggle register.
    HW_POWER_CHARGE_TOG(u16ToggleMask << BP_POWER_CHARGE_BATTCHRG_I);

    // Tell the caller what current we're set at now.
    return(hw_power_ConvertSettingToCurrent(u16NewSetting));
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t  hw_power_SetBatteryChargeCurrentThreshold(uint16_t u16Threshold)
{
    uint16_t   u16OldSetting;
    uint16_t   u16NewSetting;
    uint16_t   u16ToggleMask;

    //--------------------------------------------------------------------------
    // See hw_power_SetMaxBatteryChargeCurrent for an explanation of why we're
    // using the toggle register here.
    //
    // Since this function doesn't have any major hardware effect, we could use
    // the usual macros for writing to this bit field. But, for the sake of
    // parallel construction and any potentially odd effects on the status bit,
    // we use the toggle register in the same way as ddi_bc_hwSetMaxCurrent.
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // The threshold hardware can't express as large a range as the max
    // current setting, but we can use the same functions as long as we add an
    // extra check here.
    //
    // Thresholds larger than 180mA can't be expressed.
    //--------------------------------------------------------------------------

    if (u16Threshold > 180) u16Threshold = 180;

    ////////////////////////////////////////
    // Create the mask
    ////////////////////////////////////////

    // Get the old setting.
    u16OldSetting = BF_RD(POWER_CHARGE, STOP_ILIMIT);

    // Convert the new threshold into a setting.
    u16NewSetting = hw_power_ConvertCurrentToSetting(u16Threshold);

    // Compute the toggle mask.
    u16ToggleMask = u16OldSetting ^ u16NewSetting;

    // Shift to the correct bit position
    u16ToggleMask = BF_POWER_CHARGE_STOP_ILIMIT(u16ToggleMask);


    /////////////////////////////////////////
    // Write to the register
    /////////////////////////////////////////

    // Write to the toggle register.
    HW_POWER_CHARGE_TOG(u16ToggleMask);

    // Tell the caller what current we're set at now.
    return(hw_power_ConvertSettingToCurrent(u16NewSetting));
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetBatteryChargeCurrentThreshold(void)
{
    uint16_t u16Threshold;

    u16Threshold = BF_RD(POWER_CHARGE, STOP_ILIMIT);

    return hw_power_ConvertSettingToCurrent(u16Threshold);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetMaxBatteryChargeCurrent(void)
{
    uint8_t u8Bits;

    // Get the raw data from register
    u8Bits = BF_RD(POWER_CHARGE, BATTCHRG_I);

    // Translate raw data to current (in mA) and return it
    return hw_power_ConvertSettingToCurrent(u8Bits);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_GetDieTemperature(int16_t * pi16Low, int16_t * pi16High)
{
    int16_t i16High, i16Low;
    uint16_t u16Reading;

    // Temperature constant
    #define TEMP_READING_ERROR_MARGIN 5
    #define KELVIN_TO_CELSIUS_CONST 273

    // Get the reading in Kelvins
    u16Reading = hw_lradc_MeasureInternalDieTemperature(DIE_TEMP_CHAN_0,
	DIE_TEMP_CHAN_1);

    // Adjust for error margin
    i16High = u16Reading + TEMP_READING_ERROR_MARGIN;
    i16Low  = u16Reading - TEMP_READING_ERROR_MARGIN;

    // Convert to Celsius
    i16High -= KELVIN_TO_CELSIUS_CONST;
    i16Low  -= KELVIN_TO_CELSIUS_CONST;

    // Return the results
    *pi16High = i16High;
    *pi16Low  = i16Low;
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
