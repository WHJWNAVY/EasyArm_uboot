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

#include "registers/regspower.h"
#include "hw/power/hw_power.h"
#include "hw/digctl/hw_digctl.h"


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
bool bMasterFlagsInitialized = false;
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_Init(void)
{
    RtStatus_t rtn = SUCCESS;

    //--------------------------------------------------------------------------
    // Make the clock registers accessible by ungating power block
    //--------------------------------------------------------------------------
    //BF_CLR(POWER_CTRL, CLKGATE);

    //--------------------------------------------------------------------------
    // Improve efficieny and reduce transient ripple
    //--------------------------------------------------------------------------

    {
        //----------------------------------------------------------------------
        // Change stepping voltage only after the diff control loop
        // has toggled as well.
        //----------------------------------------------------------------------
        BF_SET(POWER_LOOPCTRL, TOGGLE_DIF);

        //----------------------------------------------------------------------
        // Enable the commom mode and differential mode hysterisis
        //----------------------------------------------------------------------
        BF_SET(POWER_LOOPCTRL, EN_CM_HYST);
        BF_SET(POWER_LOOPCTRL, EN_DF_HYST);



        //----------------------------------------------------------------------
        // To run with a lower battery voltage, adjust the duty
        // cycle positive limit
        //----------------------------------------------------------------------
        BF_WR(POWER_DCLIMITS, POSLIMIT_BUCK, 0x30);



    }

    //--------------------------------------------------------------------------
    // Finally enable the battery adjust
    //--------------------------------------------------------------------------
    BF_SET(POWER_BATTMONITOR,EN_BATADJ);

	//----------------------------------------------------------------------
	// Increase the RCSCALE_THRESHOLD
	//----------------------------------------------------------------------
	BF_SET(POWER_LOOPCTRL, RCSCALE_THRESH);

	//----------------------------------------------------------------------
	// Increase the RCSCALE level for quick DCDC response to dynamic load
	//----------------------------------------------------------------------
	hw_power_EnableRcScale(HW_POWER_RCSCALE_8X_INCR);

	hw_power_EnableHalfFets(FALSE);
	hw_power_EnableDoubleFets(TRUE);
    //--------------------------------------------------------------------------
    // Initialize the variables we use in hw_power driver.
    //--------------------------------------------------------------------------
    if( bMasterFlagsInitialized == false )
    {
        // Initialize the brownout variables and synchronize these with the
        // master brownout register.
        bEnableMaster5vBrownout = HW_POWER_5VCTRL.B.PWDN_5VBRNOUT;
        bEnableMasterBattBrownout = HW_POWER_BATTMONITOR.B.PWDN_BATTBRNOUT;

        // Initialize the 4p2 and charger variables.  Both the local and the
	// master have to be true for the variable to be true.

        bEnableMaster4p2 = HW_POWER_DCDC4P2.B.ENABLE_4P2 &&
		!HW_POWER_5VCTRL.B.PWD_CHARGE_4P2;
        bEnableMasterCharge = !HW_POWER_CHARGE.B.PWD_BATTCHRG &&
		!HW_POWER_5VCTRL.B.PWD_CHARGE_4P2;


        bMasterFlagsInitialized = true;
    }

	/* Enable 5V to battery source handoff on detection of 5V
	 * disconnection.  Get the DCDC control loop ready for
	 * action.  Leaving DCDC_XFER set causes problems with 4p2
	 * operation and has other issues so we only leave
	 * it on long enough to prepare the DCDC control loop
	 */
	HW_POWER_5VCTRL_SET(BM_POWER_5VCTRL_DCDC_XFER);
	hw_digctl_MicrosecondWait(30);
	HW_POWER_5VCTRL_CLR(BM_POWER_5VCTRL_DCDC_XFER);


    return rtn;
}


RtStatus_t hw_power_InitBatteryMonitor(hw_lradc_DelayTrigger_t eTrigger,
	 uint32_t u32SamplingInterval)
{
	RtStatus_t rtn;


        //----------------------------------------------------------------------
        // Start up the battery measurements
        //----------------------------------------------------------------------
        rtn = hw_lradc_EnableBatteryMeasurement(eTrigger,
                                          u32SamplingInterval);


        // wait to make sure first automatic conversion and copy is complete
        // Todo: replace this with a manual first conversion and write to
        // BATT_VAL field.
        hw_digctl_MicrosecondWait(30);



        return rtn;
}

