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
#include "ddi_power_internal.h"
//#include "os/pmi/os_pmi_api.h"
//#include "drivers/batterycharger/ddi_bc.h"
#include "registers/regspower.h"
#include "hw/digctl/hw_digctl.h"
#include "hw/icoll/hw_icoll.h"

////////////////////////////////////////////////////////////////////////////////
// Globals & Variables
////////////////////////////////////////////////////////////////////////////////
static bool bBatteryConnected = false;
static bool bBatteryConnectionTestComplete = false;


////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// DCDC and 5V Handoff
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable 5V-to-battery handoff
//!
//! \fntype Function
//!
//! This function prepares the hardware for a 5V-to-battery handoff.  It assumes
//! the current configuration is using 5V as the power source.  The 5V
//! interrupt will be set up for a 5V removal.
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_Enable5VoltsToBatteryHandoff(void)
{

    //--------------------------------------------------------------------------
    // Enable detection of 5V removal (unplug)
    //--------------------------------------------------------------------------

    hw_power_Enable5vUnplugDetect(TRUE);


    //hw_power_EnableAutoDcdcTransfer(TRUE) causes strange behavior when 4p2 is enabled.
    if(ddi_power_Is4p2Enabled())
    {
        if( BATT_VOLT__4P2TRG_THRESHOLD < hw_power_GetBatteryVoltage() )
        {
            HW_POWER_DCDC4P2.B.TRG = 0; // 4.2V
        }
        else
        {
            HW_POWER_DCDC4P2.B.TRG = 3; // 3.9V
        }

        hw_power_ClearVdd5vDroopInterrupt();
        hw_power_EnableVdd5vDroopInterrupt( true );
        hw_icoll_EnableVector((ICOLL_IRQ_enums_t)(VECTOR_IRQ_VDD5V_DROOP), true );
        return;
    }


    //--------------------------------------------------------------------------
    // Enable automatic transition to DCDC
    //--------------------------------------------------------------------------
    hw_power_EnableAutoDcdcTransfer(TRUE);

    //--------------------------------------------------------------------------
    // Disable hardware power down when 5V is removed since handoff to DC-DC
    // has been enabled.
    //--------------------------------------------------------------------------
    hw_power_Enable5vBrownoutPowerdown( false );
}


////////////////////////////////////////////////////////////////////////////////
//! \brief Transfers the power source from 5V to the battery.
//!
//! \fntype Function
//!
//! This function will handle all the power rail transitions necesarry to power
//! the chip from the battery when it was previously powered from the 5V power
//! source.
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_Execute5VoltsToBatteryHandoff(void)
{

    if(ddi_power_Is4p2Enabled())
    {
        //----------------------------------------------------------------------
        // Stop the 4p2 rail from powering the DCDC
        //----------------------------------------------------------------------
        ddi_power_Stop4p2();

        //--------------------------------------------------------------------------
        // Disable hardware power down when 5V is inserted or removed
        //--------------------------------------------------------------------------
        hw_power_Enable5vBrownoutPowerdown( false );

        //--------------------------------------------------------------------------
        // Re-enable the battery brownout interrupt in case it was disabled.
        //--------------------------------------------------------------------------
        hw_power_EnableBatteryBrownoutInterrupt(TRUE);

        return;
    }

    //--------------------------------------------------------------------------
    // We are about to power the chip from battery.  If the battery level is
    // at the hard brownout level, the FIQ handler will shut down the chip.
    // Set the hard level and enable the battery interrupt.
    //--------------------------------------------------------------------------
    hw_power_SetBatteryBrownoutCode( g_ddi_power_BatteryBrownOutVoltageCode );
    hw_power_EnableBatteryBrownoutInterrupt( true );

    // Enforce battery powered operation
    hw_power_EnableDcdc(true);

    //--------------------------------------------------------------------------
    if(hw_power_GetVdddPowerSource() == HW_POWER_EXTERNAL_SOURCE_5V)
	{
		// When powered from external sources, we need to temporarily
		// increase the target to prevent the DCDC from fighting
		// with the external source.  PMI will change the target to
		// the correct voltage after the source transition.
		hw_power_SetVdddBrownoutValue(175);
		hw_power_SetVdddValue(1575);
		ddi_power_WaitForVdddStable();
		hw_power_SetVdddBrownoutValue(125);
	}

	hw_power_SetVdddPowerSource(HW_POWER_DCDC_LINREG_READY);

    //--------------------------------------------------------------------------
    // Power VDDA and VDDIO from the DCDC.
    //--------------------------------------------------------------------------
    hw_power_SetVddaPowerSource(HW_POWER_DCDC_LINREG_READY);
    hw_power_SetVddioPowerSource(HW_POWER_DCDC_LINREG_ON);

    //--------------------------------------------------------------------------
    // Disable hardware power down when 5V is inserted or removed
    //--------------------------------------------------------------------------
    hw_power_Enable5vBrownoutPowerdown( false );

    //--------------------------------------------------------------------------
    // Transition to battery power went smoothly.  Now reset the soft battery
    // level for normal battery operation.
    //--------------------------------------------------------------------------
    hw_power_SetBatteryBrownoutCode( g_ddi_power_SafeBatteryVoltageCode );
    hw_power_EnableBatteryBrownoutInterrupt( true );


}

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable battery-to-5V handoff
//!
//! \fntype Function
//!
//! This function sets up battery-to-5V handoff. The power switch from
//! battery to 5V is automatic. This funtion enables the 5V present detection
//! such that the 5V interrupt can be generated if it is enabled. (The interrupt
//! handler can inform software the 5V present event.) To deal with noise or
//! a high current, this function enables DCDC1/2 based on the battery mode.
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_EnableBatteryTo5VoltsHandoff(void)
{
    //--------------------------------------------------------------------------
    // Enable 5V plug-in detection
    //--------------------------------------------------------------------------
    hw_power_Enable5vPlugInDetect(TRUE);

    //--------------------------------------------------------------------------
    // Allow DCDC be to active when 5V is present.
    //--------------------------------------------------------------------------
    hw_power_EnableDcdc(true);


}

