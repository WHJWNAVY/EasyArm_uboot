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
// Variables
////////////////////////////////////////////////////////////////////////////////
static hw_power_5vDetection_t DetectionMethod = HW_POWER_5V_UNINITIALIZED;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_Enable5vDetection(hw_power_5vDetection_t eDetection)
{
    bool bPrevPowerDownStatus = false;
    bool bPrev5vDroopStatus   = false;

    //--------------------------------------------------------------------------
    // Set the detection method for all the 5V detection calls
    //--------------------------------------------------------------------------
    hw_power_SetDetectionMethod( eDetection );


    //--------------------------------------------------------------------------
    // Disable hardware 5V removal powerdown because setting VBUSVALID can
    // cause a false brownout.
    //--------------------------------------------------------------------------
    bPrevPowerDownStatus = hw_power_Enable5vBrownoutPowerdown( false );
    bPrev5vDroopStatus   = hw_power_EnableVdd5vDroopInterrupt( false );

    //--------------------------------------------------------------------------
    // Activate the VBUS detection method.
    //--------------------------------------------------------------------------
    BF_SET(POWER_5VCTRL, PWRUP_VBUS_CMPS);
    BF_SET(POWER_5VCTRL, VBUSVALID_5VDETECT);


    //------------------------------------------------------------------
    // Set 5V detection threshold to normal level for VBUSVALID.
    //------------------------------------------------------------------
// For TA1 silicon, the VBUSVALID threshold levels aren't matching up with
// what is specified in the datasheet.  The value of 6 appears to be more
// like ~4.25V.  We'll use it for now.
    hw_power_SetVbusValidThresh((hw_power_VbusValidThresh_t)1);

    //--------------------------------------------------------------------------
    // Enable hardware 5V removal powerdown.  It will be disabled when the
    // handoff is initialized.
    //--------------------------------------------------------------------------
    hw_power_Enable5vBrownoutPowerdown( bPrevPowerDownStatus );
    hw_power_ClearVdd5vDroopInterrupt();
    hw_power_EnableVdd5vDroopInterrupt( bPrev5vDroopStatus );

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetDcdc5vDetect( hw_power_5vDetection_t eDetection )
{
    //--------------------------------------------------------------------------
    // Set the DCDC to the specified 5V detection method.
    // Note: The VBUSVALID method should always be used for DCDC
    // The switch statement is used to allow for future expansion.
    //--------------------------------------------------------------------------
    switch( eDetection )
    {
        //----------------------------------------------------------------------
        // Allow to fall through to VBUSVALID detection.
        //----------------------------------------------------------------------
        case HW_POWER_5V_VDD5V_GT_VDDIO:

        /*
            // Enable VDD5V_GT_VDDIO hardware
            hw_power_EnableVdd5vGtVddio5vDetect( true );

            // Use VDD5V_GT_VDDIO detection method for DCDC.
            BF_CLR( POWER_5VCTRL, VBUSVALID_5VDETECT );
            break;
        */

        case HW_POWER_5V_VBUSVALID_WITH_POLLING:
        case HW_POWER_5V_VBUSVALID:
        default:

            // Enable VBUSVALID hardware
            hw_power_EnableVbusValid5vDetect( true );

            // Use VBUSVALID detection method for DCDC.
            BF_SET( POWER_5VCTRL, VBUSVALID_5VDETECT );
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetSw5vDetect( hw_power_5vDetection_t eDetection )
{

    //--------------------------------------------------------------------------
    // Turn on any hardware devices need for 5V detection.
    //--------------------------------------------------------------------------
    switch( eDetection )
    {
        case HW_POWER_5V_VDD5V_GT_VDDIO:
            /* nothing to do */
            break;


        case HW_POWER_5V_VBUSVALID_WITH_POLLING:
        case HW_POWER_5V_VBUSVALID:
        default:
            hw_power_EnableVbusValid5vDetect( true );
            break;
    }

    //--------------------------------------------------------------------------
    // Save the detection method.  The driver will use this value to determine
    // which 5V detection bitfields to access.
    //--------------------------------------------------------------------------
    hw_power_SetDetectionMethod( eDetection );
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableVbusValid5vDetect( bool bEnable )
{
    //--------------------------------------------------------------------------
    // Need to turn on the VBUSVALID comparators.
    //--------------------------------------------------------------------------
    if( bEnable )
    {
        BF_SET(POWER_5VCTRL, PWRUP_VBUS_CMPS);
    }
    else
    {
        BF_CLR(POWER_5VCTRL, PWRUP_VBUS_CMPS);
    }

}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableUsb5vDetect( bool bEnable )
{
    // The USB 5V detect uses VBUSVALID.
    hw_power_EnableVbusValid5vDetect( bEnable );
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetDetectionMethod( hw_power_5vDetection_t eDetection )
{
    DetectionMethod = eDetection;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
hw_power_5vDetection_t hw_power_GetDetectionMethod( void )
{
    return DetectionMethod;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_Get5vPresentFlag(void)
{
    return hw_power_Is5vPresent();
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_Is5vPresent( void )
{
    bool b5vStatus = false;

    switch( DetectionMethod )
    {
        case HW_POWER_5V_VDD5V_GT_VDDIO:
            b5vStatus = hw_power_GetVdd5vGtVddio();
            break;

        case HW_POWER_5V_VBUSVALID:
        case HW_POWER_5V_VBUSVALID_WITH_POLLING:
        default:
            b5vStatus = hw_power_GetVbusValid();
            break;
    }

    return b5vStatus;
}


////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetVbusValid(void)
{
    //todo
    return 1;//HW_POWER_STS.B.VBUSVALID_STATUS;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetVdd5vGtVddio(void)
{
    return HW_POWER_STS.B.VDD5V_GT_VDDIO;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_IsUsbVbusPresent(void)
{
    // This status bit does not necessarily reflect 5V presence!
    return 1;// HW_POWER_STS.B.VBUSVALID;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_Enable5vPlugInDetect(bool bEnable)
{
    switch(DetectionMethod)
    {
        case HW_POWER_5V_VBUSVALID:
        case HW_POWER_5V_VBUSVALID_WITH_POLLING:

            //------------------------------------------------------------------
            // Set the VBUSVALID interrupt polarity.
            //------------------------------------------------------------------

            if(bEnable)
            {
                hw_power_SetVbusValidInterruptPolarity(
			HW_POWER_CHECK_5V_CONNECTED);
            }
            else
            {
                hw_power_SetVbusValidInterruptPolarity(
			HW_POWER_CHECK_5V_DISCONNECTED);
            }
            break;


        case HW_POWER_5V_VDD5V_GT_VDDIO:

            //------------------------------------------------------------------
            // Set the VDD5V_GT_VDDIO interrupt polarity.
            //------------------------------------------------------------------

            if(bEnable)
            {
                hw_power_SetVdd5vGtVddioInterruptPolarity(
			HW_POWER_CHECK_5V_CONNECTED);
            }
            else
            {
                hw_power_SetVdd5vGtVddioInterruptPolarity(
			HW_POWER_CHECK_5V_DISCONNECTED);
            }

            break;


        default:

            //------------------------------------------------------------------
            // Not a valid state.
            //------------------------------------------------------------------
            break;
    }

}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_Enable5vUnplugDetect(bool bEnable)
{
    switch(DetectionMethod)
    {
        case HW_POWER_5V_VBUSVALID:
        case HW_POWER_5V_VBUSVALID_WITH_POLLING:

            //------------------------------------------------------------------
            // Set the VBUSVALID interrupt polarity.
            //------------------------------------------------------------------

            if(bEnable)
            {
                hw_power_SetVbusValidInterruptPolarity(
			HW_POWER_CHECK_5V_DISCONNECTED);
            }
            else
            {
                hw_power_SetVbusValidInterruptPolarity(
			HW_POWER_CHECK_5V_CONNECTED);
            }
            break;



        case HW_POWER_5V_VDD5V_GT_VDDIO:

            //------------------------------------------------------------------
            // Set the VDD5V_GT_VDDIO interrupt polarity.
            //------------------------------------------------------------------

            if(bEnable)
            {
                hw_power_SetVdd5vGtVddioInterruptPolarity(
			HW_POWER_CHECK_5V_DISCONNECTED);
            }
            else
            {
                hw_power_SetVdd5vGtVddioInterruptPolarity(
			HW_POWER_CHECK_5V_CONNECTED);
            }
            break;


        default:

            //------------------------------------------------------------------
            // Not a valid state.
            //------------------------------------------------------------------
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_Get5vInterruptPolarity(void)
{
    switch(DetectionMethod)
    {
        case HW_POWER_5V_VBUSVALID:
        case HW_POWER_5V_VBUSVALID_WITH_POLLING:
        {
            return HW_POWER_CTRL.B.POLARITY_VBUSVALID;
        }

        case HW_POWER_5V_VDD5V_GT_VDDIO:
        {
            return HW_POWER_CTRL.B.POLARITY_VDD5V_GT_VDDIO;
        }

        default:
        {
            return false;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_Enable5vInterrupt(bool bEnable)
{
    switch(DetectionMethod)
    {
        case HW_POWER_5V_VBUSVALID:

            if(bEnable)
            {
                //--------------------------------------------------------------
                // Enable VBUSVALID for 5V detection.
                //--------------------------------------------------------------
                hw_power_EnableVbusValidInterrupt(TRUE);
                hw_power_EnableVdd5vGtVddioInterrupt(FALSE);

            }
            else
            {
                //--------------------------------------------------------------
                // Disable VBUSVALID 5V detection.
                //--------------------------------------------------------------
                hw_power_EnableVbusValidInterrupt(FALSE);
            }

            break;


        case HW_POWER_5V_VBUSVALID_WITH_POLLING:

            //------------------------------------------------------------------
            // Don't enable the VBUSVALID interrupt in this case.  The
            // application will depend on a timer that polls.
            //------------------------------------------------------------------
            hw_power_EnableVbusValidInterrupt(false);
            hw_power_EnableVdd5vGtVddioInterrupt(false);

            break;


        case HW_POWER_5V_VDD5V_GT_VDDIO:

            if(bEnable)
            {
                //--------------------------------------------------------------
                // Enable VDD5V_GT_VDDIO for 5V detection.
                //--------------------------------------------------------------
                hw_power_EnableVbusValidInterrupt(FALSE);
                hw_power_EnableVdd5vGtVddioInterrupt(TRUE);

            }
            else
            {
                //--------------------------------------------------------------
                // Disable VDD5V_GT_VDDIO for 5V detection.
                //--------------------------------------------------------------
                hw_power_EnableVdd5vGtVddioInterrupt(FALSE);
            }
            break;

        default:

            //------------------------------------------------------------------
            // Not a valid state.
            //------------------------------------------------------------------
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_power.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_power_Clear5vInterrupt(void)
{
    switch(DetectionMethod)
    {
        case HW_POWER_5V_VBUSVALID:
        case HW_POWER_5V_VBUSVALID_WITH_POLLING:

            //------------------------------------------------------------------
            // Clear the VBUSVALID interrupt.
            //------------------------------------------------------------------
            hw_power_ClearVbusValidInterrupt();

            break;

        case HW_POWER_5V_VDD5V_GT_VDDIO:

            //------------------------------------------------------------------
            // Clear the VDD5V_GT_VDDIO interrupt.
            //------------------------------------------------------------------
            hw_power_ClearVdd5vGtVddioInterrupt();

            break;

        default:

            //------------------------------------------------------------------
            // Not a valid state.
            //------------------------------------------------------------------
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}


