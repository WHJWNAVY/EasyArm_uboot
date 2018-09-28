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
#include "error.h"

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetTouchDetectPresent(void)
{
    // Read the TOUCH_PANEL_PRESENT bit of HW_LRADC_STATUS register
    return (BF_RD(LRADC_STATUS, TOUCH_PANEL_PRESENT));
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTouchDetect(bool bValue)
{
    if(bValue)
    {   //Enable the touch detector
        BF_SET(LRADC_CTRL0, TOUCH_DETECT_ENABLE);
    }
    else
    {   // Disable the touch detector
        BF_CLR(LRADC_CTRL0, TOUCH_DETECT_ENABLE);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTouchDetectInterrupt(bool bValue)
{
    if(bValue)
    {   //Enable the touch detector interrupt
        BF_SET(LRADC_CTRL1, TOUCH_DETECT_IRQ_EN);
    }
    else
    {   // Disable the touch detector interrupt
        BF_CLR(LRADC_CTRL1, TOUCH_DETECT_IRQ_EN);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetTouchDetectInterruptFlag(void)
{
    return ((bool)(BF_RD(LRADC_CTRL1, TOUCH_DETECT_IRQ)));
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_ClearTouchDetectInterruptFlag(void)
{
    BF_CLR(LRADC_CTRL1, TOUCH_DETECT_IRQ);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTouchDetectXDrive(bool bValue)
{
    if(bValue)
    {   //Enable the X-Drive
        BF_SET(LRADC_CTRL0, XMINUS_ENABLE);
        BF_SET(LRADC_CTRL0, XPLUS_ENABLE);
    }
    else
    {   // Disable the X-Drive
        BF_CLR(LRADC_CTRL0, XMINUS_ENABLE);
        BF_CLR(LRADC_CTRL0, XPLUS_ENABLE);
    }

}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTouchDetectYDrive(bool bValue)
{
    if(bValue)
    {   //Enable the Y-Drive
        BF_SET(LRADC_CTRL0, YMINUS_ENABLE);
        BF_SET(LRADC_CTRL0, YPLUS_ENABLE);
    }
    else
    {   // Disable the Y-Drive
        BF_CLR(LRADC_CTRL0, YMINUS_ENABLE);
        BF_CLR(LRADC_CTRL0, YPLUS_ENABLE);
    }

}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetTouchDetect(void)
{
    // Read the TOUCH_DETECT_RAW bit of HW_LRADC_STATUS register
    return ((bool)(BF_RD(LRADC_STATUS, TOUCH_DETECT_RAW)));
}