void hw_power_Init4p2EnabledPowerSupply(void)
{

	if(hw_power_GetBatteryVoltage()>3900)
		HW_POWER_DCDC4P2.B.CMPTRIP = 0;
	else
		HW_POWER_DCDC4P2.B.CMPTRIP = 24;

	HW_POWER_DCDC4P2.B.TRG = 0;
	HW_POWER_5VCTRL.B.HEADROOM_ADJ = 0x4;
	HW_POWER_DCDC4P2.B.DROPOUT_CTRL = 0xA;  //100mV drop before stealing
		// charging current
	HW_POWER_5VCTRL.B.CHARGE_4P2_ILIMIT = 0x3f;
}


RtStatus_t hw_power_InitPowerSupplies(void)
{
    RtStatus_t Status;

    //----------------------------------------------------------------------
    // Make sure the power supplies are configured for their power sources.
    // This sets the LINREG_OFFSET field correctly for each power supply.
    //---------------------------------------------------------------------

    if(hw_power_Get5vPresentFlag())
    {

        if((Status = hw_power_SetVdddPowerSource(HW_POWER_LINREG_DCDC_READY))
		 != SUCCESS)
            return Status;
        if((Status = hw_power_SetVddaPowerSource(HW_POWER_LINREG_DCDC_READY))
		 != SUCCESS)
            return Status;
        if((Status = hw_power_SetVddioPowerSource(HW_POWER_LINREG_DCDC_READY))
		 != SUCCESS)
            return Status;
    }
    else
    {

        if((Status = hw_power_SetVdddPowerSource(HW_POWER_DCDC_LINREG_READY))
		 != SUCCESS)
            return Status;
        if((Status = hw_power_SetVddaPowerSource(HW_POWER_DCDC_LINREG_READY))
		 != SUCCESS)
            return Status;
        if((Status = hw_power_SetVddioPowerSource(HW_POWER_DCDC_LINREG_READY))
		 != SUCCESS)
            return Status;

        // Need to make sure the DCDC does not run with a bad or broken battery.
        if( hw_power_GetBatteryVoltage() <= 2800 )
        {
            hw_power_PowerDown();
        }
        hw_power_ClearBatteryBrownoutInterrupt();
        hw_power_EnableBatteryBrownoutInterrupt(true);
        hw_power_EnableDcdc(true);
    }

    //--------------------------------------------------------------------------
    // Done.
    //--------------------------------------------------------------------------
    return SUCCESS;
}


RtStatus_t hw_power_InitFiq(void)
{
    uint16_t u16SafeBattVolt;
    extern uint8_t g_ddi_power_SafeBatteryVoltageCode;

    //--------------------------------------------------------------------------
    // Clear the brownout interrupts.
    //--------------------------------------------------------------------------

    hw_power_ClearVdddBrownoutInterrupt();
    hw_power_ClearVddaBrownoutInterrupt();
    hw_power_ClearVddioBrownoutInterrupt();

    //--------------------------------------------------------------------------
    // Enable the power supply to assert brownout interrupts.
    //--------------------------------------------------------------------------

    hw_power_EnableVdddBrownoutInterrupt(TRUE);
    hw_power_EnableVddaBrownoutInterrupt(TRUE);
    hw_power_EnableVddioBrownoutInterrupt(TRUE);

    //--------------------------------------------------------------------------
    // Enable the battery brownout interrupt.
    //--------------------------------------------------------------------------

    // Check the battery.  Don't enable the brownout until the battery has at
    // least reached a level where it can do a handoff.
    u16SafeBattVolt = hw_power_ConvertSettingToBattBo(
	g_ddi_power_SafeBatteryVoltageCode );

    // Check if the battery level is high enough for DCDC to operate.
    if( hw_power_GetBatteryVoltage() > u16SafeBattVolt )
    {
        hw_power_ClearBatteryBrownoutInterrupt();
        hw_power_EnableBatteryBrownoutInterrupt( true );
        hw_power_EnableBattBrownoutPowerdown( false );
    }

    return SUCCESS;

}
////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}




