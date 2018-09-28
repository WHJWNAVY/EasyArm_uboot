/*
 * Copyright 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
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
#include "hw/digctl/hw_digctl.h"
#include "registers/regspower.h"

//------------------------------------------------------------------------------
// Uncomment the following definition to display TSS messages about the
// external LDO status.
//------------------------------------------------------------------------------
//#define PRINT_LDO_STATUS

#ifdef PRINT_LDO_STATUS
#include "components/telemetry/tss_logtext.h"
#include "os/dpc/os_dpc_api.h"
#endif
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
hw_power_PowerSource_t VdddPowerSource;
bool bExternalPowerSourceActive = false;
bool bEnableMasterCharge = false;
bool bEnableMaster4p2 = false;

////////////////////////////////////////////////////////////////////////////////
// VDDD
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVdddValue(uint16_t u16Vddd_mV)
{
    uint16_t u16Vddd_Set;

    // Convert mV to register setting
    u16Vddd_Set = hw_power_ConvertVdddToSetting(u16Vddd_mV);

    // Use this way of writing to the register because we want a read, modify,
    // write operation.  Using the other available macros will call a clear/set
    // which momentarily clears the target field causing the voltage to dip
    // then rise again.  That should be avoided.
    HW_POWER_VDDDCTRL.B.TRG = u16Vddd_Set;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVdddValue(void)
{
    uint16_t    u16Vddd_Set;
    uint16_t    u16Vddd_mV;

    //Read VDDD bitfiled value
    u16Vddd_Set = HW_POWER_VDDDCTRL.B.TRG;

    //  Convert to mVolts
    u16Vddd_mV = hw_power_ConvertSettingToVddd(u16Vddd_Set);

    return u16Vddd_mV;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVdddBrownoutValue(uint16_t u16VdddBoOffset_mV)
{
    uint16_t u16VdddBoOffset_Set;

    // Convert millivolts to register setting (1 step = 25mV)
    u16VdddBoOffset_Set = u16VdddBoOffset_mV/(VOLTAGE_STEP_MV);

    // Use this way of writing to the register because we want a read, modify,
    // write operation.  Using the other available macros will call a clear/set
    // which momentarily makes the brownout offset zero which may trigger a
    // false brownout.
    HW_POWER_VDDDCTRL.B.BO_OFFSET = u16VdddBoOffset_Set;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVdddBrownoutValue(void)
{
    uint16_t u16VdddBoOffset_Set;
    uint16_t u16VdddBoOffset_mV;

    // Read the VDDD brownout offset field.
    u16VdddBoOffset_Set = HW_POWER_VDDDCTRL.B.BO_OFFSET;

    // Convert setting to millivolts. (1 step = 25mV)
    u16VdddBoOffset_mV = (u16VdddBoOffset_Set * VOLTAGE_STEP_MV);

    // Return the brownout offset in mV.
    return u16VdddBoOffset_mV;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_SetVdddPowerSource(hw_power_PowerSource_t eSource)
{
    RtStatus_t rtn = SUCCESS;
    VdddPowerSource = eSource;
    //--------------------------------------------------------------------------
    // The VDDD can use two power sources in three configurations: the Linear
    // Regulator, the DCDC with LinReg off, and the DCDC with LinReg on.
    // Each has its own configuration that must be set up to
    // prevent power rail instability.
    //--------------------------------------------------------------------------

    switch(eSource)
    {
        //----------------------------------------------------------------------
        // Power the VDDD rail from DCDC.  LinReg is off.
        //----------------------------------------------------------------------
        case HW_POWER_DCDC_LINREG_OFF:
        case HW_POWER_DCDC_LINREG_READY:

            //------------------------------------------------------------------
            // Use LinReg offset for DCDC mode.
            //------------------------------------------------------------------
            hw_power_SetVdddLinRegOffset(HW_POWER_LINREG_OFFSET_DCDC_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDD DCDC output and turn off the VDDD LinReg output.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDDCTRL, DISABLE_FET);
            BF_CLR(POWER_VDDDCTRL, ENABLE_LINREG);


            //------------------------------------------------------------------
            // Make sure stepping is enabled when using DCDC.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDDCTRL, DISABLE_STEPPING);


        break;


        //----------------------------------------------------------------------
        // Power the VDDD rail from DCDC.  LinReg is on.
        //----------------------------------------------------------------------
        case HW_POWER_DCDC_LINREG_ON:

            //------------------------------------------------------------------
            // Use LinReg offset for DCDC mode.
            //------------------------------------------------------------------
            hw_power_SetVdddLinRegOffset(HW_POWER_LINREG_OFFSET_DCDC_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDD DCDC output and turn on the VDDD LinReg output.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDDCTRL, DISABLE_FET);
            BF_SET(POWER_VDDDCTRL, ENABLE_LINREG);


            //------------------------------------------------------------------
            // Make sure stepping is enabled when using DCDC.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDDCTRL, DISABLE_STEPPING);


        break;


        //----------------------------------------------------------------------
        // Power the VDDD rail from the linear regulator.  The DCDC is not
        // set up to automatically power the chip if 5V is removed.  Use this
        // configuration when battery is powering the player, but we want to
        // use LinReg instead of DCDC.
        //----------------------------------------------------------------------
        case HW_POWER_LINREG_DCDC_OFF:

            //------------------------------------------------------------------
            // Use LinReg offset for LinReg mode.
            //------------------------------------------------------------------
            hw_power_SetVdddLinRegOffset(HW_POWER_LINREG_OFFSET_LINREG_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDD LinReg and turn off the VDDD DCDC output.
            //------------------------------------------------------------------
            BF_SET(POWER_VDDDCTRL, ENABLE_LINREG);
            BF_SET(POWER_VDDDCTRL, DISABLE_FET);


            //------------------------------------------------------------------
            // Make sure stepping is disabled when using linear regulators.
            //------------------------------------------------------------------
            BF_SET(POWER_VDDDCTRL, DISABLE_STEPPING);


        break;


        //----------------------------------------------------------------------
        // Power the VDDD rail from the linear regulator.  The DCDC is ready
        // to automatically power the chip when 5V is removed.  Use this
        // configuration when powering from 5V.
        //----------------------------------------------------------------------
        case HW_POWER_LINREG_DCDC_READY:

            //------------------------------------------------------------------
            // Use LinReg offset for LinReg mode.
            //------------------------------------------------------------------
            hw_power_SetVdddLinRegOffset(HW_POWER_LINREG_OFFSET_DCDC_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDD LinReg and turn on the VDDD DCDC output.  The
            // ENABLE_DCDC must be cleared to avoid LinReg and DCDC conflict.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDDCTRL, ENABLE_LINREG);
            BF_CLR(POWER_VDDDCTRL, DISABLE_FET);


            //------------------------------------------------------------------
            // Make sure stepping is disabled when using linear regulators.
            //------------------------------------------------------------------
            BF_SET(POWER_VDDDCTRL, DISABLE_STEPPING);


        break;


        //----------------------------------------------------------------------
        // Invalid power source option.
        //----------------------------------------------------------------------
        default:
            rtn = ERROR_HW_POWER_INVALID_INPUT_PARAM;

        break;

    }

    return rtn;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
hw_power_PowerSource_t hw_power_GetVdddPowerSource(void)
{

    //--------------------------------------------------------------------------
    // If the Vddd DCDC Converter output is disabled, LinReg must be powering
    // Vddd.
    //--------------------------------------------------------------------------

    if(HW_POWER_VDDDCTRL.B.DISABLE_FET)
    {

        if(VdddPowerSource == HW_POWER_EXTERNAL_SOURCE_5V)
        {
            return HW_POWER_EXTERNAL_SOURCE_5V;
        }

        //----------------------------------------------------------------------
        // Make sure the LinReg offset is correct for this source.
        //----------------------------------------------------------------------

        if(hw_power_GetVdddLinRegOffset() == HW_POWER_LINREG_OFFSET_LINREG_MODE)
        {
            return HW_POWER_LINREG_DCDC_OFF;
        }

    }


    //--------------------------------------------------------------------------
    // If 5V is present, but the ENABLE_DCDC bit is not set, then LinRegs
    // must be powering the chip.
    //--------------------------------------------------------------------------
    if( hw_power_Get5vPresentFlag() )
    {
        if( HW_POWER_5VCTRL.B.ENABLE_DCDC )
        {
            return HW_POWER_DCDC_LINREG_ON;
        }

        else
        {
            return HW_POWER_LINREG_DCDC_OFF;
        }
    }


    //--------------------------------------------------------------------------
    // If here, DCDC must be powering Vddd.  Determine if the LinReg is also on.
    //--------------------------------------------------------------------------

    if(HW_POWER_VDDDCTRL.B.ENABLE_LINREG)
    {

        //----------------------------------------------------------------------
        // The LinReg offset must be below the target if DCDC and LinRegs' are
        // on at the same time.  Otherwise, we have an invalid configuration.
        //----------------------------------------------------------------------

        if(hw_power_GetVdddLinRegOffset() == HW_POWER_LINREG_OFFSET_DCDC_MODE)
        {
            return HW_POWER_DCDC_LINREG_ON;
        }

    }

    else
    {
        //----------------------------------------------------------------------
        // Is the LinReg offset set to power Vddd from linreg?
        //----------------------------------------------------------------------

        if(hw_power_GetVdddLinRegOffset() == HW_POWER_LINREG_OFFSET_DCDC_MODE)
        {
            return HW_POWER_DCDC_LINREG_OFF;
        }

    }


    //--------------------------------------------------------------------------
    // If we get here, the power source is in an unknown configuration.  Most
    // likely, the LinReg offset is incorrect for the given power supply.
    //--------------------------------------------------------------------------
    return HW_POWER_UNKNOWN_SOURCE;
}

////////////////////////////////////////////////////////////////////////////////
// VDDIO
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddioValue(uint16_t u16Vddio_mV)
{
    uint16_t u16Vddio_Set;

    // Convert mV to register setting
    u16Vddio_Set = hw_power_ConvertVddioToSetting(u16Vddio_mV);

    // Use this way of writing to the register because we want a read, modify,
    // write operation.  Using the other available macros will call a clear/set
    // which momentarily clears the target field causing the voltage to dip
    // then rise again.  That should be avoided.
    HW_POWER_VDDIOCTRL.B.TRG = u16Vddio_Set;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVddioValue(void)
{
    uint16_t    u16Vddio_Set;
    uint16_t    u16Vddio_mV;

    //Read VDDIO bitfiled value
    u16Vddio_Set = HW_POWER_VDDIOCTRL.B.TRG;

    //  Convert to mVolts
    u16Vddio_mV = hw_power_ConvertSettingToVddio(u16Vddio_Set);

    return u16Vddio_mV;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddioBrownoutValue(uint16_t u16VddioBoOffset_mV)
{
    uint16_t u16VddioBoOffset_Set;

    // Convert millivolts to register setting (1 step = 25mV)
    u16VddioBoOffset_Set = u16VddioBoOffset_mV/(VOLTAGE_STEP_MV);

    // Use this way of writing to the register because we want a read, modify,
    // write operation.  Using the other available macros will call a clear/set
    // which momentarily makes the brownout offset zero which may trigger a
    // false brownout.
    HW_POWER_VDDIOCTRL.B.BO_OFFSET = u16VddioBoOffset_Set;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVddioBrownoutValue(void)
{
    uint16_t u16VddioBoOffset_Set;
    uint16_t u16VddioBoOffset_mV;

    // Read the VDDIO brownout offset field.
    u16VddioBoOffset_Set = HW_POWER_VDDIOCTRL.B.BO_OFFSET;

    // Convert setting to millivolts. (1 step = 25mV)
    u16VddioBoOffset_mV = (u16VddioBoOffset_Set * VOLTAGE_STEP_MV);

    // Return the brownout offset in mV.
    return u16VddioBoOffset_mV;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_SetVddioPowerSource(hw_power_PowerSource_t eSource)
{
    RtStatus_t rtn = SUCCESS;

    //--------------------------------------------------------------------------
    // The VDDIO can use two power sources in three configurations: the Linear
    // Regulator, the DCDC with LinReg off, and the DCDC with LinReg on.
    // Each has its own configuration that must be set up to prevent power
    // rail instability. VDDIO will only use the linear regulator when
    // 5V is present.
    //--------------------------------------------------------------------------


    switch(eSource)
    {

        //----------------------------------------------------------------------
        // Power the VDDIO rail from DCDC with LinReg off.
        //----------------------------------------------------------------------
        case HW_POWER_DCDC_LINREG_OFF:

            //------------------------------------------------------------------
            // Use LinReg offset for DCDC mode.
            //------------------------------------------------------------------
            hw_power_SetVddioLinRegOffset(HW_POWER_LINREG_OFFSET_DCDC_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDIO DCDC output and turn off the LinReg output.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDIOCTRL, DISABLE_FET);
            hw_power_DisableVddioLinearRegulator(TRUE);


            //------------------------------------------------------------------
            // Make sure stepping is enabled when using DCDC.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDIOCTRL, DISABLE_STEPPING);


        break;


        //----------------------------------------------------------------------
        // Power the VDDIO rail from DCDC with LinReg on.
        //----------------------------------------------------------------------
        case HW_POWER_DCDC_LINREG_READY:
        case HW_POWER_DCDC_LINREG_ON:


            //------------------------------------------------------------------
            // Use LinReg offset for DCDC mode.
            //------------------------------------------------------------------
            hw_power_SetVddioLinRegOffset(HW_POWER_LINREG_OFFSET_DCDC_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDIO DCDC output and turn on the LinReg output.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDIOCTRL, DISABLE_FET);
            hw_power_DisableVddioLinearRegulator(FALSE);


            //------------------------------------------------------------------
            // Make sure stepping is enabled when using DCDC.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDIOCTRL, DISABLE_STEPPING);


        break;


        //----------------------------------------------------------------------
        // Power the VDDIO rail from the linear regulator.
        // Assumes 5V is present.
        //----------------------------------------------------------------------
        case HW_POWER_LINREG_DCDC_OFF:

            //------------------------------------------------------------------
            // Use LinReg offset for LinReg mode.
            //------------------------------------------------------------------
            hw_power_SetVddioLinRegOffset(HW_POWER_LINREG_OFFSET_LINREG_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDIO LinReg output and turn off the VDDIO DCDC
            // output.
            //------------------------------------------------------------------
            hw_power_DisableVddioLinearRegulator(FALSE);
            BF_SET(POWER_VDDIOCTRL, DISABLE_FET);


            //------------------------------------------------------------------
            // Make sure stepping is disabled when using linear regulators.
            //------------------------------------------------------------------
            BF_SET(POWER_VDDIOCTRL, DISABLE_STEPPING);


        break;


        //----------------------------------------------------------------------
        // Power the VDDIO rail from the linear regulator.
        // Assumes 5V is present.  The DCDC is ready to take over when 5V
        // is removed.
        //----------------------------------------------------------------------
        case HW_POWER_LINREG_DCDC_READY:


            //------------------------------------------------------------------
            // Use LinReg offset for LinReg mode.
            //------------------------------------------------------------------
            hw_power_SetVddioLinRegOffset(HW_POWER_LINREG_OFFSET_DCDC_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDIO LinReg output and prepare the
            // VDDIO DCDC output.
            // ENABLE_DCDC must be cleared to prevent DCDC and LinReg conflict.
            //------------------------------------------------------------------
            hw_power_DisableVddioLinearRegulator(FALSE);
            BF_CLR(POWER_VDDIOCTRL, DISABLE_FET);


            //------------------------------------------------------------------
            // Make sure stepping is disabled when using linear regulators.
            //------------------------------------------------------------------
            BF_SET(POWER_VDDIOCTRL, DISABLE_STEPPING);


        break;


        //----------------------------------------------------------------------
        // Invalid power source option.
        //----------------------------------------------------------------------
        default:
            rtn = ERROR_HW_POWER_INVALID_INPUT_PARAM;
        break;
    }

    return rtn;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
hw_power_PowerSource_t hw_power_GetVddioPowerSource(void)
{

    //--------------------------------------------------------------------------
    // 5 Volts must be present to use VDDIO LinReg
    //--------------------------------------------------------------------------

    if(hw_power_Get5vPresentFlag())
    {
        //----------------------------------------------------------------------
        // Check VDDIO's DCDC converter output.
        //----------------------------------------------------------------------

        if(HW_POWER_VDDIOCTRL.B.DISABLE_FET)
        {

            //------------------------------------------------------------------
            // DCDC output is off so we must be using LinReg.  Check that the
            // offset if correct.
            //------------------------------------------------------------------

            if(hw_power_GetVddioLinRegOffset() ==
		HW_POWER_LINREG_OFFSET_LINREG_MODE)
            {
                return HW_POWER_LINREG_DCDC_OFF;
            }

        }


        //----------------------------------------------------------------------
        // We have 5V and DCDC converter output enabled.  Now check if the
        // DCDC is enabled.
        //----------------------------------------------------------------------

        if(hw_power_GetEnableDcdc())
        {

            //------------------------------------------------------------------
            // DCDC is enabled.  We must be powered from DCDC with LinReg on.
            // Check the offset.
            //------------------------------------------------------------------

            if(hw_power_GetVdddLinRegOffset() ==
		HW_POWER_LINREG_OFFSET_DCDC_MODE)
            {
                return HW_POWER_DCDC_LINREG_ON;
            }

        }

        else
        {

            //------------------------------------------------------------------
            // DCDC is not enabled.  We must be powered from LinReg.
            //------------------------------------------------------------------

            if(hw_power_GetVddioLinRegOffset() ==
		HW_POWER_LINREG_OFFSET_LINREG_MODE)
            {
                return HW_POWER_LINREG_DCDC_OFF;
            }
        }
    }

    else
    {
        //----------------------------------------------------------------------
        // VDDIO must be powered from DCDC with LinReg off.  Check the offset.
        //----------------------------------------------------------------------

            if(hw_power_GetVdddLinRegOffset() ==
		HW_POWER_LINREG_OFFSET_DCDC_MODE)
            {
                return HW_POWER_DCDC_LINREG_ON;
            }
    }


    //--------------------------------------------------------------------------
    // If we get here, the power source is in an unknown configuration.  Most
    // likely, the LinReg offset is incorrect for the given power supply.
    //--------------------------------------------------------------------------
    return HW_POWER_UNKNOWN_SOURCE;

}

////////////////////////////////////////////////////////////////////////////////
// VDDA
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddaValue(uint16_t u16Vdda_mV)
{
    uint16_t u16Vdda_Set;

    // Convert mV to register setting
    u16Vdda_Set = hw_power_ConvertVddaToSetting(u16Vdda_mV);

    // Use this way of writing to the register because we want a read, modify,
    // write operation.  Using the other available macros will call a clear/set
    // which momentarily clears the target field causing the voltage to dip
    // then rise again.  That should be avoided.
    HW_POWER_VDDACTRL.B.TRG = u16Vdda_Set;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVddaValue(void)
{
    uint16_t    u16Vdda_Set;
    uint16_t    u16Vdda_mV;

    //Read VDDA bitfiled value
    u16Vdda_Set = HW_POWER_VDDACTRL.B.TRG;

    //  Convert to mVolts
    u16Vdda_mV = hw_power_ConvertSettingToVdda(u16Vdda_Set);

    return u16Vdda_mV;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddaBrownoutValue(uint16_t u16VddaBoOffset_mV)
{
    uint16_t u16VddaBoOffset_Set;

    // Convert millivolts to register setting (1 step = 25mV)
    u16VddaBoOffset_Set = u16VddaBoOffset_mV/(VOLTAGE_STEP_MV);

    // Use this way of writing to the register because we want a read, modify,
    // write operation.  Using the other available macros will call a clear/set
    // which momentarily makes the brownout offset zero which may trigger a
    // false brownout.
    HW_POWER_VDDACTRL.B.BO_OFFSET = u16VddaBoOffset_Set;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVddaBrownoutValue(void)
{
    uint16_t u16VddaBoOffset_Set;
    uint16_t u16VddaBoOffset_mV;

    // Read the VDDA brownout offset field.
    u16VddaBoOffset_Set = HW_POWER_VDDACTRL.B.BO_OFFSET;

    // Convert setting to millivolts. (1 step = 25mV)
    u16VddaBoOffset_mV = (u16VddaBoOffset_Set * VOLTAGE_STEP_MV);

    // Return the brownout offset in mV.
    return u16VddaBoOffset_mV;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_SetVddaPowerSource(hw_power_PowerSource_t eSource)
{
    RtStatus_t rtn=SUCCESS;

    //--------------------------------------------------------------------------
    // The VDDA can use two power sources: the DCDC, or the Linear Regulator.
    // Each has its own configuration that must be set up to prevent power
    // rail instability.
    //--------------------------------------------------------------------------
    switch(eSource)
    {

        //----------------------------------------------------------------------
        // Power the VDDA supply from DCDC with VDDA LinReg off.
        //----------------------------------------------------------------------
        case HW_POWER_DCDC_LINREG_OFF:
        case HW_POWER_DCDC_LINREG_READY:


            //------------------------------------------------------------------
            // Use LinReg offset for DCDC mode.
            //------------------------------------------------------------------
            hw_power_SetVddaLinRegOffset(HW_POWER_LINREG_OFFSET_DCDC_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDA DCDC converter output and turn off the LinReg
	    // output.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDACTRL, DISABLE_FET);
            BF_CLR(POWER_VDDACTRL, ENABLE_LINREG);


            //------------------------------------------------------------------
            // Make sure stepping is enabled when using DCDC.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDACTRL, DISABLE_STEPPING);

        break;


        //----------------------------------------------------------------------
        // Power the VDDA supply from DCDC with VDDA LinReg off.
        //----------------------------------------------------------------------
        case HW_POWER_DCDC_LINREG_ON:


            //------------------------------------------------------------------
            // Use LinReg offset for DCDC mode.
            //------------------------------------------------------------------
            hw_power_SetVddaLinRegOffset(HW_POWER_LINREG_OFFSET_DCDC_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDA DCDC converter output and turn on the LinReg
            // output.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDACTRL, DISABLE_FET);
            BF_SET(POWER_VDDACTRL, ENABLE_LINREG);


            //------------------------------------------------------------------
            // Make sure stepping is enabled when using DCDC.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDACTRL, DISABLE_STEPPING);

        break;


        //----------------------------------------------------------------------
        // Power the VDDA supply from the linear regulator.  The DCDC output is
        // off and not ready to power the rail if 5V is removed.
        //----------------------------------------------------------------------
        case HW_POWER_LINREG_DCDC_OFF:


            //------------------------------------------------------------------
            // Use LinReg offset for LinReg mode.
            //------------------------------------------------------------------
            hw_power_SetVddaLinRegOffset(HW_POWER_LINREG_OFFSET_LINREG_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDA LinReg output and turn off the VDDA DCDC output.
            //------------------------------------------------------------------
            BF_SET(POWER_VDDACTRL, ENABLE_LINREG);
            BF_SET(POWER_VDDACTRL, DISABLE_FET);


            //------------------------------------------------------------------
            // Make sure stepping is disabled when using linear regulators.
            //------------------------------------------------------------------
            BF_SET(POWER_VDDACTRL, DISABLE_STEPPING);

        break;


        //----------------------------------------------------------------------
        // Power the VDDA supply from the linear regulator.  The DCDC output is
        // ready to power the rail if 5V is removed.
        //----------------------------------------------------------------------
        case HW_POWER_LINREG_DCDC_READY:

            //------------------------------------------------------------------
            // Use LinReg offset for LinReg mode.
            //------------------------------------------------------------------
            hw_power_SetVddaLinRegOffset(HW_POWER_LINREG_OFFSET_DCDC_MODE);


            //------------------------------------------------------------------
            // Turn on the VDDA LinReg output and prepare the DCDC for transfer.
            // ENABLE_DCDC must be clear to avoid DCDC and LinReg conflict.
            //------------------------------------------------------------------
            BF_CLR(POWER_VDDACTRL, ENABLE_LINREG);
            BF_CLR(POWER_VDDACTRL, DISABLE_FET);


            //------------------------------------------------------------------
            // Make sure stepping is disabled when using linear regulators.
            //------------------------------------------------------------------
            BF_SET(POWER_VDDACTRL, DISABLE_STEPPING);

        break;


        //----------------------------------------------------------------------
        // Invalid power source option.
        //----------------------------------------------------------------------
        default:
            rtn = ERROR_HW_POWER_INVALID_INPUT_PARAM;

        break;

    }

    return rtn;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
hw_power_PowerSource_t hw_power_GetVddaPowerSource(void)
{
    //--------------------------------------------------------------------------
    // If the DCDC converter output is not enabled, we must be using LinReg.
    //--------------------------------------------------------------------------
    if(HW_POWER_VDDACTRL.B.DISABLE_FET)
    {

        //----------------------------------------------------------------------
        // Make sure the LinReg offset is correct for this source.
        //----------------------------------------------------------------------
        if(hw_power_GetVddaLinRegOffset() == HW_POWER_LINREG_OFFSET_LINREG_MODE)
        {
            return HW_POWER_LINREG_DCDC_OFF;
        }
    }


    //--------------------------------------------------------------------------
    // If here, DCDC must be powering Vdda.  Determine if the LinReg is also on.
    //--------------------------------------------------------------------------
    if(HW_POWER_VDDACTRL.B.ENABLE_LINREG)
    {

        //----------------------------------------------------------------------
        // The LinReg offset must be below the target if DCDC and LinRegs' are
        // on at the same time.  Otherwise, we have an invalid configuration.
        //----------------------------------------------------------------------
        if(hw_power_GetVddaLinRegOffset() == HW_POWER_LINREG_OFFSET_DCDC_MODE)
        {
            return HW_POWER_DCDC_LINREG_ON;
        }

    }

    else
    {
        //----------------------------------------------------------------------
        // Is the LinReg offset set to power Vdda from linreg?
        //----------------------------------------------------------------------
        if(hw_power_GetVddaLinRegOffset() == HW_POWER_LINREG_OFFSET_DCDC_MODE)
        {
            return HW_POWER_DCDC_LINREG_OFF;
        }

    }

    //--------------------------------------------------------------------------
    // If we get here, the power source is in an unknown configuration.  Most
    // likely, the LinReg offset is incorrect for the given power supply.
    //--------------------------------------------------------------------------
    return HW_POWER_UNKNOWN_SOURCE;

}

////////////////////////////////////////////////////////////////////////////////
// Linear Regulators
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVddioLinearRegulator(bool bDisable)
{
    if(bDisable)
    {
        BF_SET(POWER_5VCTRL, ILIMIT_EQ_ZERO);
    }
    else
    {
        BF_CLR(POWER_5VCTRL, ILIMIT_EQ_ZERO);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVddaLinearRegulator(bool bDisable)
{
    if(bDisable)
    {
        BF_CLR(POWER_VDDACTRL, ENABLE_LINREG);
    }
    else
    {
        BF_SET(POWER_VDDACTRL, ENABLE_LINREG);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVdddLinearRegulator(bool bDisable)
{
    if(bDisable)
    {
        BF_CLR(POWER_VDDDCTRL, ENABLE_LINREG);
    }
    else
    {
        BF_SET(POWER_VDDDCTRL, ENABLE_LINREG);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_Disable5vLinearRegulators(bool bDisable)
{
    if(bDisable)
    {
        // Turn off Vddio LinReg
        hw_power_DisableVddioLinearRegulator(TRUE);
        // Turn off Vdda LinReg
        hw_power_DisableVddaLinearRegulator(TRUE);
        // Turn off Vddd LinReg
        hw_power_DisableVdddLinearRegulator(TRUE);
    }
    else
    {
        // Turn on Vddio Linreg
        hw_power_DisableVddioLinearRegulator(FALSE);
        // Turn on Vdda Linreg
        hw_power_DisableVddaLinearRegulator(FALSE);
        // Turn on Vddd Linreg
        hw_power_DisableVdddLinearRegulator(FALSE);
    }
}

////////////////////////////////////////////////////////////////////////////////
// DCDC/5V Connection
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVdddDcdcFet(bool bDisable)
{
    if(bDisable)
    {
        BF_SET(POWER_VDDDCTRL, DISABLE_FET);
    }
    else
    {
        BF_CLR(POWER_VDDDCTRL, DISABLE_FET);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVddaDcdcFet(bool bDisable)
{
    if(bDisable)
    {
        BF_SET(POWER_VDDACTRL, DISABLE_FET);
    }
    else
    {
        BF_CLR(POWER_VDDACTRL, DISABLE_FET);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVddioDcdcFet(bool bDisable)
{
    if(bDisable)
    {
        BF_SET(POWER_VDDIOCTRL, DISABLE_FET);
    }
    else
    {
        BF_CLR(POWER_VDDIOCTRL, DISABLE_FET);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_CheckDcdcTransitionDone(void)
{
    // Only need to check DCDC if a supply is sourced from it.  The LinRegs
    // will also cause the DC_OK flag to go high when transitioning from high
    // to low voltages.
    if((hw_power_GetVddioPowerSource()== HW_POWER_DCDC_LINREG_OFF) ||
       (hw_power_GetVddaPowerSource() == HW_POWER_DCDC_LINREG_OFF) ||
       (hw_power_GetVdddPowerSource() == HW_POWER_DCDC_LINREG_OFF) ||
       (hw_power_GetVddioPowerSource()== HW_POWER_DCDC_LINREG_ON) ||
       (hw_power_GetVddaPowerSource() == HW_POWER_DCDC_LINREG_ON) ||
       (hw_power_GetVdddPowerSource() == HW_POWER_DCDC_LINREG_ON) )
    {
        // Someone is using DCDC's so we need to check the transition
        if (HW_POWER_STS.B.DC_OK)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}


#ifdef PRINT_LDO_STATUS
void hw_power_PrintExternalLdoStatusCallback(uint32_t Status)
{
    if( Status )
    {
        tss_logtext_Print(LOGTEXT_VERBOSITY_1,"HW_PWR: External LDO - ON\r\n");
    }
    else
    {
        tss_logtext_Print(LOGTEXT_VERBOSITY_1,"HW_PWR: External LDO - OFF\r\n");
    }
}

void hw_power_PrintExternalLdoStatus(uint32_t Status)
{
    os_dpc_Send(OS_DPC_HIGH_LEVEL_DPC,
                &hw_power_PrintExternalLdoStatusCallback,
                Status,
                TX_NO_WAIT);
}
#endif


////////////////////////////////////////////////////////////////////////////////
//! \brief Turns on power to the 4p2 rail's components.
//!
//! \fntype Function
//!
//! Initializes the hardware for the 4p2 rail.
//!
//! \retval SUCCESS Hardware initialized complete.
//! \retval false something failed...
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_PowerOn4p2Rail( void )
{
    // If 4p2 has already been enabled, don't try to re-enable without
    // calling Stop4p2 first.
    if(HW_POWER_5VCTRL.B.PWD_CHARGE_4P2==0)
        return ERROR_HW_POWER_4P2_ALREADY_ON;

    // Working VBUSVALID functionality required.  When VBUSVALID detection
    // is enabled, it automatically keeps these comparators.  But manually
    // keeping them on here, just in case.
    BF_SET(POWER_5VCTRL, PWRUP_VBUS_CMPS);

    // Configure DCDCs output FETS and linear regulators for proper
    // DCDC operation

    // Enables but DCDC 4P2 logic.  This appears to be a necessary step not
    // only for DCDC operation from 4p2, but for the 4p2 rail itself to
    // start up properly.
    BF_SET(POWER_DCDC4P2, ENABLE_4P2);  //enable 5V to 4p2 regulator

    // This must be used if there is no static load on 4p2 as 4p2 will
    // become unstable with no load.
    BF_SET(POWER_CHARGE,ENABLE_LOAD);

    // Initialize 4p2 current limt before powering up 4p2
    HW_POWER_5VCTRL.B.CHARGE_4P2_ILIMIT = INITIAL_CHARGE_4P2_CURRENT_LIMIT;

    //enable DCDC 4P2 capability.  The DCDC_4P2_BO functionality does not
    // become enabled until this is done.
    hw_power_Enable4p2DcdcInput( true );

    // Set the initial BO level to 3.6 V so the 4p2 regulator won't brownout
    // and steal from
    // battery while ramping up to full current.
    HW_POWER_DCDC4P2.B.BO = 0; // 0 * 25mW +3.6V = 3.60V
    HW_POWER_DCDC4P2.B.TRG = 0; // 4.2V

    // power up the 4p2 rail and logic/control
    hw_power_EnableMaster4p2( true );

    // Start charging up the 4p2 capacitor at 30mA
    HW_POWER_5VCTRL.B.CHARGE_4P2_ILIMIT = START_CAP_CHARGE_4P2_CURRENT_LIMIT;

    return SUCCESS;

}

////////////////////////////////////////////////////////////////////////////////
//! \brief Turns off power to the 4p2 rail's components.
//!
//! \fntype Function
//!
//! Powers off the 4p2 rail's hardware components.
//!
//! \retval SUCCESS Power off completed successfully.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_PowerOff4p2Rail( void )
{
    BF_SET(POWER_5VCTRL, PWRUP_VBUS_CMPS);

    BF_CLR(POWER_CTRL,ENIRQ_DCDC4P2_BO);

    hw_power_EnableMaster4p2( false );

    return SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
//! \brief Enables the 4p2 rail.
//!
//! \fntype Function
//!
//! Turns on the DCDC's when 5V is present.  Until the DCDC's are on, the 4p2
//! rail will not have a load.
//!
//! \retval SUCCESS 4p2 railed enabled.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_Enable4p2Rail( void )
{
    HW_POWER_DCDC4P2.B.BO = 12; // 4.1V BO level

    // Need to turn off this auto transfer because the DCDC will already be on.
    // Enabling this causes weirdness when 4p2 hands off to battery.
    hw_power_EnableAutoDcdcTransfer( false );

    // Enables DCDC during 5V connnection (for both battery or 4p2 powered DCDC)
    // Use the API because it handles the false 5V brownout cases.
    hw_power_EnableDcdc( true );

    /* now that DCDC load is present, we can remove the 4p2 interal load */
    BF_CLR(POWER_CHARGE,ENABLE_LOAD);

    // Make sure internal linear regulators can't supply more than 100mA
    // Disabling this for now until further investigation of pros and cons
    // of enabling it.
    //BF_SET(POWER_5VCTRL, ENABLE_LINREG_ILIMIT);

    return SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
