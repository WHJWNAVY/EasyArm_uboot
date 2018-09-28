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
#include "hw/digctl/hw_digctl.h"

#include "drivers/power/ddi_power.h"       // Driver API
#include "drivers/power/ddi_power_errordefs.h"
#include "drivers/power/src/ddi_power_internal.h"
#include "registers/regsaudioout.h"


////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
//static bool bEnableVdddSafetyLimits = true;
static bool bEnableVddaSafetyLimits = true;
static bool bEnableVddioSafetyLimits = true;
static bool b5vPowerSourceValid = false;
static bool bForceDcdcPowerSource = false;
static bool bUseSafeVoltageLimits = true;

static bool b4p2Enabled = false;

RtStatus_t (*ddi_power_WaitCallback)(uint32_t) = NULL;
RtStatus_t (*ddi_power_LockRailCallback)(uint32_t) = NULL;
RtStatus_t (*ddi_power_UnlockRailCallback)(uint32_t) = NULL;


////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// VDDD
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set the VDDD and its brownout voltages.
//!
//! \fntype     Non-reentrant Function
//!
//! This function sets the VDDD value and VDDD brownout level specified by the
//! input parameters. If the new brownout level is equal to the current setting
//! it'll only update the VDDD setting. If the new brownout level is less than
//! the current setting, it will update the VDDD brownout first and then the VDDD.
//! Otherwise, it will update the VDDD first and then the brownout. This
//! arrangement is intended to prevent from false VDDD brownout. This function
//! will not return until the output VDDD stable.
//!
//! \param[in]  u16Vddd_mV              Vddd voltage in millivolts
//! \param[in]  u16VdddBrownout_mV      Vddd brownout in millivolts
//!
//! \return SUCCESS.
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t  ddi_power_SetVddd(uint16_t  u16NewTarget, uint16_t  u16NewBrownout)
{
    uint16_t    u16CurrentTarget, u16StepTarget, u16TargetDifference;
    RtStatus_t  rtn = SUCCESS;
    bool        bPoweredByLinReg;
    //hw_audioout_refctrl_t tempRefCtrlReg;

    //--------------------------------------------------------------------------
    // Make sure this is the only thread that can change this rail.
    //--------------------------------------------------------------------------
    ddi_power_LockRail( 100 );

    //--------------------------------------------------------------------------
    // Limit inputs and get the current target level
    //--------------------------------------------------------------------------

    if(ddi_power_GetSafeVoltageLimitsStatus())
    {
        // Apply ceiling and floor limits to Vddd and BO
        rtn = ddi_power_LimitVdddAndBo(&u16NewTarget, &u16NewBrownout);
    }

    // Convert the brownout as millivolt value to an offset from the target
    u16NewBrownout = u16NewTarget - u16NewBrownout;

    // Get the current target value
    u16CurrentTarget = hw_power_GetVdddValue();

    // Determine if this rail is powered by linear regulator.
    if(hw_power_GetVdddPowerSource() == HW_POWER_LINREG_DCDC_READY ||
       hw_power_GetVdddPowerSource() == HW_POWER_LINREG_DCDC_OFF )
    {
        bPoweredByLinReg = TRUE;
    }
    else
    {
        bPoweredByLinReg = FALSE;
    }


    //--------------------------------------------------------------------------
    // Voltage and brownouts need to be changed in specific order.
    //
    // Because the brownout is now an offset of the target, as the target
    // steps up or down, the brownout voltage will follow.  This causes a
    // problem where the brownout will be one step too close when the target
    // raised because the brownout changes instantly, but the output voltage
    // ramps up.
    //
    //  Target does this              ____
    //                               /
    //                          ____/
    //  but brownout does this       _____
    //                              |
    //                          ____|
    //
    //  We are concerned about the time under the ramp when the brownout
    //  is too close.
    //--------------------------------------------------------------------------
    if(u16NewTarget > u16CurrentTarget)
    {
        bool bPrevBoSetting;

        //----------------------------------------------------------------------
        // Temporarily change the rail's brownout.
        //----------------------------------------------------------------------
        if(bPoweredByLinReg)
        {
            // Disable detection if powered by linear regulator.  This avoids
            // the problem where the brownout level reaches its new value before
            // the target does.
            bPrevBoSetting = hw_power_EnableVdddBrownoutInterrupt(FALSE);
        }

        {
            // Temporarily use the maximum offset for DCDC transitions.  DCDC
            // transitions step the target and the brownout offset in 25mV
            // increments so there is not a risk of an inverted brownout level.
            hw_power_SetVdddBrownoutValue(BO_MAX_OFFSET_MV);
        }


        //----------------------------------------------------------------------
        // We want to limit voltage step sizes to account for FUNCV changes and
        // linreg voltage change which may result in current surges/voltage dips
        // on VDD5V.  To accomplish this, we will step the voltage in
        // pre-determined levels until the new target is reached.
        //----------------------------------------------------------------------
        do{

            // Calculate the next target to step to.  If we can't get to the
            // target without exceeding the maximum step voltage, then step
            // by the maximum and try again next loop.
            u16TargetDifference = u16NewTarget - u16CurrentTarget;
            if(u16TargetDifference > DDI_POWER_MAX_VOLTAGE_STEP_MV)
            {
                u16StepTarget = u16CurrentTarget + DDI_POWER_MAX_VOLTAGE_STEP_MV;
            }
            else
            {
                u16StepTarget = u16NewTarget;
            }

            // Now change target and wait for it to stabilize.
            hw_power_SetVdddValue(u16StepTarget);
            ddi_power_WaitForVdddStable();

            // Read the current value for the next comparison.
            u16CurrentTarget = hw_power_GetVdddValue();

        }
        while(u16NewTarget > u16CurrentTarget);


        //----------------------------------------------------------------------
        // Clean up any brownout issues and set the new brownout level.
        //----------------------------------------------------------------------
        if(bPoweredByLinReg)
        {
            // Clear the interrupt in case it occured.
            hw_power_ClearVdddBrownoutInterrupt();

            // Restore brownout setting since it was disabled for linreg cases.
            hw_power_EnableVdddBrownoutInterrupt(bPrevBoSetting);
        }

        // Set the real brownout offset.
        hw_power_SetVdddBrownoutValue(u16NewBrownout);


    }


    else
    {
        //----------------------------------------------------------------------
        // We want to limit voltage step sizes to account for FUNCV changes and
        // linreg voltage change which may result in current surges/voltage dips
        // on VDD5V.
        //----------------------------------------------------------------------
        do{

            // Calculate the next target to step to.  If we can't get to the
            // target without exceeding the maximum step voltage, then step
            // by the maximum and try again next loop.
            u16TargetDifference = u16CurrentTarget - u16NewTarget;
            if(u16TargetDifference > DDI_POWER_MAX_VOLTAGE_STEP_MV)
            {
                u16StepTarget = u16CurrentTarget - DDI_POWER_MAX_VOLTAGE_STEP_MV;
            }
            else
            {
                u16StepTarget = u16NewTarget;
            }

            // Now change target and wait for it to stabilize.
            hw_power_SetVdddValue(u16StepTarget);
            ddi_power_WaitForVdddStable();

            // Read the current value for the next comparison.
            u16CurrentTarget = hw_power_GetVdddValue();
        }
        while(u16NewTarget < u16CurrentTarget);


        //----------------------------------------------------------------------
        // Set the new brownout level.
        //----------------------------------------------------------------------
        hw_power_SetVdddBrownoutValue(u16NewBrownout);
    }

    //--------------------------------------------------------------------------
    // Voltage change complete.  Unlock the rail for other threads.
    //--------------------------------------------------------------------------
    ddi_power_UnlockRail(100);

    return rtn;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDD voltage
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present values of the VDDD voltage in millivolts
//!
//! \param[in]  none
//!
//! \return     VDDD voltage in millivolts
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVddd(void)
{

    ///////////////////////////////////////////////
    // Read the converted register value
    ///////////////////////////////////////////////

    return hw_power_GetVdddValue();
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDD brownout level voltage
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present value of VDDD brownout in millivolts
//!
//! \param[in]  none
//!
//! \return     VDDD brownout voltage in millivolts
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVdddBrownout(void)
{
    uint16_t VdddBoOffset_mV;
    uint16_t Vddd_mV;
    uint16_t VdddBo;

    //--------------------------------------------------------------------------
    // Read the target and brownout register values.
    //--------------------------------------------------------------------------

    Vddd_mV = hw_power_GetVdddValue();
    VdddBoOffset_mV = hw_power_GetVdddBrownoutValue();


    //--------------------------------------------------------------------------
    // The brownout level is the difference between the target and the offset.
    //--------------------------------------------------------------------------

    VdddBo = Vddd_mV - VdddBoOffset_mV;

    return VdddBo;
}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Waits on status bit signal or timeout for Vddd stability
//!
//! \fntype     Non-reentrant Function
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_WaitForVdddStable(void)
{
    //--------------------------------------------------------------------------
    // For DCDC cases, wait the prep time, then check if the transition has
    // completed.  If not wait a while and check again.  For linear regulator
    // cases, just wait a specified amount of time.
    //--------------------------------------------------------------------------

    // Obtain the current power source for this rail.
    hw_power_PowerSource_t Source = hw_power_GetVdddPowerSource();

    // Handle the appropriate power source.
    switch( Source )
    {
        case HW_POWER_LINREG_DCDC_OFF:
        case HW_POWER_LINREG_DCDC_READY:
        case HW_POWER_EXTERNAL_SOURCE_5V:

            ddi_power_WaitLinRegStable( MIN_STABLE_WAIT_TIME_LINREG,
                                        MAX_STABLE_WAIT_TIME_LINREG,
                                        RECHECK_WAIT_TIME_LINREG );
            break;

        case HW_POWER_DCDC_LINREG_OFF:
        case HW_POWER_DCDC_LINREG_ON:
        case HW_POWER_DCDC_LINREG_READY:
        default:

            ddi_power_WaitDcdcStable( MIN_STABLE_WAIT_TIME_DCDC,
                                      MAX_STABLE_WAIT_TIME_DCDC,
                                      RECHECK_WAIT_TIME_DCDC );
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See ddi_power.h for details.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_LimitVdddAndBo(uint16_t *pu16Vddd_mV, uint16_t *pu16Bo_mV)
{
    uint16_t    u16Vddd_mV = *pu16Vddd_mV;
    uint16_t    u16Bo_mV = *pu16Bo_mV;
    RtStatus_t  rtn = SUCCESS;

    //////////////////////////////////////////
    // Check Vddd limits
    //////////////////////////////////////////


	// Make sure Vddd is not above the safe voltage
	if (u16Vddd_mV > VDDD_SAFE_MAX_MV)
	{
		u16Vddd_mV = VDDD_SAFE_MAX_MV;
		rtn = ERROR_DDI_POWER_VDDD_PARAM_ADJUSTED;
	}

	// Make sure Vddd is not below the safe voltage
    if (u16Vddd_mV < VDDD_SAFE_MIN_MV)
    {
        u16Vddd_mV = VDDD_SAFE_MIN_MV;
        rtn = ERROR_DDI_POWER_VDDD_PARAM_ADJUSTED;
    }

    //////////////////////////////////////////
    // Check Vddd brownout limits
    //////////////////////////////////////////

    // Make sure there's at least a margin of difference between Vddd and Bo
    if (VDDD_TO_BO_MARGIN > (u16Vddd_mV - u16Bo_mV))
    {
        u16Bo_mV = u16Vddd_mV - VDDD_TO_BO_MARGIN;
        rtn = ERROR_DDI_POWER_VDDD_PARAM_ADJUSTED;
    }

    // Make sure the brownout value does not exceed the maximum allowed
    // by the system.
    if ((u16Vddd_mV - u16Bo_mV) > BO_MAX_OFFSET_MV)
    {
        u16Bo_mV = u16Vddd_mV - BO_MAX_OFFSET_MV;
        rtn = ERROR_DDI_POWER_VDDIO_PARAM_ADJUSTED;
    }

    // Give the results back to the caller.
    *pu16Vddd_mV = u16Vddd_mV;
    *pu16Bo_mV = u16Bo_mV;
    return rtn;
}

////////////////////////////////////////////////////////////////////////////////
// VDDIO
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set the VDDIO and its brownout voltages.
//!
//! \fntype     Non-reentrant Function
//!
//! This function sets the VDDIO value and VDDIO brownout level specified by the
//! input parameters. If the new brownout level is equal to the current setting
//! it'll only update the VDDIO setting. If the new brownout level is less than
//! the current setting, it will update the VDDIO brownout first and then the VDDIO.
//! Otherwise, it will update the VDDIO first and then the brownout. This
//! arrangement is intended to prevent from false VDDIO brownout. This function
//! will not return until the output VDDIO stable.
//!
//! \param[in]  u16Vddio_mV              Vddio voltage in millivolts
//! \param[in]  u16VddioBrownout_mV      Vddio brownout in millivolts
//!
//! \return SUCCESS.
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t  ddi_power_SetVddio(uint16_t u16NewTarget, uint16_t u16NewBrownout)
{
    uint16_t    u16CurrentTarget, u16StepTarget, u16TargetDifference;
    RtStatus_t  rtn = SUCCESS;
    bool        bPoweredByLinReg;

    //--------------------------------------------------------------------------
    // Make sure this is the only thread that can change this rail.
    //--------------------------------------------------------------------------
    ddi_power_LockRail(100);

    //--------------------------------------------------------------------------
    // Limit inputs and get the current BO level
    //--------------------------------------------------------------------------

    // Apply ceiling and floor limits to Vddio and BO
    rtn = ddi_power_LimitVddioAndBo(&u16NewTarget, &u16NewBrownout);

    // Convert the brownout as millivolt value to an offset from the target
    u16NewBrownout = u16NewTarget - u16NewBrownout;

    // Get the current target value
    u16CurrentTarget = hw_power_GetVddioValue();

    // Determine if this rail is powered by linear regulator.
    if(hw_power_GetVddioPowerSource() == HW_POWER_LINREG_DCDC_READY ||
       hw_power_GetVddioPowerSource() == HW_POWER_LINREG_DCDC_OFF )
    {
        bPoweredByLinReg = TRUE;
    }
    else
    {
        bPoweredByLinReg = FALSE;
    }

    //--------------------------------------------------------------------------
    // Voltage and brownouts need to be changed in specific order.
    //
    // Because the brownout is now an offset of the target, as the target
    // steps up or down, the brownout voltage will follow.  This causes a
    // problem where the brownout will be one step too close when the target
    // raised because the brownout changes instantly, but the output voltage
    // ramps up.
    //
    //  Target does this              ____
    //                               /
    //                          ____/
    //  but brownout does this       _____
    //                              |
    //                          ____|
    //
    //  We are concerned about the time under the ramp when the brownout
    //  is too close.
    //--------------------------------------------------------------------------
    if(u16NewTarget > u16CurrentTarget)
    {
        bool bPrevBoSetting;

        //----------------------------------------------------------------------
        // Temporarily change the rail's brownout.
        //----------------------------------------------------------------------
        if(bPoweredByLinReg)
        {
            // Disable detection if powered by linear regulator.  This avoids
            // the problem where the brownout level reaches its new value before
            // the target does.
            bPrevBoSetting = hw_power_EnableVddioBrownoutInterrupt(FALSE);
        }

        {
            // Temporarily use the maximum offset for DCDC transitions.  DCDC
            // transitions step the target and the brownout offset in 25mV
            // increments so there is not a risk of an inverted brownout level.
            hw_power_SetVddioBrownoutValue(BO_MAX_OFFSET_MV);
        }


        //----------------------------------------------------------------------
        // We want to limit voltage step sizes to account for FUNCV changes and
        // linreg voltage change which may result in current surges/voltage dips
        // on VDD5V.  To accomplish this, we will step the voltage in
        // pre-determined levels until the new target is reached.
        //----------------------------------------------------------------------
        do{

            // Calculate the next target to step to.  If we can't get to the
            // target without exceeding the maximum step voltage, then step
            // by the maximum and try again next loop.
            u16TargetDifference = u16NewTarget - u16CurrentTarget;
            if(u16TargetDifference > DDI_POWER_MAX_VOLTAGE_STEP_MV)
            {
                u16StepTarget = u16CurrentTarget + DDI_POWER_MAX_VOLTAGE_STEP_MV;
            }
            else
            {
                u16StepTarget = u16NewTarget;
            }

            // Now change target and wait for it to stabilize.
            hw_power_SetVddioValue(u16StepTarget);
            ddi_power_WaitForVddioStable();

            // Read the current value for the next comparison.
            u16CurrentTarget = hw_power_GetVddioValue();

        }
        while(u16NewTarget > u16CurrentTarget);


        //----------------------------------------------------------------------
        // Clean up any brownout issues and set the new brownout level.
        //----------------------------------------------------------------------
        if(bPoweredByLinReg)
        {
            // Clear the interrupt in case it occured.
            hw_power_ClearVddioBrownoutInterrupt();

            // Restore brownout setting since it was disabled for linreg cases.
            hw_power_EnableVddioBrownoutInterrupt(bPrevBoSetting);
        }

        // Set the real brownout offset.
        hw_power_SetVddioBrownoutValue(u16NewBrownout);


    }


    else
    {
        //----------------------------------------------------------------------
        // We want to limit voltage step sizes to account for FUNCV changes and
        // linreg voltage change which may result in current surges/voltage dips
        // on VDD5V.
        //----------------------------------------------------------------------
        do{

            // Calculate the next target to step to.  If we can't get to the
            // target without exceeding the maximum step voltage, then step
            // by the maximum and try again next loop.
            u16TargetDifference = u16CurrentTarget - u16NewTarget;
            if(u16TargetDifference > DDI_POWER_MAX_VOLTAGE_STEP_MV)
            {
                u16StepTarget = u16CurrentTarget - DDI_POWER_MAX_VOLTAGE_STEP_MV;
            }
            else
            {
                u16StepTarget = u16NewTarget;
            }

            // Now change target and wait for it to stabilize.
            hw_power_SetVddioValue(u16StepTarget);
            ddi_power_WaitForVddioStable();

            // Read the current value for the next comparison.
            u16CurrentTarget = hw_power_GetVddioValue();
        }
        while(u16NewTarget < u16CurrentTarget);


        //----------------------------------------------------------------------
        // Set the new brownout level.
        //----------------------------------------------------------------------
        hw_power_SetVddioBrownoutValue(u16NewBrownout);
    }

    //--------------------------------------------------------------------------
    // Voltage change complete.  Unlock the rail for other threads.
    //--------------------------------------------------------------------------
    ddi_power_UnlockRail(100);

    return rtn;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDIO voltage
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present values of the VDDIO voltage in millivolts
//!
//! \param[in]  none
//!
//! \return     VDDIO voltage in millivolts
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVddio(void)
{
    //--------------------------------------------------------------------------
    // Return the converted register value.
    //--------------------------------------------------------------------------

    return hw_power_GetVddioValue();
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDIO brownout level voltage
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present value of VDDIO brownout in millivolts
//!
//! \param[in]  none
//!
//! \return     VDDIO brownout voltage in millivolts
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVddioBrownout(void)
{
    uint16_t VddioBoOffset_mV;
    uint16_t Vddio_mV;
    uint16_t VddioBo;

    //--------------------------------------------------------------------------
    // Read the target and brownout register value
    //--------------------------------------------------------------------------

    Vddio_mV = hw_power_GetVddioValue();
    VddioBoOffset_mV = hw_power_GetVddioBrownoutValue();


    //--------------------------------------------------------------------------
    // The brownout level is the difference between the target and the offset.
    //--------------------------------------------------------------------------

    VddioBo = Vddio_mV - VddioBoOffset_mV;

    return VddioBo;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Waits on status bit signal or timeout for Vddio stability
//!
//! \fntype     Non-reentrant Function
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_WaitForVddioStable(void)
{
    //--------------------------------------------------------------------------
    // For DCDC cases, wait the prep time, then check if the transition has
    // completed.  If not wait a while and check again.  For linear regulator
    // cases, just wait a specified amount of time.
    //--------------------------------------------------------------------------

    // Obtain the current power source for this rail.
    hw_power_PowerSource_t Source = hw_power_GetVddioPowerSource();

    // Handle the appropriate power source.
    switch( Source )
    {
        case HW_POWER_LINREG_DCDC_OFF:
        case HW_POWER_LINREG_DCDC_READY:
        case HW_POWER_EXTERNAL_SOURCE_5V:

            ddi_power_WaitLinRegStable( MIN_STABLE_WAIT_TIME_LINREG,
                                        MAX_STABLE_WAIT_TIME_LINREG,
                                        RECHECK_WAIT_TIME_LINREG );
            break;

        case HW_POWER_DCDC_LINREG_OFF:
        case HW_POWER_DCDC_LINREG_ON:
        case HW_POWER_DCDC_LINREG_READY:
        default:

            ddi_power_WaitDcdcStable( MIN_STABLE_WAIT_TIME_DCDC,
                                      MAX_STABLE_WAIT_TIME_DCDC,
                                      RECHECK_WAIT_TIME_DCDC );
            break;
    }
}
////////////////////////////////////////////////////////////////////////////////
//! See ddi_power.h for details.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_LimitVddioAndBo(uint16_t *pu16Vddio_mV, uint16_t *pu16Bo_mV)
{
    uint16_t    u16Trg_mV = *pu16Vddio_mV;
    uint16_t    u16Bo_mV = *pu16Bo_mV;
    RtStatus_t  rtn = SUCCESS;

    //--------------------------------------------------------------------------
    // Check Vddio limits. Use different limits depending on whether we are
    // checking safety limits or register limits.
    //--------------------------------------------------------------------------
    if(bEnableVddioSafetyLimits)
    {
        //----------------------------------------------------------------------
        // Make sure Vddio is not above the safe voltage
        //----------------------------------------------------------------------
        if (u16Trg_mV > VDDIO_SAFE_MAX_MV)
        {
            u16Trg_mV = VDDIO_SAFE_MAX_MV;
            rtn = ERROR_DDI_POWER_VDDIO_PARAM_ADJUSTED;
        }

        //----------------------------------------------------------------------
        // Make sure Vddio is not below the safe voltage
        //----------------------------------------------------------------------
        if (u16Trg_mV < VDDIO_SAFE_MIN_MV)
        {
            u16Trg_mV = VDDIO_SAFE_MIN_MV;
            rtn = ERROR_DDI_POWER_VDDIO_PARAM_ADJUSTED;
        }
    }


    //--------------------------------------------------------------------------
    // Check Vddio brownout limits
    //--------------------------------------------------------------------------
    {
        //----------------------------------------------------------------------
        // Make sure there's at least a margin of difference between Vddio
        // and the brownout level.
        //----------------------------------------------------------------------
        if (BO_MIN_OFFSET_MV > (u16Trg_mV - u16Bo_mV))
        {
            u16Bo_mV = u16Trg_mV - BO_MIN_OFFSET_MV;
            rtn = ERROR_DDI_POWER_VDDIO_PARAM_ADJUSTED;
        }

        //----------------------------------------------------------------------
        // Make sure the brownout value does not exceed the maximum allowed
        // by the system.
        //----------------------------------------------------------------------
        if ((u16Trg_mV - u16Bo_mV) > BO_MAX_OFFSET_MV)
        {
            u16Bo_mV = u16Trg_mV - BO_MAX_OFFSET_MV;
            rtn = ERROR_DDI_POWER_VDDIO_PARAM_ADJUSTED;
        }

    }

    //--------------------------------------------------------------------------
    // Give the results back to the caller and return.
    //--------------------------------------------------------------------------
    *pu16Vddio_mV = u16Trg_mV;
    *pu16Bo_mV = u16Bo_mV;
    return rtn;
}

void ddi_power_EnableVddioSafetyLimits(bool bEnable)
{
    bEnableVddioSafetyLimits = bEnable;
}

////////////////////////////////////////////////////////////////////////////////
// VDDA
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set the VDDA and its brownout voltages.
//!
//! \fntype     Non-reentrant Function
//!
//! This function sets the VDDA value and VDDA brownout level specified by the
//! input parameters. If the new brownout level is equal to the current setting
//! it'll only update the VDDA setting. If the new brownout level is less than
//! the current setting, it will update the VDDA brownout first and then the VDDA.
//! Otherwise, it will update the VDDA first and then the brownout. This
//! arrangement is intended to prevent from false VDDA brownout. This function
//! will not return until the output VDDA stable.
//!
//! \param[in]  u16Vdda_mV              Vdda voltage in millivolts
//! \param[in]  u16VddaBrownout_mV      Vdda brownout in millivolts
//!
//! \return SUCCESS.
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t  ddi_power_SetVdda(uint16_t  u16NewTarget, uint16_t  u16NewBrownout)
{
    uint16_t    u16CurrentTarget, u16StepTarget, u16TargetDifference;
    RtStatus_t  rtn = SUCCESS;
    bool        bPoweredByLinReg;

    //--------------------------------------------------------------------------
    // Make sure this is the only thread that can change this rail.
    //--------------------------------------------------------------------------
    ddi_power_LockRail(100);

    //--------------------------------------------------------------------------
    // Limit inputs and get the current BO level
    //--------------------------------------------------------------------------

    // Apply ceiling and floor limits to Vdda and BO
    rtn = ddi_power_LimitVddaAndBo(&u16NewTarget, &u16NewBrownout);

    // Convert the brownout as millivolt value to an offset from the target
    u16NewBrownout = u16NewTarget - u16NewBrownout;

    // Get the current target value
    u16CurrentTarget = hw_power_GetVddaValue();

    // Determine if this rail is powered by linear regulator.
    if(hw_power_GetVddaPowerSource() == HW_POWER_LINREG_DCDC_READY ||
       hw_power_GetVddaPowerSource() == HW_POWER_LINREG_DCDC_OFF )
    {
        bPoweredByLinReg = TRUE;
    }
    else
    {
        bPoweredByLinReg = FALSE;
    }

    //--------------------------------------------------------------------------
    // Voltage and brownouts need to be changed in specific order.
    //
    // Because the brownout is now an offset of the target, as the target
    // steps up or down, the brownout voltage will follow.  This causes a
    // problem where the brownout will be one step too close when the target
    // raised because the brownout changes instantly, but the output voltage
    // ramps up.
    //
    //  Target does this              ____
    //                               /
    //                          ____/
    //  but brownout does this       _____
    //                              |
    //                          ____|
    //
    //  We are concerned about the time under the ramp when the brownout
    //  is too close.
    //--------------------------------------------------------------------------
    if(u16NewTarget > u16CurrentTarget)
    {
        bool bPrevBoSetting;

        //----------------------------------------------------------------------
        // Temporarily change the rail's brownout.
        //----------------------------------------------------------------------
        if(bPoweredByLinReg)
        {
            // Disable detection if powered by linear regulator.  This avoids
            // the problem where the brownout level reaches its new value before
            // the target does.
            bPrevBoSetting = hw_power_EnableVddaBrownoutInterrupt(FALSE);
        }

        {
            // Temporarily use the maximum offset for DCDC transitions.  DCDC
            // transitions step the target and the brownout offset in 25mV
            // increments so there is not a risk of an inverted brownout level.
            hw_power_SetVddaBrownoutValue(BO_MAX_OFFSET_MV);
        }


        //----------------------------------------------------------------------
        // We want to limit voltage step sizes to account for FUNCV changes and
        // linreg voltage change which may result in current surges/voltage dips
        // on VDD5V.  To accomplish this, we will step the voltage in
        // pre-determined levels until the new target is reached.
        //----------------------------------------------------------------------
        do{

            // Calculate the next target to step to.  If we can't get to the
            // target without exceeding the maximum step voltage, then step
            // by the maximum and try again next loop.
            u16TargetDifference = u16NewTarget - u16CurrentTarget;
            if(u16TargetDifference > DDI_POWER_MAX_VOLTAGE_STEP_MV)
            {
                u16StepTarget = u16CurrentTarget + DDI_POWER_MAX_VOLTAGE_STEP_MV;
            }
            else
            {
                u16StepTarget = u16NewTarget;
            }

            // Now change target and wait for it to stabilize.
            hw_power_SetVddaValue(u16StepTarget);
            ddi_power_WaitForVddaStable();

            // Read the current value for the next comparison.
            u16CurrentTarget = hw_power_GetVddaValue();

        }
        while(u16NewTarget > u16CurrentTarget);


        //----------------------------------------------------------------------
        // Clean up any brownout issues and set the new brownout level.
        //----------------------------------------------------------------------
        if(bPoweredByLinReg)
        {
            // Clear the interrupt in case it occured.
            hw_power_ClearVddaBrownoutInterrupt();

            // Restore brownout setting since it was disabled for linreg cases.
            hw_power_EnableVddaBrownoutInterrupt(bPrevBoSetting);
        }

        // Set the real brownout offset.
        hw_power_SetVddaBrownoutValue(u16NewBrownout);


    }


    else
    {
        //----------------------------------------------------------------------
        // We want to limit voltage step sizes to account for FUNCV changes and
        // linreg voltage change which may result in current surges/voltage dips
        // on VDD5V.
        //----------------------------------------------------------------------
        do{

            // Calculate the next target to step to.  If we can't get to the
            // target without exceeding the maximum step voltage, then step
            // by the maximum and try again next loop.
            u16TargetDifference = u16CurrentTarget - u16NewTarget;
            if(u16TargetDifference > DDI_POWER_MAX_VOLTAGE_STEP_MV)
            {
                u16StepTarget = u16CurrentTarget - DDI_POWER_MAX_VOLTAGE_STEP_MV;
            }
            else
            {
                u16StepTarget = u16NewTarget;
            }

            // Now change target and wait for it to stabilize.
            hw_power_SetVddaValue(u16StepTarget);
            ddi_power_WaitForVddaStable();

            // Read the current value for the next comparison.
            u16CurrentTarget = hw_power_GetVddaValue();
        }
        while(u16NewTarget < u16CurrentTarget);


        //----------------------------------------------------------------------
        // Set the new brownout level.
        //----------------------------------------------------------------------
        hw_power_SetVddaBrownoutValue(u16NewBrownout);
    }

    //--------------------------------------------------------------------------
    // Voltage change complete.  Unlock the rail for other threads.
    //--------------------------------------------------------------------------
    ddi_power_UnlockRail(100);


    return rtn;
}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDA voltage
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present values of the VDDA voltage in millivolts
//!
//! \param[in]  none
//!
//! \return     VDDA voltage in millivolts
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVdda(void)
{
    //--------------------------------------------------------------------------
    // Return the converted register value.
    //--------------------------------------------------------------------------

    return hw_power_GetVddaValue();
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDA brownout level voltage
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present value of VDDA brownout in millivolts
//!
//! \param[in]  none
//!
//! \return     VDDA brownout voltage in millivolts
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVddaBrownout(void)
{
    uint16_t VddaBoOffset_mV;
    uint16_t Vdda_mV;
    uint16_t VddaBo;

    //--------------------------------------------------------------------------
    // Read the target and brownout register values.
    //--------------------------------------------------------------------------

    Vdda_mV = hw_power_GetVddaValue();
    VddaBoOffset_mV = hw_power_GetVddaBrownoutValue();


    //--------------------------------------------------------------------------
    // The brownout level is the difference between the target and the offset.
    //--------------------------------------------------------------------------

    VddaBo = Vdda_mV - VddaBoOffset_mV;

    return VddaBo;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Waits on status bit signal or timeout for Vdda stability
//!
//! \fntype     Non-reentrant Function
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_WaitForVddaStable(void)
{
    //--------------------------------------------------------------------------
    // For DCDC cases, wait the prep time, then check if the transition has
    // completed.  If not wait a while and check again.  For linear regulator
    // cases, just wait a specified amount of time.
    //--------------------------------------------------------------------------

    // Obtain the current power source for this rail.
    hw_power_PowerSource_t Source = hw_power_GetVddioPowerSource();

    // Handle the appropriate power source.
    switch( Source )
    {
        case HW_POWER_LINREG_DCDC_OFF:
        case HW_POWER_LINREG_DCDC_READY:
        case HW_POWER_EXTERNAL_SOURCE_5V:

            ddi_power_WaitLinRegStable( MIN_STABLE_WAIT_TIME_LINREG,
                                        MAX_STABLE_WAIT_TIME_LINREG,
                                        RECHECK_WAIT_TIME_LINREG );
            break;

        case HW_POWER_DCDC_LINREG_OFF:
        case HW_POWER_DCDC_LINREG_ON:
        case HW_POWER_DCDC_LINREG_READY:
        default:

            ddi_power_WaitDcdcStable( MIN_STABLE_WAIT_TIME_DCDC,
                                      MAX_STABLE_WAIT_TIME_DCDC,
                                      RECHECK_WAIT_TIME_DCDC );
            break;
    }
}


////////////////////////////////////////////////////////////////////////////////
//! See ddi_power.h for details.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_LimitVddaAndBo(uint16_t *pu16Vdda_mV, uint16_t *pu16Bo_mV)
{
    uint16_t    u16Trg_mV = *pu16Vdda_mV;
    uint16_t    u16Bo_mV = *pu16Bo_mV;
    RtStatus_t  rtn = SUCCESS;

    //--------------------------------------------------------------------------
    // Check Vdda limits. Use different limits depending on whether we are
    // checking safety limits or register limits.
    //--------------------------------------------------------------------------
    if(bEnableVddaSafetyLimits)
    {
        //----------------------------------------------------------------------
        // Make sure Vdda is not above the safe voltage
        //----------------------------------------------------------------------
        if (u16Trg_mV > VDDA_SAFE_MAX_MV)
        {
            u16Trg_mV = VDDA_SAFE_MAX_MV;
            rtn = ERROR_DDI_POWER_VDDA_PARAM_ADJUSTED;
        }

        //----------------------------------------------------------------------
        // Make sure Vdda is not below the safe voltage
        //----------------------------------------------------------------------
        if (u16Trg_mV < VDDA_SAFE_MIN_MV)
        {
            u16Trg_mV = VDDA_SAFE_MIN_MV;
            rtn = ERROR_DDI_POWER_VDDA_PARAM_ADJUSTED;
        }
    }


    //--------------------------------------------------------------------------
    // Check Vdda brownout limits
    //--------------------------------------------------------------------------
    {
        //----------------------------------------------------------------------
        // Make sure there's at least a margin of difference between Vdda
        // and the brownout level.
        //----------------------------------------------------------------------
        if (BO_MIN_OFFSET_MV > (u16Trg_mV - u16Bo_mV))
        {
            u16Bo_mV = u16Trg_mV - BO_MIN_OFFSET_MV;
            rtn = ERROR_DDI_POWER_VDDA_PARAM_ADJUSTED;
        }

        //----------------------------------------------------------------------
        // Make sure the brownout value does not exceed the maximum allowed
        // by the system.
        //----------------------------------------------------------------------
        if ((u16Trg_mV - u16Bo_mV) > BO_MAX_OFFSET_MV)
        {
            u16Bo_mV = u16Trg_mV - BO_MAX_OFFSET_MV;
            rtn = ERROR_DDI_POWER_VDDIO_PARAM_ADJUSTED;
        }

    }

    //--------------------------------------------------------------------------
    // Give the results back to the caller and return.
    //--------------------------------------------------------------------------
    *pu16Vdda_mV = u16Trg_mV;
    *pu16Bo_mV = u16Bo_mV;
    return rtn;
}

void ddi_power_EnableVddaSafetyLimits(bool bEnable)
{
    bEnableVddaSafetyLimits = bEnable;
}

////////////////////////////////////////////////////////////////////////////////
// Power and reset functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! \brief Waits at least the requested amount of time.
//!
//! This function will either call a registered wait function, or the default
//! driver's wait function.  The default wait funciton will wait on the
//! microsecond's counter until the specified time has elapsed.  The registered
//! function can be registered with ddi_power_RegisterWaitFunction.
//!
//! \param[in] u32MicrosecondsToWait Number of microseconds to wait.
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Wait(uint32_t u32MicrosecondsToWait)
{
    uint32_t u32StartTime;

    // Read the time when we are starting to wait.
    u32StartTime = hw_digctl_GetCurrentTime();


    //--------------------------------------------------------------------------
    // The preferred wait method is to suspend the thread for the requested
    // amount of time.  This driver will allow the OS to register a wait
    // function that should be called.  If no function is registered, the
    // wait function will call microsecond timer directly and spin in a
    // while loop.
    //--------------------------------------------------------------------------
    if( ddi_power_WaitCallback != NULL )
    {
        // Make sure the callback does wait at least the requested amount
        // of time.  If not, call it again because coming back early
        // can cause rail instablility in the power block
        while ( !hw_digctl_CheckTimeOut( u32StartTime, u32MicrosecondsToWait ))
        {
            // The function has been registered so call it.
            ddi_power_WaitCallback(u32MicrosecondsToWait);
        }
    }

    else
    {
        // The wait function has not been registered.  Call the digital control
        // wait timeout function that waits until the timer reaches the start
        // time plus the wait time.  This will spin in a while loop.
        while ( !hw_digctl_CheckTimeOut( u32StartTime, u32MicrosecondsToWait ));
    }

    return SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Locks the power rail to prevent access from other threads.
//!
//! \fntype Function
//!
//! Calls an external function to lock the power rail using a mutex or
//! semaphore.
//!
//! \param[in] MsecToWait Milliseconds to wait for lock before timing out.
//!
//! \retval SUCCESS Power rail lock acquired.
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_LockRail( uint32_t MsecToWait )
{
    //--------------------------------------------------------------------------
    // This will lock all three rails.  VDDD and VDDIO are generally only
    // changed during initialization so dedicating a semaphore or mutex to
    // those rails would only waste memory.  If we start dynamically changing
    // those rails, we'll need to implement separate lock functions.
    //--------------------------------------------------------------------------
    if( ddi_power_LockRailCallback != NULL )
    {
        // This requires mutex or semaphore to control multiple thread access
        // to rail.  The callback should acquire one of the control devices.
        ddi_power_LockRailCallback( MsecToWait );
    }

    else
    {
        // Need internal mechanism to block other threads.  Don't include
        // any OS references to avoid coupling this driver with OS.
    }

    return SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Unlocks the power rail to provide other threads access.
//!
//! \fntype Function
//!
//! Calls an external function to unlock the power rail by releasing a mutex or
//! semaphore.
//!
//! \param[in] MsecToWait Milliseconds to wait for unlock before timing out.
//!
//! \retval SUCCESS Power rail lock released.
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_UnlockRail( uint32_t MsecToWait )
{
    //--------------------------------------------------------------------------
    // This requires mutex or semaphore to control multiple thread access to
    // rail.  The callback should release one of the control devices acquired
    // in the Lock API.
    //--------------------------------------------------------------------------
    if( ddi_power_UnlockRailCallback != NULL )
    {
        ddi_power_UnlockRailCallback( MsecToWait );
    }

    else
    {
        // Need internal mechanism to block other threads.  Don't include
        // any OS references to avoid coupling this driver with OS.
    }

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Waits for the linear regulators to become stable.
//!
//! \fntype Function
//!
//! Causes the system to wait for the linear regulators to stabilize.  If the
//! function returns SUCCESS, the linear regulators have a stable outptut.
//!
//! \param[in] u32StableWaitTime Expected time to wait, in microseconds, for
//! the linear regulator to become stable.
//! \param[in] u32MaxWaitTime Maximum microseconds to wait for the linear
//! regulator to become stable.
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_WaitLinRegStable( uint32_t u32MinWaitTime,
                                       uint32_t u32MaxWaitTime,
                                       uint32_t u32RecheckTime )
{
    uint32_t u32WaitStartTime;
    bool bLinRegStable = false;
    bool bMaxWaitTimeElapsed = false;

    //--------------------------------------------------------------------------
    // Read the current time and start waiting for the "minimum wait time"
    //--------------------------------------------------------------------------
    u32WaitStartTime = hw_digctl_GetCurrentTime();
    ddi_power_Wait( u32MinWaitTime );


    //--------------------------------------------------------------------------
    // We can only exit this loop if the linear regulators stablize or
    // if the maximum wait time elapses.
    //--------------------------------------------------------------------------
    while ( bLinRegStable == false && bMaxWaitTimeElapsed == false )
    {
        // Check the linear regulator stability.
        bLinRegStable = ddi_power_IsLinRegStable();

        // Check if we've waited longer than the maximum wait time.
        bMaxWaitTimeElapsed = hw_digctl_CheckTimeOut( u32WaitStartTime, u32MaxWaitTime );

        // Wait some time before rechecking the conditions again.
        ddi_power_Wait( u32RecheckTime );
    }


    //--------------------------------------------------------------------------
    // Check the return status.
    //--------------------------------------------------------------------------
    if( bLinRegStable == true )
    {
        return SUCCESS;
    }
    else
    {
        // TODO: check if the timeout caused the loop to end.  If so
        // return a timeout error.
        return ERROR_DDI_POWER_GENERAL;
    }

}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Returns true.
//!
//! \fntype Function
//!
//! This function just return true and must be called only from
//! ddi_power_WaitLinRegStable().  It is not intended to be used outside of
//! the power driver.
//!
//! \retval True Linear regulator output is stable.
//! \retval False Linear regulator output is not stable.
//!
//! \note The linear regulators may use an interrupt to notify us when they are
//! stable in the future.  We can add this functionality to the wait logic
//! when it becomes available.
/////////////////////////////////////////////////////////////////////////////////
bool ddi_power_IsLinRegStable( void )
{
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//! \brief Waits until the DCDC is stable, or the timeout occurs.
//!
//! This function provides the driver a mechanism to wait for the DCDC to
//! stabilize.  It will check the DCDC stability bit, and if not ready,
//! call the driver's wait function.  If the DCDC is not stable after the
//! maximum timeout value, this funciton will exit.
//!
//!
//!
//! \param[in] u32MinWaitTime Minimum amount of time, in microseconds,
//! to wait before checking if the DCDC has stabilized after its transition.
//!
//! \param[in] u32MaxWaitTime Maximum time, in microseconds, to wait
//! for the DCDC to stabilize.  If the DCDC has not stabilized after this time,
//! the function will exit with a timeout error.
//!
//! \param[in] u32RecheckTime Amount of time, in microseconds, to wait
//! before re-checking if the DCDC has stabilized.
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_WaitDcdcStable( uint32_t u32MinWaitTime,
                                     uint32_t u32MaxWaitTime,
                                     uint32_t u32RecheckTime )
{
    uint32_t u32WaitStartTime;
    bool bDcdcStable = false;
    bool bMaxWaitTimeElapsed = false;

    //--------------------------------------------------------------------------
    // Read the current time and start waiting for the "minimum wait time"
    //--------------------------------------------------------------------------
    u32WaitStartTime = hw_digctl_GetCurrentTime();
    ddi_power_Wait( u32MinWaitTime );


    //--------------------------------------------------------------------------
    // Call the power driver's wait function.  We can only exit this loop if
    // the DCDCs stablize or if the maximum wait time elapses.
    //--------------------------------------------------------------------------
    while ( bDcdcStable == false && bMaxWaitTimeElapsed == false )
    {
        // Check the DCDC stability.
        bDcdcStable = ddi_power_IsDcdcStable();

        // Check if we've waited longer than the maximum wait time.
        bMaxWaitTimeElapsed = hw_digctl_CheckTimeOut( u32WaitStartTime, u32MaxWaitTime );

        // Wait some time before rechecking the conditions again.
        ddi_power_Wait( u32RecheckTime );
    }


    //--------------------------------------------------------------------------
    // Check the return status.
    //--------------------------------------------------------------------------
    if( bDcdcStable == true )
    {
        return SUCCESS;
    }
    else
    {
        // TODO: check if the timeout caused the loop to end.  If so
        // return a timeout error.
        return ERROR_DDI_POWER_GENERAL;
    }
}


/////////////////////////////////////////////////////////////////////////////////
//! \brief Returns transition status of the DCDC
//!
//! \fntype Function
//!
//! Returns the transition status of the DCDC>
//!
//! \retval True DCDC output is stable.
//! \retval False DCDC output is not stable.
//!
/////////////////////////////////////////////////////////////////////////////////
bool ddi_power_IsDcdcStable( void )
{
    return hw_power_CheckDcdcTransitionDone();
}



/////////////////////////////////////////////////////////////////////////////////
//! \brief Waits until the 4p2 rail is stable, or the timeout occurs.
//!
//! \fntype Function
//!
//! The 4p2 rail will become stable after some amount of time.  We will call the
//! register "wait" function.
//!
//! \param[in] u32MinWaitTime Minimum amount of time, in microseconds, to
//! wait before checking if the 4p2 rail has become stable.
//! \param[in] u32MaxWaitTime Maximum amount of time, in microseconds, to
//! wait for the 4p2 rail to stabilize.
//!
//! \retval SUCCESS The 4p2 rail stablilized before the maximum wait time elapsed.
//! \retval ERROR_DDI_POWER_ The maximum wait time elapsed.  The 4p2 rail may not
//! be stable.
//!
//! \note In the future, the 4p2 rail may notify us by interrupt when it
//! becomes stable.  We will incorporate that logic into this function
//! when it becomes available.
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Wait4p2Stable( uint32_t u32MinWaitTime,
                                    uint32_t u32MaxWaitTime,
                                    uint32_t u32RecheckTime )
{
    uint32_t u32WaitStartTime;
    bool b4p2Stable = false;
    bool bMaxWaitTimeElapsed = false;

    //--------------------------------------------------------------------------
    // Read the current time and start waiting for the "minimum wait time"
    //--------------------------------------------------------------------------
    u32WaitStartTime = hw_digctl_GetCurrentTime();
    ddi_power_Wait( u32MinWaitTime );


    //--------------------------------------------------------------------------
    // Call the power driver's wait function.  We can only exit this loop if
    // the 4p2 rail stablizes or if the maximum wait time elapses.
    //--------------------------------------------------------------------------
    while ( b4p2Stable == false && bMaxWaitTimeElapsed == false )
    {
        // Check the 4p2 rail stability.
        b4p2Stable = ddi_power_IsDcdcStable();

        // Check if we've waited longer than the maximum wait time.
        bMaxWaitTimeElapsed = hw_digctl_CheckTimeOut( u32WaitStartTime, u32MaxWaitTime );

        // Wait some time before rechecking the conditions again.
        ddi_power_Wait( u32RecheckTime );
    }


    //--------------------------------------------------------------------------
    // Check the return status.
    //--------------------------------------------------------------------------
    if( b4p2Stable == true )
    {
        return SUCCESS;
    }
    else
    {
        // TODO: check if the timeout caused the loop to end.  If so
        // return a timeout error.
        return ERROR_DDI_POWER_GENERAL;
    }
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Returns true.
//!
//! \fntype Function
//!
//! This function just return true and must be called only from
//! ddi_power_Wait4p2Stable().  It is not intended to be used outside of
//! the power driver.
//!
//! \retval True 4p2 rail's output is stable.
//! \retval False 4p2 rail's output is not stable.
//!
//! \note The 4p2 rail may use an interrupt to notify us when they are
//! stable in the future.  We can add this functionality to the wait logic
//! when it becomes available.
/////////////////////////////////////////////////////////////////////////////////
bool ddi_power_Is4p2Stable( void )
{
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the strength of the internal power FETs
//!
//! This function will set the strength of the FETs depending on the current
//! system requirements.  Generally, stronger FETs will result in more power
//! usage and better performance.  Weaker FETs will result in lower performance
//! and much less power usage.
//!
//! \param [in] Strength FET strength setting
//!
//! \retval SUCCESS
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_SetPowerFetStrength( ddi_power_FetStrength_t Strength )
{
    //--------------------------------------------------------------------------
    // Change the strength of the power FETs.
    //--------------------------------------------------------------------------
    switch( Strength )
    {
        // Half strenth typically used in low power situations.
        case DDI_POWER_FET_STRENGTH_HALF:
            hw_power_EnableHalfFets(true);
            hw_power_EnableDoubleFets(false);
            break;

        // Normal strength for normal audio playback operation.
        case DDI_POWER_FET_STRENGTH_NORMAL:
            hw_power_EnableHalfFets(false);
            hw_power_EnableDoubleFets(false);
            break;

        // Double strength for video playback and high performance situations.
        // Draws extra power for high performance.
        case DDI_POWER_FET_STRENGTH_DOUBLE:
        default:
            hw_power_EnableHalfFets(false);
            hw_power_EnableDoubleFets(true);
            break;
    }

    return SUCCESS;

}

/////////////////////////////////////////////////////////////
//! \brief Returns the logical state of the 4p2 rail.
//!
//! This returns a logical state, not a hardware state.  Unforunatley
//! we cannot determine if 4p2 id definitely active just by reading
//! the regiesters.
//!
//! \retval True The 4p2 rail is enabled
//! \retval False The 4p2 rail is disabled.
//!
/////////////////////////////////////////////////////////////
bool ddi_power_Is4p2Enabled(void)
{
    return b4p2Enabled;
}

/////////////////////////////////////////////////////////////
//! \brief Saves the logical state of the 4p2 rail.
//!
//! This returns a logical state, not a hardware state.  Unforunatley
//! we cannot determine if 4p2 id definitely active just by reading
//! the regiesters.
//!
//! \param[in] b4p2IsEnabled Logical state of the 4p2 rail.  Set to
//! true if 4p2 is enabled and false if disabled.
//!
/////////////////////////////////////////////////////////////
void ddi_power_Save4p2Enabled( bool b4p2IsEnabled )
{
    b4p2Enabled = b4p2IsEnabled;
}


/////////////////////////////////////////////////////////////////////////////////
//! \brief Enables the 4p2 rail and readies it for use.
//!
//! \fntype Function
//!
//! This function enables the 4p2 rail and DCDC from 4p2 if 5V is connected.  If
//! 5V is not connected, this functionality will cause 4p2 and DCDC from 4p2 to
//! become immediately active once 5V is connected and valid.  If successful,
//! the logical state of the 4p2 rail will be changed to "enabled"
//!
//! \retval SUCCESS The 4p2 rail is ready for use.
//! \retval ERROR_DDI_FAILED_TO_PREPARE_4P2_RAIL The system could not prepare
//! power supplies to use the 4p2 rail when 5V becomes present.
//! \retval ERROR_DDI_FAILED_TO_START_4P2_RAIL The 4p2 could not start properly.
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Enable4p2( void )
{
    RtStatus_t PrepStatus, StartStatus;

    //--------------------------------------------------------------------------
    // We need to start or prepare to start the 4p2 rail depending on the
    // presence of 5V.
    //--------------------------------------------------------------------------
    if( hw_power_Get5vPresentFlag() )
    {
        // 5V is present.  We need to power on the 4p2 circuitry, then charge
        // the capacitor and the rail itself before enabling it and allowing
        // the DCDC to use it.
        PrepStatus  = ddi_power_PrepareToStart4p2();
        StartStatus = ddi_power_Start4p2();
    }

    else
    {
        // 5V is not present.  We will prepare the 4p2 rail to power the DCDC
        // when it does become present and valid.  The start 4p2 function will be
        // called later.
        PrepStatus = ddi_power_PrepareToStart4p2();
        StartStatus = SUCCESS;
    }


    //--------------------------------------------------------------------------
    // Save the 4p2 rail's logical state.  Does not save or set the physical
    // state of the 4p2 rail.
    //--------------------------------------------------------------------------
    if( PrepStatus==SUCCESS && StartStatus==SUCCESS )
    {
        ddi_power_Save4p2Enabled( true );
    }
    else
    {
        ddi_power_Save4p2Enabled( false );
    }

    //--------------------------------------------------------------------------
    // Return the error code from PrepareToStart4p2 first.  If it was successful
    // return the status of ddi_power_Start4p2().
    //--------------------------------------------------------------------------
    if( PrepStatus != SUCCESS )
    {
        return PrepStatus;
    }
    else
    {
        return StartStatus;
    }
}


///////////////////////////////////////////////////////////////////////////////
//! \brief Disables the 4p2 rail.
//!
//! The 4p2 rail will not be used as power source until it is enabled again.
//! The logical state of the 4p2 rail will be changed to "disabled".
//!
//! \retval SUCCESS
///////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Disable4p2( void )
{
    //--------------------------------------------------------------------------
    // Stop using the 4p2 rail as DCDC power source.
    //--------------------------------------------------------------------------
    ddi_power_Stop4p2();


    //--------------------------------------------------------------------------
    // Save the 4p2 rail's logical state.
    //--------------------------------------------------------------------------
    ddi_power_Save4p2Enabled( false );

    return SUCCESS;
}


/////////////////////////////////////////////////////////////////////////////////
//! \brief Prepare the 4p2 rail for use when 5V becomes valid.
//!
//! \fntype Function
//!
//! When 5V is not valid, the 4p2 rail will be prepared so it can turn on
//! when 5V does become valid.
//!
//! \retval SUCCESS The 4p2 rail is ready to turn on when 5V is valid.
//! \retval ERROR_DDI_FAILED_TO_PREPARE_4P2_RAIL One of the three power sources,
//! VDDD, VDDA, or VDDIO, was not set properly.
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_PrepareToStart4p2( void )
{
    RtStatus_t Status;

    //----------------------------------------------------------------------
    // For 4p2 mode enabled, the os level driver will control the handoff
    // from DCDC battery to DCDC 4p2 on a 5V connection.  We do not
    // want automatic handoffs to linear regulators.  We will set up the
    // power block to use DCDCs even when 5V is present.
    //----------------------------------------------------------------------

    if((Status = hw_power_SetVdddPowerSource(HW_POWER_DCDC_LINREG_READY)) != SUCCESS)
        return ERROR_DDI_FAILED_TO_PREPARE_4P2_RAIL;
    if((Status = hw_power_SetVddaPowerSource(HW_POWER_DCDC_LINREG_READY)) != SUCCESS)
        return ERROR_DDI_FAILED_TO_PREPARE_4P2_RAIL;
    if((Status = hw_power_SetVddioPowerSource(HW_POWER_DCDC_LINREG_READY)) != SUCCESS)
        return ERROR_DDI_FAILED_TO_PREPARE_4P2_RAIL;

    return Status;
}


/////////////////////////////////////////////////////////////////////////////////
//! \brief Activates the 4p2 rail to allow the DCDC to use it as a power source.
//!
//! \fntype Function
//!
//! The 4p2 rail will be taken from a completely off state to a fully powered
//! state.  When this function completes, the DCDC will be powered from the
//! 4p2 rail.
//!
//! \retval SUCCESS
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Start4p2( void )
{
    RtStatus_t rtn;

    //--------------------------------------------------------------------------
    // Turn on the 4p2 hardware.
    //--------------------------------------------------------------------------
    if( ( rtn = hw_power_PowerOn4p2Rail() ) != SUCCESS )
        return rtn;


    hw_power_Set4p2CmpTripPoint( CMPTRIP_85PCT );

    //--------------------------------------------------------------------------
    // Charge the capcacitor first, then the rail.  Charging the rails will
    // prevent the 4p2 rail from causing a dip on the 5V supply.
    //--------------------------------------------------------------------------
    if( ( rtn = ddi_power_Charge4p2Cap( CHARGE_CURRENT_LIMIT_4P2_CAP,
                                        CHARGE_CURRENT_STEP_SIZE_4P2_CAP)) != SUCCESS )
        return rtn;

    if( ( rtn = ddi_power_Charge4p2Rail( CHARGE_CURRENT_LIMIT_4P2_RAIL,
                                         CHARGE_CURRENT_STEP_SIZE_4P2_RAIL)) != SUCCESS )
        return rtn;


    //--------------------------------------------------------------------------
    // Switch the DCDC to use the 4p2 rail as a power source and return.
    //--------------------------------------------------------------------------
    hw_power_Enable4p2Rail();

    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//! \brief Stops DCDC 4p2, Powers down 4p2, and powers off internal 4p2 circuit
//!
//! \fntype Function
//!
//! Turns off the 4p2 rail.  The 4p2 rail will be powered down, but it is
//! ready to be turned on again.
//!
//! \retval SUCCESS The 4p2 rail was turned off successfully.
///////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Stop4p2( void )
{
    //--------------------------------------------------------------------------
    // Switch the DCDC power source to battery or internal linear regulator.
    //--------------------------------------------------------------------------
    hw_power_Disable4p2Rail();

    //--------------------------------------------------------------------------
    // Turns off the 4p2 rail hardware.
    //--------------------------------------------------------------------------
    hw_power_PowerOff4p2Rail();

    return SUCCESS;
}


/////////////////////////////////////////////////////////////////////////////////
//! \brief Charges the 4p2 rail.
//!
//! \fntype Function
//!
//! Brings the rail's power up to a level that is usable by the DCDC.  The step
//! size can be adjusted to speed up the charge, but 5V must be monitored to
//! ensure a brownout does not occur.
//!
//! \param[in] u32NewChargeCurrentLimit New current limit, in milliamps.
//! \param[in] u32ChargeCurrentStepSize Amount of current, in milliamps, to
//! increment in each iteration.
//!
//! \retval SUCCESS
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Charge4p2Rail( uint32_t u32NewChargeCurrentLimit,
                                    uint32_t u32ChargeCurrentStepSize )
{
    uint32_t u32ChargeCurrentLimit = 0;


    //--------------------------------------------------------------------------
    // We need to charge the 4p2 rail before using it.  We will step up the
    // current until we reach the requested current.
    //--------------------------------------------------------------------------

    // Start the ramp up from the current limit already in use.
    u32ChargeCurrentLimit = hw_power_GetCharge4p2CurrentLimit();

    // Now start stepping up the current and loop until we reach
    // the new current limit.
    while( u32ChargeCurrentLimit < u32NewChargeCurrentLimit )
    {
        // Increment the charge current limit.
        u32ChargeCurrentLimit += u32ChargeCurrentStepSize;
        hw_power_SetCharge4p2CurrentLimit( u32ChargeCurrentLimit );

        // Wait for the rail to stabilize.
        ddi_power_Wait4p2Stable( MIN_RAIL_STABLE_WAIT_TIME_4P2,
                                 MAX_RAIL_STABLE_WAIT_TIME_4P2,
                                 RAIL_RECHECK_WAIT_TIME_4P2 );
    }


    return SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Charges the 4p2 capacitor.
//!
//! \fntype Function
//!
//! Applies an initial charge and voltage to the 4p2 capacitor.  This is
//! primarily for very low or dead battery conditions since the 4p2 pin will
//! be at a very low level.
//!
//! \param[in] u32NewChargeCurrentLimit The new current limit, in milliamps, that
//! should be set in hardware after the 4p2 capacitor has been charged.
//! \param[in] u32ChargeCurrentStepSize The amount of current, in milliamps, that
//! each iteration should increment by.
//!
//! \retval SUCCESS
//!
//! \note  This isn't the only thing needed because you can't enable DCDC for 4p2
//! until the circuiry is ready regardless of what voltage is present on 4p2.
//! This is mostly for the very low or dead battery cases.
//! \note  If there is external load on 4p2, we could get stuck here.
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Charge4p2Cap( uint32_t u32NewChargeCurrentLimit,
                                   uint32_t u32ChargeCurrentStepSize )
{
    uint32_t u32StartTime = 0;
    bool bMaxChargeTimeElapsed = false;

    //--------------------------------------------------------------------------
    // We are going to slowly charge the 4p2 capacitor until the voltage level
    // rises above the brownout level.  In some cases, the 4p2 level may not
    // rise above the brownout level because of an external load, so we will also
    // charge for a maximum amount of time.
    //--------------------------------------------------------------------------

    // Read the current time so we know when we started charging.
    u32StartTime = hw_digctl_GetCurrentTime();

    // Now start charging the rail until the voltage rises above the brownout
    // level or the maximum charge time has elapsed.
    while( hw_power_GetDcdc4p2Brownout() == true &&
           bMaxChargeTimeElapsed == false )
    {
        uint32_t u32ChargeCurrentLimit = 0;


        //----------------------------------------------------------------------
        // Slowly raise the current to charge the capacitor.
        //----------------------------------------------------------------------
        u32ChargeCurrentLimit = hw_power_GetCharge4p2CurrentLimit();
        if(u32ChargeCurrentLimit < u32NewChargeCurrentLimit)
        {
            // Increase charge current limit we are using.
            u32ChargeCurrentLimit += u32ChargeCurrentStepSize;
            hw_power_SetCharge4p2CurrentLimit( u32ChargeCurrentLimit );
        }


        //----------------------------------------------------------------------
        // Wait some time for the 4p2 voltage and brownout to stabalize before
        // re-checking the brownout status and the elapsed charge time.
        //----------------------------------------------------------------------
        ddi_power_Wait4p2Stable( MIN_STABLE_WAIT_TIME_4P2,
                                 MAX_STABLE_WAIT_TIME_4P2,
                                 RECHECK_WAIT_TIME_4P2 );

        bMaxChargeTimeElapsed = hw_digctl_CheckTimeOut( u32StartTime,
                                                        MAX_4P2_CAP_CHARGE_TIME);


        //----------------------------------------------------------------------
        // If 5V goes away during this time, completely turn off 4p2 and return.
        //----------------------------------------------------------------------
        if(!hw_power_Get5vPresentFlag())
        {
            ddi_power_Stop4p2();
            return ERROR_DDI_POWER_GENERAL;
        }
    }

    return SUCCESS;
}



////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Enable or Disable safe voltage limits.  Only disable for testing
//! needs
//!
//! \fntype Function
//!
//! \retval none
////////////////////////////////////////////////////////////////////////////////
void ddi_power_EnableSafeVoltageLimits(bool bEnable)
{
    bUseSafeVoltageLimits = bEnable;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Determines if the battery is charged enough to power DCDC's after
//! a handoff.
//!
//! \fntype Function
//!
//! \retval true Safe voltage limits are enabled.
//! \retval false Safe voltage limits are disabled.
////////////////////////////////////////////////////////////////////////////////
bool ddi_power_GetSafeVoltageLimitsStatus(void)
{
    return bUseSafeVoltageLimits;
}
////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}