////////////////////////////////////////////////////////////////////////////////
//! \brief Transfers the  power source from battery to 5V.
//!
//! \fntype Function
//!
//! This function handles the transitions on each of the power rails necessary
//! to power the chip from the 5V power supply when it was previously powered
//! from the battery power supply.
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_ExecuteBatteryTo5VoltsHandoff(void)
{


    //----------------------------------------------------------------------
    // Start the 4p2 rail and power DCDC from it.
    //----------------------------------------------------------------------
    if(ddi_power_Is4p2Enabled())
    {
        ddi_power_Start4p2();
        return;
    }


    //--------------------------------------------------------------------------
    // We are about to power the chip solely from 5V, and possibly without
    // handoffs enabled.  We need to make sure the chip shuts down properly
    // if 5V is removed in this window.
    //--------------------------------------------------------------------------
    hw_power_Enable5vBrownoutPowerdown( true );

    hw_power_SetVdddPowerSource(HW_POWER_LINREG_DCDC_READY);

    hw_power_SetVddaPowerSource(HW_POWER_LINREG_DCDC_READY);
    hw_power_SetVddioPowerSource(HW_POWER_LINREG_DCDC_READY);

    //----------------------------------------------------------------------
    // Disable the DCDC during 5V connections.
    //----------------------------------------------------------------------
    hw_power_EnableDcdc(false);

}