//! \brief Disables the 4p2 rail.
//!
//! \fntype Function
//!
//! Turns off the DCDC converter when 5V is present.
//!
//! \retval SUCCESS
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_Disable4p2Rail( void )
{
    uint32_t u32PrevTarget = HW_POWER_DCDC4P2.B.TRG;
    uint32_t u32PrevCmpTrip = HW_POWER_DCDC4P2.B.CMPTRIP;
    uint8_t  i;

    // Disable the 4p2 brownout.
    hw_power_Enable4p2BrownoutInterrupt( false );

    // Bring the 4P2 rail to battery voltage to allow DCDC feedback loop
    // to adjust to new input voltage.
    for( i=0; i<5; i++)
    {
        HW_POWER_DCDC4P2.B.TRG = i;
        //BF_WR(POWER_DCDC4P2, TRG, i);
        hw_digctl_MicrosecondWait(100);
    }

    // Force DCDC to use battery input by setting comparison to highest
    // bias towards battery.
    BF_WR(POWER_DCDC4P2, CMPTRIP, 31);
    hw_digctl_MicrosecondWait(25);

    // Disable the 4P2 circuitry.
    BF_CLR(POWER_DCDC4P2, ENABLE_4P2);

    // Disable the 4P2 input to the DCDC and remove the load.
    hw_power_Enable4p2DcdcInput( false );
    BF_CLR(POWER_CHARGE,ENABLE_LOAD);

    // Restore the previous target and trip point.
    BF_WR(POWER_DCDC4P2, TRG, u32PrevTarget);
    BF_WR(POWER_DCDC4P2, CMPTRIP, u32PrevCmpTrip);

    return SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_SetMax5vCurrentLimiterCurrent(uint16_t u16Current)
{
    uint16_t   u16OldSetting;
    uint16_t   u16NewSetting;
    uint16_t   u16ToggleMask;

    if(u16Current > 780)
        u16Current = 780;

    // Get the old setting.
    u16OldSetting = BF_RD(POWER_5VCTRL, CHARGE_4P2_ILIMIT);

    // Convert the new threshold into a setting.
    u16NewSetting = hw_power_ConvertCurrentToSetting(u16Current);

    // Compute the toggle mask.
    u16ToggleMask = u16OldSetting ^ u16NewSetting;

    // Write to the toggle register.
    HW_POWER_5VCTRL_TOG(u16ToggleMask << BP_POWER_5VCTRL_CHARGE_4P2_ILIMIT);

    // Tell the caller what current we're set at now.
    return(hw_power_ConvertSettingToCurrent(u16NewSetting));
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable/Disable VDDMEM regulation
//!
//! Enables or disables the internal VDDMEM regulator
//!
//! \param[in] bEnable true to enable the power source, false to disable it.
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_Enable2p5(bool bEnable)
{
    if(bEnable)
    {
        // clear pulldown as it isn't needed for internal linear regulator
        BF_CLR(POWER_VDDMEMCTRL, PULLDOWN_ACTIVE);
        HW_POWER_VDDMEMCTRL.B.TRG = 0x10; // ~2.5v

        BF_SET(POWER_VDDMEMCTRL, ENABLE_ILIMIT);
        BF_SET(POWER_VDDMEMCTRL, ENABLE_LINREG);
        // wait 10ms for 2p5 capacitor to fully charge
        hw_digctl_MicrosecondWait(10000);
        BF_CLR(POWER_VDDMEMCTRL, ENABLE_ILIMIT);

    }
    else
    {
        BF_CLR(POWER_VDDMEMCTRL, ENABLE_LINREG);

        // we could also enable the pull down here
        // but I'll leave that for a separate call if it
        // becomes necessary on some applications.
    }
}
////////////////////////////////////////////////////////////////////////////////
//! \brief Returns value of CHARGE_4P2_ILIMIT in the HW_POWER_5VCTRL register.
//!
//! \fntype Function
//!
//! \retval Current limit for the 4p2 rail in milliamps.
////////////////////////////////////////////////////////////////////////////////
uint32_t hw_power_GetCharge4p2CurrentLimit( void )
{
    return hw_power_ConvertSettingToCurrent(
	HW_POWER_5VCTRL.B.CHARGE_4P2_ILIMIT );
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the value of CHARGE_4P2_ILIMIT in the HW_POWER_5VCTRL register.
//!
//! \fntype Function
//!
//! \param[in] u32mA Current limit in milliamps.
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetCharge4p2CurrentLimit( uint32_t u32mA )
{
    uint32_t RegSetting;

    RegSetting = hw_power_ConvertCurrentToSetting( u32mA );

    HW_POWER_5VCTRL.B.CHARGE_4P2_ILIMIT = RegSetting;
}


////////////////////////////////////////////////////////////////////////////////
//! \brief Returns the value of the DCDC_4P2 brownout bit.
//!
//! \fntype Function
//!
//! \retval True 4p2 brownout is true
//! \retval False 4p2 brownout is false, or has not occurred.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetDcdc4p2Brownout( void )
{
    return HW_POWER_STS.B.DCDC_4P2_BO;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the ENABLE_LOAD status for the 4.2V rail
//!
//! \fntype Function
//!
//! \retval Previous setting.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableLoadOn4p2( bool bEnable )
{
    bool bPrev = HW_POWER_CHARGE.B.ENABLE_LOAD;

    if( bEnable )
        BF_SET(POWER_CHARGE, ENABLE_LOAD);
    else
        BF_CLR(POWER_CHARGE, ENABLE_LOAD);

    return bPrev;

}

////////////////////////////////////////////////////////////////////////////////
//! \brief Configures the master enable switch for battery charger.
//!
//! \fntype Function
//!
//! The master enable switch must be managed between the charger and 4P2 rail.
//! The setting for the charger is saved in this function and passed on to the
//! master setting function.
//!
//! \retval Previous master switch setting.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableMasterCharge( bool bEnable )
{
    bool bPrev;
    bPrev = HW_POWER_5VCTRL.B.PWD_CHARGE_4P2;

    bEnableMasterCharge = bEnable;
    hw_power_ConfigureMasterChargeAnd4p2();

    return bPrev;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Configures the master enable switch for the 4P2 rail.
//!
//! \fntype Function
//!
//! The master enable switch must be managed between the charger and 4P2 rail.
//! The setting for the 4P2 rail is saved in this function and passed on to the
//! master setting function.
//!
//! \retval Previous master switch setting.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableMaster4p2( bool bEnable )
{
    bool bPrev;
    bPrev = HW_POWER_5VCTRL.B.PWD_CHARGE_4P2;

    bEnableMaster4p2 = bEnable;
    hw_power_ConfigureMasterChargeAnd4p2();

    return bPrev;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables/disables the master enable switch for charger and 4P2 rail.
//!
//! \fntype Function
//!
//! The master enable switch must be managed between the charger and 4P2 rail.
//! The setting for the charger and 4P2 rail is passed to this function, and
//! this function determines how the master charge/4P2 switch should be set.
//!
//! The function uses OR-gate logic.  If either switch enabled, the master
//! switch will be enabled.  If both switches are disabled, the master switch
//! will be disabled.
//!
//! \retval Previous master enable setting.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_ConfigureMasterChargeAnd4p2( void )
{
    // If either is true, we must enable the master switch because at least
    // one needs to turn on.  We only turn off the master if both are disabled.
    if( bEnableMasterCharge || bEnableMaster4p2 )
        BF_CLR(POWER_5VCTRL, PWD_CHARGE_4P2);
    else
        BF_SET(POWER_5VCTRL, PWD_CHARGE_4P2);

    return HW_POWER_5VCTRL.B.PWD_CHARGE_4P2;

}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_Enable4p2DcdcInput( bool bEnable )
{
    //--------------------------------------------------------------------------
    // STMP378x experiences a false 5V brownout when this bitfield is set,
    // so we need to temporarily disable the 5V brownout.
    //--------------------------------------------------------------------------
	// Save the current 5V brownout setting so we can restore it later.
	bool orig_vbusvalid_5vdetect=false;
	bool orig_pwd_bo=false;
	uint8_t orig_vbusvalid_threshold;
	bool bPrev5vBoPwrdn;
	bool bPrev5vDroop;

	bPrev5vBoPwrdn = hw_power_Enable5vBrownoutPowerdown( false );
	bPrev5vDroop = hw_power_EnableVdd5vDroopInterrupt( false );

	if(bEnable)
	{
		/* recording orignal values that will be modified
		* temporarlily to handle a chip bug.  See chip imx23 errata
		*  5837
		*/
		orig_vbusvalid_threshold =
				(HW_POWER_5VCTRL_RD() & BM_POWER_5VCTRL_VBUSVALID_TRSH)
				>> BP_POWER_5VCTRL_VBUSVALID_TRSH;

		if(HW_POWER_5VCTRL_RD() & BM_POWER_5VCTRL_VBUSVALID_5VDETECT)
			orig_vbusvalid_5vdetect = true;

		if(HW_POWER_MINPWR_RD() & BM_POWER_MINPWR_PWD_BO)
			orig_pwd_bo=true;

		/* disable mechanisms that get erroneously tripped by
		* when setting the DCDC4P2 EN_DCDC
		*/
		HW_POWER_5VCTRL_CLR(BM_POWER_5VCTRL_VBUSVALID_5VDETECT);
		HW_POWER_5VCTRL_CLR(BF_POWER_5VCTRL_VBUSVALID_TRSH(0x7));
		HW_POWER_5VCTRL_SET(BM_POWER_MINPWR_PWD_BO);

		HW_POWER_CTRL_CLR(BM_POWER_CTRL_VBUSVALID_IRQ);

		BF_SET(POWER_DCDC4P2, ENABLE_DCDC);

		// Allow time for glitch/noise to settle (errata 5837)
		hw_digctl_MicrosecondWait( 21 );

		HW_POWER_5VCTRL_SET(BF_POWER_5VCTRL_VBUSVALID_TRSH(
				orig_vbusvalid_threshold));

		/* give a little time after writing new vbusvalid threshold for glitch/noise
		 * to exceed vbusvalid_threshold.  Glitch/noise should be gone
		 * but this is an attempt to handle it in case it is not.
		 */

		hw_digctl_MicrosecondWait( 10 );

		if(HW_POWER_CTRL_RD() & BM_POWER_CTRL_VBUSVALID_IRQ)
		{

			/* if we arrived here, the glitch from errata 5837 has
			* caused VBUSVALID_IRQ to assert which causes
			* PWD_CHARGE_4P2 to be activated internally (shutting down
			* 4P2) even though the bit value in the register remains 0.
			* We toggle PWD_CHARGE_4P2 here after a delay to restart
			* 4P2 regulator power.
			*/
			hw_digctl_MicrosecondWait(1000);
			HW_POWER_CTRL_CLR(BM_POWER_CTRL_VBUSVALID_IRQ);

			HW_POWER_5VCTRL_SET(BM_POWER_5VCTRL_PWD_CHARGE_4P2);
			HW_POWER_5VCTRL_CLR(BM_POWER_5VCTRL_PWD_CHARGE_4P2);

		}

		if(orig_vbusvalid_5vdetect){
			HW_POWER_5VCTRL_SET(BM_POWER_5VCTRL_VBUSVALID_5VDETECT);
		}

		if(!orig_pwd_bo){
			HW_POWER_5VCTRL_CLR(BM_POWER_MINPWR_PWD_BO);
		}



		// Restore the 5V brownout setting.
		hw_power_Enable5vBrownoutPowerdown( bPrev5vBoPwrdn );
		hw_power_ClearVdd5vDroopInterrupt();
		hw_power_EnableVdd5vDroopInterrupt( bPrev5vDroop );
	}
	else
		BF_CLR(POWER_DCDC4P2, ENABLE_DCDC);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_Configure4p2DropoutControl(
	hw_power_4p2DropoutMargin_t eDropoutMargin,
	hw_power_4p2SourceSelect_t  eSourceSelect)
{
    uint8_t Margin = 0;
    uint8_t Source = 0;
    uint8_t RegSetting = 0;

    //--------------------------------------------------------------------------
    // Note: The translation is performed here instead of just using the
    // enumerated values in an attempt to keep the STMP37** compiler definitions
    // out of the header files.
    //--------------------------------------------------------------------------

    // Do the dropout margin translation to register setting.  Note this sets
    // the upper two bits of a four-bit value.
    switch( eDropoutMargin )
    {
        case DROPOUT_MARGIN_25:
            Margin = 0;  // 0b00XX (datasheet value)
            break;

        case DROPOUT_MARGIN_50:
            Margin = 4;  // 0b01XX
            break;

         case DROPOUT_MARGIN_100:
            Margin = 8;  // 0b10XX
            break;

        case DROPOUT_MARGIN_200:
        default:
            Margin = 12; // 0b11XX
            break;
    }

    // Do the source selection translation to register setting.  Note this sets
    // the lower two bits of a four-bit value.
    switch( eSourceSelect )
    {
        case DCDC4P2_ALWAYS:
        default:
            Source = 0;  // 0bXX00 (datasheet value)
            break;

        case DCDC4P2_GT_DCDCBATT:
            Source = 1;  // 0bXX01
            break;

        case DCDC4P2_OR_DCDCBATT:
            Source = 2;  // 0bXX1X
            break;
    }

    // Now create the register setting by logical ORing the upper two-bits and
    // lower two-bits of the four-bit register setting.  Write the calcualted
    // value to the register field.
    RegSetting = Margin | Source;
    BF_WR( POWER_DCDC4P2, DROPOUT_CTRL, RegSetting );

}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_Set4p2CmpTripPoint( hw_power_4p2CmpTripPoints_t CmpTrip )
{
    uint8_t RegSetting = 0;

    switch( CmpTrip )
    {
        case CMPTRIP_85PCT:
            RegSetting = 0;   // 0b00000 (datasheet value)
            break;

        case CMPTRIP_86PCT:
            RegSetting = 1;   // 0b00001
            break;

        case CMPTRIP_100PCT:
        default:
            RegSetting = 24;  // 0b11000
            break;

        case CMPTRIP_105PCT:
            RegSetting = 31;  // 0b11111
            break;
    }

    // Now write to the register field.
    BF_WR( POWER_DCDC4P2, CMPTRIP, RegSetting );

}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetHeadroomAdj( uint16_t Adj )
{
    // The register field is only 3 bits wide.  Clear out any extra values.
    Adj &= 7;

    // Now write to the register field.
    BF_WR( POWER_5VCTRL, HEADROOM_ADJ, Adj );
}


////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}








