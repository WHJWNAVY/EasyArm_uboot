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

#include "registers/regsicoll.h"
#include "registers/regspower.h"

#include "hw/icoll/hw_icoll.h"
#include "hw/core/hw_core.h"
#include "hw/power/hw_power.h"
#include "hw/lradc/hw_lradc.h"

//#include "drivers/icoll/ddi_icoll.h"
#include "drivers/power/ddi_power.h"       // Driver API
#include "drivers/power/ddi_power_errordefs.h"
#include "ddi_power_internal.h"      // Driver internal API

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Globals & Variables
////////////////////////////////////////////////////////////////////////////////
bool bPowerDriverInitialized = false;
bool bBatteryCheckComplete = false;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Initize the power driver
//!
//! \fntype Function
//! This function initializes the power-supply hardware.
//!
//! \param[in]  eLradcDelayTrigger    LRADC delay trigger for battery measurement
//!
//! \return     SUCCESS                         No error
//!             Others                          Error
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t  ddi_power_Init(ddi_power_InitValues_t* pInitValues)
{
    RtStatus_t Status;
    uint32_t u32SampleInterval;
    hw_lradc_DelayTrigger_t eDelayTrigger;

    //--------------------------------------------------------------------------
    // Check if we are initialized already.
    //--------------------------------------------------------------------------
    if( IsPowerDriverInitialized() )
    {
        ddi_power_Save4p2Enabled( pInitValues->bEnable4p2 );

    }

    //--------------------------------------------------------------------------
    // Initialize the power block.
    //--------------------------------------------------------------------------
    // disabling this check as it was bogus (power prep ungates the clock before
    // hw_power_Init is ever called).
    //if( hw_power_GetPowerClkGate() )
    //{
        if((Status = hw_power_Init()) != SUCCESS)
            return Status;
    //}

    //--------------------------------------------------------------------------
    // Initialize the 5V detection.
    //--------------------------------------------------------------------------
    if( Is5vDetectionInitialized() == false )
    {
        Status = ddi_power_Configure5vDetection( pInitValues->eDcdc5vDetection,
                                                 pInitValues->e5vDetection,
                                                 pInitValues->u16Valid5vThreshold );

        if( Status != SUCCESS )
            return Status;
    }


    //--------------------------------------------------------------------------
    // Initialize the 5V IRQ.  (may be overwritten by PMI)
    //--------------------------------------------------------------------------
    if( Is5vIrqInitialized() == false )
    {
/*
        Status = ddi_power_5vIrqInit( );

        if( Status != SUCCESS )
            return Status;
*/
    }

    //--------------------------------------------------------------------------
    // Initialize the battery monitor.
    //--------------------------------------------------------------------------
    if( IsBatteryMonitorInitialized() == false )
    {

        // Acquire the delay trigger assigned for battery monitoring and
        // parse the init structure for the sampling interval.
        eDelayTrigger = hw_lradc_AcquireBatteryMonitorDelayTrigger();
        u32SampleInterval = pInitValues->u32BatterySamplingInterval;

        // Set up the battery monitor and return if unsuccessful.
        if((Status = hw_power_InitBatteryMonitor(eDelayTrigger, u32SampleInterval)) != SUCCESS)
            return Status;
    }


    //----------------------------------------------------------------------
    // Check if we have a battery.
    //----------------------------------------------------------------------
    if( bBatteryCheckComplete == false )
    {
        bool bBattCon;

        bBattCon = ddi_power_IsBatteryConnected();
        ddi_power_BatteryConnected( bBattCon );

        bBatteryCheckComplete = true;
    }


    //----------------------------------------------------------------------
    // Configure the power supplies for their current power source.
    //----------------------------------------------------------------------
    if( IsPowerSupplyInitialized() == false )
    {
        if(pInitValues->bEnable4p2)
        {
            hw_power_Init4p2EnabledPowerSupply();
            if( ( Status = ddi_power_Enable4p2() ) != SUCCESS )
                return Status;
        }
        else
        {
            if( ( Status = hw_power_InitPowerSupplies() ) != SUCCESS)
                return Status;
        }
    }
    else
    {
        ddi_power_Save4p2Enabled( pInitValues->bEnable4p2 );
    }


    //--------------------------------------------------------------------------
	//Disable Power Down to improve EMI.  Can only be used
    //for release builds because it prevents JTAG debugging
    //--------------------------------------------------------------------------
#ifndef DEBUG
    hw_power_EnableHardwarePowerdown( false );
#endif


    //----------------------------------------------------------------------
    // If we are here, the power driver has completed initialization.
    //----------------------------------------------------------------------
    bPowerDriverInitialized = true;
    hw_power_SetPowerRegInitFlag( POWER_REGS_INITIALIZED );

    return SUCCESS;

}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Determines if the power driver has already been initialized.
//!
//! The power driver should only be initialized once.  If it has been initialized
//! in the current application, the initialized flag will be set.  If it has
//! been initialized by an outside application (i.e. PowerPrep), then we need
//! to check some of the power registers.
//!
//! \retval true Power driver was previously initialized.
//! \retval false Power driver has not yet been initialized.
//!
////////////////////////////////////////////////////////////////////////////////
bool IsPowerDriverInitialized( void )
{
    // Check if current application has initialized the driver.
    if( bPowerDriverInitialized == true )
    {
        return true;
    }

    //--------------------------------------------------------------------------
    // Check if another application initialized the driver.  Need to check
    // three things.
    // 1. Has power block been ungated?  If not, we are not initialized.
    // 2. Have the linreg offsets been changed?  The reset value is
    //    not a good setting and we will need to change it.
    // 3. Has the hardware register flag been set?  We use the speed sensor
    //    control to leave us a message.
    //--------------------------------------------------------------------------

    if( hw_power_GetPowerClkGate() == 1)
    {
        // Power registers have not been ungated.
        return false;
    }

    if( hw_power_GetVdddLinRegOffset() == HW_POWER_LINREG_OFFSET_STEP_ABOVE ||
        hw_power_GetVddaLinRegOffset() == HW_POWER_LINREG_OFFSET_STEP_ABOVE ||
        hw_power_GetVddioLinRegOffset() == HW_POWER_LINREG_OFFSET_STEP_ABOVE )
    {
        // At least one of the linear regulators is set to one step above DCDC.
        // This is not a setting we use, but it is the reset value.
        return false;
    }

    if( hw_power_GetPowerRegInitFlag() == POWER_REGS_UNINITIALIZED )
    {
        // The speed register has not changed state.  This could mean
        // the power driver has not been initialized.
        return false;
    }



    // If the previous tests are all false, then we can assume the
    // driver has been intialized.
    return true;



}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Returns status of the hardware power driver initialization.
//!
//! \fntype Function
//!
//! \retval True Hardware power driver has been initialized.
//! \retval False Hardware power driver has not been initialized.
/////////////////////////////////////////////////////////////////////////////////
bool IsHwPowerInitialized( void )
{
    // Have power registers been ungated?
    if( hw_power_GetPowerClkGate() == 1)
        return false;

    // Have the loop control bits been set?
    if( HW_POWER_LOOPCTRL.B.EN_CM_HYST == 0 || HW_POWER_LOOPCTRL.B.EN_DF_HYST == 0 )
        return false;

    // Check the master control flags.
    if( bMasterFlagsInitialized == false )
        return false;

    return true;

}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Determines if battery monitor has been initialized
//!
//! Checks hardware register status to determine if the battery monitor has
//! been initialized.
//!
//! \retval true Battery monitor has been initialized.
//! \retval false Power driver has not been initialized.
//!
////////////////////////////////////////////////////////////////////////////////
bool IsBatteryMonitorInitialized( void )
{
    // Check the DCDC automatic battery update bit.  If this has been enabled,
    // then the LRADC battery update has probably been started.
    if( HW_POWER_BATTMONITOR.B.EN_BATADJ )
        return true;
    else
        return false;

    // More checks can be added if necessary...
    // Ex. Check if LRADC automatic battery voltage update has been setup.
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Determines if the power supply has been initialzied.
//!
//! The power supply should only be initialized once.  This function checks
//! the status of the power supply to determine if it has been enabled.
//!
//! \retval true Power supply has been initialized.
//! \retval false Power driver has not been initialized.
//!
////////////////////////////////////////////////////////////////////////////////
bool IsPowerSupplyInitialized( void )
{
    // We only need to check the 378x power supply for now...
    if( HW_POWER_DCDC4P2.B.ENABLE_4P2 )
        return true;
    else
        return false;

}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Returns status of the 5V initialization.
//!
//! \fntype Function
//!
//! \retval True 5V detection has been initialized.
//! \retval False 5V detection has not been initialized.
/////////////////////////////////////////////////////////////////////////////////
bool Is5vDetectionInitialized( void )
{
    if( hw_power_GetDetectionMethod() == HW_POWER_5V_UNINITIALIZED )
    {
        return false;
    }

    return true;
}

bool Is5vIrqInitialized( void )
{
    return false;
}



////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}






