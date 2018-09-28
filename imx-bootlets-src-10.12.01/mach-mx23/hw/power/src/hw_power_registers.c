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
#include "hw/digctl/hw_digctl.h"
#include "registers/regspower.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
bool bEnableMaster5vBrownout = true;
bool bEnableMasterBattBrownout = true;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetPowerClkGate(bool bGate)
{
    // Gate/Ungate the clock to the power block
    if(bGate)
    {
        BF_SET(POWER_CTRL, CLKGATE);
    }
    else
    {
        BF_CLR(POWER_CTRL, CLKGATE);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetPowerClkGate(void)
{
    return HW_POWER_CTRL.B.CLKGATE;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVbusValidThresh(hw_power_VbusValidThresh_t eThresh)
{
    BF_WR(POWER_5VCTRL, VBUSVALID_TRSH, eThresh);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
hw_power_VbusValidThresh_t hw_power_GetVbusValidThresh( void )
{
    return (hw_power_VbusValidThresh_t) BF_RD(POWER_5VCTRL, VBUSVALID_TRSH);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////


void hw_power_DisableBrownoutPowerdown(void)
{
    hw_power_EnableBattBrownoutPowerdown( false );
}

void hw_power_Disable5vBrownoutPowerdown(void)
{
    hw_power_Enable5vBrownoutPowerdown( false );
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables/disables the hardware's ability to powerdown the chip because
//! of a 5V brownout.
//!
//! \fntype Function
//!
//! \param[in] bEnable True to enable the hardware's ability to powerdown,
//! false to disable the ability.
//!
//! \retval Current state of hardware's ability before writing new state.
//!
//! \note PWD_OFF must also be cleared for a 5V brownout to powerdown the chip.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_Enable5vBrownoutPowerdown( bool bEnable )
{
    bool bPrevious = HW_POWER_5VCTRL.B.PWDN_5VBRNOUT;

    // Update the 5V brownout bit.
    if( bEnable )
        BF_SET(POWER_5VCTRL, PWDN_5VBRNOUT);
    else
        BF_CLR(POWER_5VCTRL, PWDN_5VBRNOUT);

    // Now update the master brownout bit.
    hw_power_EnableMaster5vBrownout( bEnable );

    return bPrevious;
}

#pragma ghs section text=".static.text"
////////////////////////////////////////////////////////////////////////////////
//! \brief Enables/disables the hardware's ability to powerdown the chip because
//! of a battery brownout.
//!
//! \fntype Function
//!
//! \param[in] bEnable True to enable the hardware's ability to powerdown,
//! false to disable the ability.
//!
//! \retval Current state of hardware's ability before writing new state.
//!
//! \note PWD_OFF must also be cleared for a battery brownout to powerdown.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableBattBrownoutPowerdown( bool bEnable )
{
    bool bPrevious = HW_POWER_BATTMONITOR.B.PWDN_BATTBRNOUT;

    // Update the battery bit.
    if( bEnable )
        BF_SET(POWER_BATTMONITOR, PWDN_BATTBRNOUT);
    else
        BF_CLR(POWER_BATTMONITOR, PWDN_BATTBRNOUT);

    // Now update the master brownout bit.
    hw_power_EnableMasterBattBrownout( bEnable );

    return bPrevious;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables/disables the hardware's ability to powerdown the chip for
//! any reason.
//!
//! \fntype Function
//!
//! Controls the bit that allows hardware to powerdown the chip.  For debugger
//! builds, we can never disable the hardware powerdown beacuse it also disables
//! the debugger reset functionality.

//!
//! \param[in] bEnable True to enable the hardware's ability to powerdown,
//! false to disable the ability. For debugger builds, this param is ignored and
//! the hardware is always enabled.
//!
//! \retval Current state of hardware's ability before writing new state.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableHardwarePowerdown( bool bEnable )
{
    bool bPrevious = HW_POWER_RESET.B.PWD_OFF;

#ifdef DEBUG
    // For debugger builds, we need to always allow hardware to shut down
    // the chip so the debugger will be able to reset.
    bEnable = true;
#endif

    if( bEnable )
    {
        // Just key so PWD_OFF will be written with zero.  When
        // PWD_OFF=0, the powerdown circuitry is _not_ disabled.
        // The hardware can powerdown the chip.
        HW_POWER_RESET_WR(POWERDOWN_KEY << 16);
    }
    else
    {
        // Key and mask for PWD_OFF to set PWD_OFF.  When PWD_OFF=1
        // the powerdown circuitry is disabled.  The hardware cannot
        // powerdown the chip until PWD_OFF bit is cleared.
        HW_POWER_RESET_WR((POWERDOWN_KEY << 16) | BM_POWER_RESET_PWD_OFF);
    }

    return bPrevious;

}
#pragma ghs section text=default

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableDcdc(bool bEnable)
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
		if(HW_POWER_5VCTRL.B.ENABLE_DCDC==1)
			return;

		/* recording orignal values that will be modified
		* temporarlily to handle a chip bug.  See chip errata
		* for CQ ENGR00115837
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

		/* DCDC_XFER is necessary when first powering on the DCDC
		 * regardless if the source is 4p2 or battery
		 */
		HW_POWER_5VCTRL_SET(BM_POWER_5VCTRL_DCDC_XFER);

		hw_digctl_MicrosecondWait( 20 );

		HW_POWER_5VCTRL_CLR(BM_POWER_5VCTRL_DCDC_XFER);

	        HW_POWER_CTRL_CLR(BM_POWER_CTRL_VBUSVALID_IRQ);

	        BF_SET(POWER_5VCTRL, ENABLE_DCDC);

		// Allow settling time for the DC-DC.
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
		hw_power_ClearVbusValidInterrupt();
		hw_power_Enable5vBrownoutPowerdown( bPrev5vBoPwrdn );
		hw_power_ClearVdd5vDroopInterrupt();
		hw_power_EnableVdd5vDroopInterrupt( bPrev5vDroop );
	}
	else
		BF_CLR(POWER_5VCTRL, ENABLE_DCDC);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetEnableDcdc(void)
{
    return HW_POWER_5VCTRL.B.ENABLE_DCDC;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableAutoDcdcTransfer(bool bEnable)
{
    if(bEnable)
    {
        BF_SET(POWER_5VCTRL, DCDC_XFER);
    }
    else
    {
        BF_CLR(POWER_5VCTRL, DCDC_XFER);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVdddLinRegOffset(hw_power_LinRegOffsetStep_t eOffset)
{
    BF_WR(POWER_VDDDCTRL,LINREG_OFFSET,eOffset);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddaLinRegOffset(hw_power_LinRegOffsetStep_t eOffset)
{
    BF_WR(POWER_VDDACTRL,LINREG_OFFSET,eOffset);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddioLinRegOffset(hw_power_LinRegOffsetStep_t eOffset)
{
    BF_WR(POWER_VDDIOCTRL,LINREG_OFFSET,eOffset);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
hw_power_LinRegOffsetStep_t hw_power_GetVdddLinRegOffset(void)
{
    return (hw_power_LinRegOffsetStep_t) HW_POWER_VDDDCTRL.B.LINREG_OFFSET;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
hw_power_LinRegOffsetStep_t hw_power_GetVddaLinRegOffset(void)
{
    return (hw_power_LinRegOffsetStep_t) HW_POWER_VDDACTRL.B.LINREG_OFFSET;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
hw_power_LinRegOffsetStep_t hw_power_GetVddioLinRegOffset(void)
{
    return (hw_power_LinRegOffsetStep_t) HW_POWER_VDDIOCTRL.B.LINREG_OFFSET;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableRcScale(hw_power_RcScaleLevels_t eLevel)
{
    BF_WR(POWER_LOOPCTRL, EN_RCSCALE, eLevel);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
#pragma ghs section text=".static.text"
void hw_power_PowerDown(void)
{
    //--------------------------------------------------------------------------
    // Make sure the power down bit is not disabled.  Just key so PWD_OFF
    // will be written with zero.
    //--------------------------------------------------------------------------
    HW_POWER_RESET_WR(POWERDOWN_KEY << 16);

    //--------------------------------------------------------------------------
    // Set the PWD bit to shut off the power.
    //--------------------------------------------------------------------------
    HW_POWER_RESET_WR((POWERDOWN_KEY << 16) | BM_POWER_RESET_PWD);
}
#pragma ghs section text=default
////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisablePowerDown(bool bDisable)
{
    hw_power_EnableHardwarePowerdown( !bDisable );
}


////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetPosLimitBuck(uint16_t u16Limit)
{
    BF_WR(POWER_DCLIMITS, POSLIMIT_BUCK, u16Limit);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetNegLimit(uint16_t u16Limit)
{
    BF_WR(POWER_DCLIMITS, NEGLIMIT, u16Limit);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableDoubleFets(bool bEnable)
{
    if(bEnable)
    {
        BF_SET(POWER_MINPWR, DOUBLE_FETS);
    }
    else
    {
        BF_CLR(POWER_MINPWR, DOUBLE_FETS);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableHalfFets(bool bEnable)
{
    if(bEnable)
    {
        BF_SET(POWER_MINPWR, HALF_FETS);
    }
    else
    {
        BF_CLR(POWER_MINPWR, HALF_FETS);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetLoopCtrlDcC(uint16_t u16Value)
{
    BF_WR(POWER_LOOPCTRL, DC_C, u16Value);
}

////////////////////////////////////////////////////////////////////////////////
//
//! \brief Sets the register value for the new DCDC clock frequency.
//!
//! \fntype Function
//!
//! This function will convert the requested frequency to a register setting and
//! then write those values to the register.  If an invalid frequency is input,
//! the function will use a default value of 24MHz.
//!
//! \param[in] u16Freq New DCDC frequency in kHz
//! \param[in] bUsePll TRUE if PLL is to be used as clock source. FALSE if
//! crystal will be used.
//!
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetDcdcClkFreq(uint16_t u16Freq, bool bUsePll)
{
    uint16_t Setting;

    //--------------------------------------------------------------------------
    // Convert the decimal frequency value to the register setting.  The default
    // value will be 24MHz.
    //--------------------------------------------------------------------------
    if(u16Freq == 19200)
    {
        Setting = 0x3;
    }
    else if(u16Freq == 20000)
    {
        Setting = 0x1;
    }
    else
    {
        Setting = 0x2;
    }

    //--------------------------------------------------------------------------
    // Set the new PLL frequency first even if we are using the XTAL.  This will
    // synchronize the frequencies.  Then use the bUsePll variable to set the
    // SEL_PLLCLK field.  A TRUE value will set a one in the register field.
    //--------------------------------------------------------------------------
    BF_WR(POWER_MISC, FREQSEL, Setting);
    BF_WR(POWER_MISC, SEL_PLLCLK, bUsePll);

}



void hw_power_SetSpeedSensorControl( uint16_t u16Ctrl )
{
    HW_POWER_SPEED.B.CTRL = u16Ctrl;
}

uint16_t hw_power_GetSpeedSensorControl( void )
{
    return HW_POWER_SPEED.B.CTRL;
}

void hw_power_SetPowerRegInitFlag( uint16_t u16Flag )
{
    if( u16Flag == POWER_REGS_INITIALIZED)
    {
        hw_power_SetSpeedSensorControl( POWER_REGS_INITIALIZED );
    }
}

uint16_t hw_power_GetPowerRegInitFlag( void )
{
    return hw_power_GetSpeedSensorControl();
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables/disables the master brownout bit for a 5V brownout.
//!
//! \fntype Function
//!
//! The master 5V brownout bit must also be set to allow automatic hardware
//! shutdowns on 5V brownouts.  This functions configures that bit.
//!
//! \param[in] bEnable True to enable automatic shutdown on 5V brownout, false
//! to disable.
//!
//! \retval bool Previous state of the master brownout bit.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableMaster5vBrownout( bool bEnable )
{
    // Save previous state so we can return to caller.
    // Note: The bit is "powerdown off" so we will invert the value so we can
    // use it as an enable bit.
    bool bPrev;
    bPrev = !(HW_POWER_RESET.B.PWD_OFF);

    bEnableMaster5vBrownout = bEnable;
    hw_power_ConfigureMasterBrownout();

    return bPrev;
}


#pragma ghs section text=".static.text"
////////////////////////////////////////////////////////////////////////////////
//! \brief Enables/disables the master brownout bit for a battery brownout.
//!
//! \fntype Function
//!
//! The master battery brownout bit must also be set to allow automatic
//! hardware shutdowns on battery brownouts. This functions configures that bit.
//!
//! \param[in] bEnable True to enable automatic shutdown on battery brownout,
//! false to disable.
//!
//! \retval bool Previous state of the master brownout bit.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableMasterBattBrownout( bool bEnable )
{
    // Save previous state so we can return to caller.
    // Note: The bit is "powerdown off" so we will invert the value so we can
    // use it as an enable bit.
    bool bPrev;
    bPrev = !(HW_POWER_RESET.B.PWD_OFF);

    bEnableMasterBattBrownout = bEnable;
    hw_power_ConfigureMasterBrownout();

    return bPrev;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Configures the master brownout bit.
//!
//! \fntype Function
//!
//! The master brownout controls all hardware powerdowns.  We want to disable
//! this as often as possible because that will improve ESD tolerance.  We will
//! only set the bit if either battery or 5V brownout needs it.
//!
//! \retval New state of the master brownout bit.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_ConfigureMasterBrownout( void )
{
    // If either is true, we must enable the master switch because at least
    // one needs to turn on.  We only turn off the master if both are disabled.
    if( bEnableMaster5vBrownout || bEnableMasterBattBrownout )
        hw_power_EnableHardwarePowerdown( true );
    else
        hw_power_EnableHardwarePowerdown( false );

    return HW_POWER_RESET.B.PWD_OFF;
}
#pragma ghs section text=default

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVbusDroopThresh( hw_power_VbusDroopThresh_t eThresh )
{
    BF_WR(POWER_5VCTRL, VBUSDROOP_TRSH, eThresh);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
hw_power_VbusDroopThresh_t hw_power_GetVbusDroopThresh( void )
{
    return (hw_power_VbusDroopThresh_t) BF_RD(POWER_5VCTRL, VBUSDROOP_TRSH);
}
////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}


