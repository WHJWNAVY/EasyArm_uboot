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
#include "hw/power/hw_power_errordefs.h"
#include "registers/regspower.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
typedef struct _VoltVbusTable_t
{
    uint16_t Volt;
    hw_power_VbusValidThresh_t VbusThresh;
} VoltVbusTable_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! This array maps bit numbers to current increments, as used in the register
//! fields HW_POWER_CHARGE.STOP_ILIMIT and HW_POWER_CHARGE.BATTCHRG_I.

                                   // Bit: |0|  |1|  |2|  |3|  |4|  |5|
static const uint16_t  currentPerBit[] = {  10,  20,  50, 100, 200, 400 };

static const VoltVbusTable_t s_VoltVbusTable[] =
{
    { 2900, VBUS_VALID_THRESH_2900 },
    { 4000, VBUS_VALID_THRESH_4000 },
    { 4100, VBUS_VALID_THRESH_4100 },
    { 4200, VBUS_VALID_THRESH_4200 },
    { 4300, VBUS_VALID_THRESH_4300 },
    { 4400, VBUS_VALID_THRESH_4400 },
    { 4500, VBUS_VALID_THRESH_4500 },
    { 4600, VBUS_VALID_THRESH_4600 }
};

static uint16_t s_u16VoltVbusTableNumEntries =
	( sizeof( s_VoltVbusTable )/sizeof( VoltVbusTable_t ) );

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t  hw_power_ConvertCurrentToSetting(uint16_t u16Current)
{
    int       i;
    uint16_t  u16Mask;
    uint16_t  u16Setting = 0;

    // Scan across the bit field, adding in current increments.
    u16Mask = (0x1 << 5);

    for (i = 5; (i >= 0) && (u16Current > 0); i--, u16Mask >>= 1)
    {
        if (u16Current >= currentPerBit[i])
        {
            u16Current -= currentPerBit[i];
            u16Setting |= u16Mask;
        }
    }

    // Return the result.
    return(u16Setting);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t  hw_power_ConvertSettingToCurrent(uint16_t u16Setting)
{
    int       i;
    uint16_t  u16Mask;
    uint16_t  u16Current = 0;

    // Scan across the bit field, adding in current increments.
    u16Mask = (0x1 << 5);

    for (i = 5; i >= 0; i--, u16Mask >>= 1)
    {
        if (u16Setting & u16Mask) u16Current += currentPerBit[i];
    }

    // Return the result.
    return(u16Current);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertVdddToSetting(uint16_t u16Vddd)
{
    return ((u16Vddd - VDDD_BASE_MV)/25);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertSettingToVddd(uint16_t u16Setting)
{
    return ((u16Setting * 25) + VDDD_BASE_MV);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertVddaToSetting(uint16_t u16Vdda)
{
    return ((u16Vdda - VDDA_BASE_MV)/25);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertSettingToVdda(uint16_t u16Setting)
{
    return ((u16Setting * 25) + VDDA_BASE_MV);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertVddioToSetting(uint16_t u16Vddio)
{
    return ((u16Vddio - VDDIO_BASE_MV)/25);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertSettingToVddio(uint16_t u16Setting)
{
    return ((u16Setting * 25) + VDDIO_BASE_MV);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertBattToSetting(uint16_t u16Batt, uint16_t u16BattMode)
{
    // Li-Ion or alkaline battery mode.
    if( u16BattMode == 0 || u16BattMode == 1 )
    {
        return u16Batt / BATT_VAL_FIELD_RESOL;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertSettingToBatt(uint16_t u16Setting,
	uint16_t u16BattMode)
{
    // Li-Ion or alkaline battery mode.
    if( u16BattMode == 0 || u16BattMode == 1 )
    {
        return u16Setting * BATT_VAL_FIELD_RESOL;
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertBattBoToSetting(uint16_t u16BattBo)
{
    uint16_t LiIonEqnConst;

    // Calculate the equation constant.
      LiIonEqnConst =
	BATT_BRWNOUT_LIION_BASE_MV_378x - BATT_BRWNOUT_LIION_CEILING_OFFSET_MV;

        return ((u16BattBo - LiIonEqnConst) / BATT_BRWNOUT_LIION_LEVEL_STEP_MV);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertSettingToBattBo(uint16_t u16Setting)
{
    uint16_t LiIonBase;
    uint16_t AlkalBase;

    // Calculate the equation constant.
      LiIonBase = BATT_BRWNOUT_LIION_BASE_MV_378x;

        return ( u16Setting * BATT_BRWNOUT_LIION_LEVEL_STEP_MV ) + LiIonBase;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
hw_power_VbusValidThresh_t hw_power_ConvertVoltToVbusThresh(
	uint16_t u16ThreshVolt )
{
    uint8_t i;

    //--------------------------------------------------------------------------
    // Translate the voltage value to a register setting.  The threshold
    // voltage levels are taken from the datasheet.
    //
    // Table is sorted from smallest to largest value.  Use the first
    // setting that is just larger than the voltage level.
    //--------------------------------------------------------------------------
    i=0;
    while( i <= s_u16VoltVbusTableNumEntries )
    {
        // Is the setting larger than the requested voltage level?
        if( u16ThreshVolt <= s_VoltVbusTable[i].Volt )
        {
            // Yes, use this register setting.
            return s_VoltVbusTable[i].VbusThresh;
        }

        // No, this setting is less than the requested voltage level.
        else
        {
            // Are there more table entries to check?
            if( i < s_u16VoltVbusTableNumEntries )
            {
                // Yes, increment counter.
                i++;
            }
            else
            {
                // No, return the current setting.  It is the largest available.
                return s_VoltVbusTable[i].VbusThresh;
            }
        }
    }

    // The voltage value must be larger than our max, so use the max.
    return VBUS_VALID_THRESH_MAX;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertVbusThreshToVolt( hw_power_VbusValidThresh_t eThresh )
{
    uint8_t i;

    //--------------------------------------------------------------------------
    // Translate the register setting to a voltage level.  The threshold
    // voltage levels are taken from the datasheet.
    //
    // Table is sorted from smallest to largest value.
    //--------------------------------------------------------------------------
    i=0;
    while( i <= s_u16VoltVbusTableNumEntries )
    {
        // Does the requested threshold match the table entry?
        if( eThresh == s_VoltVbusTable[i].VbusThresh )
        {
            // Yes, use this voltage level.
            return s_VoltVbusTable[i].Volt;
        }

        // No match.
        else
        {
            // Are there more table entries to check?
            if( i < s_u16VoltVbusTableNumEntries )
            {
                // Yes, increment counter.
                i++;
            }
            else
            {
                // No, return zero value which is not a valid threshold.
                return 0;
            }
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}