////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Determines if the battery is charged enough to power DCDC's after
//! a handoff.
//!
//! \fntype Function
//!
//! \retval true Battery is sufficiently charged to power DCDC.
//! \retval false Battery is not sufficient to power DCDC.
////////////////////////////////////////////////////////////////////////////////
bool ddi_power_IsBattValidForHandoff( void )
{
    uint16_t u16SafeBattVolt;

    // First, check if we have a battery
    if( bBatteryConnected == false )
    {
        return false;
    }

    // Determine the brownout voltage.
    u16SafeBattVolt = hw_power_ConvertSettingToBattBo( g_ddi_power_SafeBatteryVoltageCode);

    // Check if the battery level is high enough for DCDC to operate.
    if( hw_power_GetBatteryVoltage() > u16SafeBattVolt )
        return true;
    else
        return false;


}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Saves the battery connected status.
//!
//! \fntype Function
//!
//! \param[in] bConnected True if battery is connected, false if no battery is
//! present.
/////////////////////////////////////////////////////////////////////////////////
void ddi_power_BatteryConnected( bool bConnected )
{
    bBatteryConnected = bConnected;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Determines if a battery is connected to the device.
//!
//! \fntype Function
//!
//! Senses the battery pin to determine if a real battery is present.
//!
//! \retval True A battery is connected to the device.
//! \retval False A battery is not connected to the device.
/////////////////////////////////////////////////////////////////////////////////
bool ddi_power_IsBatteryConnected( void )
{
    // If we have already run the test, just send back the result.
    if( bBatteryConnectionTestComplete )
    {
        return bBatteryConnected;
    }
    else
    {
        bBatteryConnected = ddi_power_TestBatteryConnection();
        return bBatteryConnected;
    }
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Runs the test to determine if a battery is connected to device.
//!
//! \fntype Function
//!
//! The test will turn on the charger with minimal charge current to determine
//! if a battery is present.  This test should only be run once since subsequent
//! tests may be invalid due to the charge left on the battery pin from
//! capacitance.
//!
//! \retval True Test determined battery is connected.
//! \retval False Test determined battery is not connected.
/////////////////////////////////////////////////////////////////////////////////
bool ddi_power_TestBatteryConnection( void )
{
    //--------------------------------------------------------------------------
    // Check if we've already run the test before.  If so, we need to return
    // the result instead of running the test again.
    //--------------------------------------------------------------------------
    if( DidCurrAppRunBattTest() )
    {
        // This application ran the test once, so the battery connection flag is valid.
        return bBatteryConnected;
    }

    if( DidPrevAppRunBattTest() )
    {
        // If a previous application ran this test, return the result.
        return GetPrevAppTestResult();
    }


    //--------------------------------------------------------------------------
    // Let's make sure we have a stable battery voltage first.
    //--------------------------------------------------------------------------
    {
        uint16_t StartVolt, FirstVolt, SecondVolt;
        bool bVoltageStable = false;
        bool bPrevEnLoad = false;

        // Load the battery pin to drain the voltage if an open circuit.
        bPrevEnLoad = HW_POWER_CHARGE.B.ENABLE_LOAD;
        HW_POWER_CHARGE.B.ENABLE_LOAD = 1;

        StartVolt = hw_power_GetBatteryVoltage();
        while( !bVoltageStable )
        {
            // Take a reading, then wait some time before taking the
            // second reading.
            FirstVolt = hw_power_GetBatteryVoltage();
            hw_digctl_MicrosecondWait( 100000 );
            SecondVolt = hw_power_GetBatteryVoltage();

            // Are the two measurements within the stable margin?
            if( FirstVolt > SecondVolt )
            {
                if( FirstVolt - SecondVolt < 25 )
                    bVoltageStable = true;
            }
            else
            {
                if( SecondVolt - FirstVolt < 25 )
                    bVoltageStable = true;
            }

            // Has the voltage dropped significantly since test started?
            if( StartVolt > SecondVolt )
            {
                if( StartVolt - SecondVolt > 200 )
                {
                    SetPrevAppTestResult( false );
                    bBatteryConnectionTestComplete = true;
                    return false;
                }
            }

            else
            {
                if( SecondVolt - StartVolt > 200 )
                {
                    SetPrevAppTestResult( false );
                    bBatteryConnectionTestComplete = true;
                    return false;
                }
            }
        }

        HW_POWER_CHARGE.B.ENABLE_LOAD = bPrevEnLoad;
    }

    //--------------------------------------------------------------------------
    // Before we turn on the charger, first check if the battery voltage is
    // high enough to indicate a battery is present.
    //--------------------------------------------------------------------------
    if( hw_power_GetBatteryVoltage() > BATT_TEST__MIN_BATT_VOLT )
    {
        // If have any battery voltage at this point, then there is a battery.
        SetPrevAppTestResult( true );
        bBatteryConnectionTestComplete = true;
        return true;
    }


    //--------------------------------------------------------------------------
    // Run the battery detection test.
    //--------------------------------------------------------------------------
    {
        bool bBattCon;

        //----------------------------------------------------------------------
        // Save the previous settings, then initialize the charger for the
        // charger test.
        //----------------------------------------------------------------------
        bool prevBATTCHRG_I = HW_POWER_CHARGE.B.BATTCHRG_I;
        bool prevCHARGE_4P2_ILIMIT = HW_POWER_5VCTRL.B.CHARGE_4P2_ILIMIT;
        bool prevPWD_BATTCHRG = HW_POWER_CHARGE.B.PWD_BATTCHRG;
        bool prevPWD_CHARGE_4P2 = HW_POWER_5VCTRL.B.PWD_CHARGE_4P2;
        bool prevENABLE_4P2 = HW_POWER_DCDC4P2.B.ENABLE_4P2;

        // Set the total 4P2/CHRG current limit to 30mA
        HW_POWER_5VCTRL.B.CHARGE_4P2_ILIMIT = 3;

        // Turn on 4p2 rail and wait a little bit to let it start up.
        HW_POWER_DCDC4P2.B.ENABLE_4P2 = 1;
        HW_POWER_5VCTRL.B.PWD_CHARGE_4P2 = 0;
        hw_digctl_MicrosecondWait( 10 );

        // Turn on the charger with 30mA of charge current
        HW_POWER_CHARGE.B.BATTCHRG_I = 3;
        HW_POWER_CHARGE.B.PWD_BATTCHRG = 0;


        //----------------------------------------------------------------------
        // The battery pin has started charging.  The voltage on the battery pin
        // of an open circuit will jump to about 4.2V.  A real battery will
        // sink the small amount of current and the voltage will stay very low.
        //----------------------------------------------------------------------
        {
            bool bThresholdReached = false;
            bool bTimeoutExpired = false;
            uint32_t StartTime;

            StartTime = hw_digctl_GetCurrentTime();
            while( !bThresholdReached && !bTimeoutExpired )
            {
                // Check voltage threshold.
                if( hw_power_GetBatteryVoltage() > BATT_TEST__MIN_OPEN_CIRC_VOLT )
                {
                    // If the threshold was reached first, then we can
                    // assume we have an open circuit.
                    bThresholdReached = true;
                    bBattCon = false;
                }

                // Check timeout.
                if( ( hw_digctl_GetCurrentTime() - StartTime ) > BATT_TEST__WAIT_TIME_US )
                {
                    // If the timeout expired, then we assume a battery is
                    // present and it sank the current.
                    bTimeoutExpired = true;
                    bBattCon = true;
                }
            }
        }


        //----------------------------------------------------------------------
        // Restore the previous settings and return our result.
        //----------------------------------------------------------------------
        HW_POWER_CHARGE.B.PWD_BATTCHRG = prevPWD_BATTCHRG;
        HW_POWER_CHARGE.B.BATTCHRG_I = prevBATTCHRG_I;
        HW_POWER_5VCTRL.B.CHARGE_4P2_ILIMIT = prevCHARGE_4P2_ILIMIT;
        HW_POWER_5VCTRL.B.PWD_CHARGE_4P2 = prevPWD_CHARGE_4P2;
        HW_POWER_DCDC4P2.B.ENABLE_4P2 = prevENABLE_4P2;

        SetPrevAppTestResult(bBattCon);
        bBatteryConnectionTestComplete = true;
        return bBattCon;
    }

}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Determines if the current application has already run the
//! battery connection test.
//!
//! \retval True Current application has already run the test once.
//! \retval False Current application has not yet run the test.
/////////////////////////////////////////////////////////////////////////////////
bool DidCurrAppRunBattTest( void )
{
    return bBatteryConnectionTestComplete;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Determines if a previous application has already run the
//! battery connection test.
//!
//! \retval True A previous application has already run the test once.
//! \retval False A previous application has not yet run the test.
/////////////////////////////////////////////////////////////////////////////////
bool DidPrevAppRunBattTest( void )
{
    // If this bit is set, then a previous application ran this test.
    if( HW_POWER_CHARGE.B.CHRG_STS_OFF )
        return true;
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Saves the battery connection test result for a previous test.
//!
//! \param[in] bResult True if battery is present, false if not present.
/////////////////////////////////////////////////////////////////////////////////
void SetPrevAppTestResult( bool bResult )
{
    // Now set the flag to let subsequent apps know the test already ran once.
    // We will use the default value (0) of this bitfield to indicate a battery is
    // present.  If we set the bitfield, then we know there is no battery.
    HW_POWER_CHARGE.B.CHRG_STS_OFF = !bResult;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Returns the battery connection test result for a previous test.
//!
//! \retval True Battery detected in a previous test.
//! \retval False Battery not detected in a previous test.
/////////////////////////////////////////////////////////////////////////////////
bool GetPrevAppTestResult( void )
{
    //--------------------------------------------------------------------------
    // We stored the test result somewhere. Find it and return it.
    //--------------------------------------------------------------------------

    // We will use the default value (0) of this bitfield to indicate a battery is
    // present.  If we set the bitfield, then we know there is no battery.
    if( HW_POWER_CHARGE.B.CHRG_STS_OFF == 0)
    {
        // Battery previously detected.
        return true;
    }
    else
    {
        // No battery previously detected.
        return false;
    }

}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
