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
#include "hw/power/hw_power.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_ClearInterruptFlag(hw_lradc_Channel_t eChannel)
{
    // Clears a LRADC channel interrupt flag
    switch (eChannel)
    {
        case LRADC_CH0:
            BF_CLR(LRADC_CTRL1, LRADC0_IRQ);
            break;
        case LRADC_CH1:
            BF_CLR(LRADC_CTRL1, LRADC1_IRQ);
            break;
        case LRADC_CH2:
            BF_CLR(LRADC_CTRL1, LRADC2_IRQ);
            break;
        case LRADC_CH3:
            BF_CLR(LRADC_CTRL1, LRADC3_IRQ);
            break;
        case LRADC_CH4:
            BF_CLR(LRADC_CTRL1, LRADC4_IRQ);
            break;
        case LRADC_CH5:
            BF_CLR(LRADC_CTRL1, LRADC5_IRQ);
            break;
        case LRADC_CH6:
            BF_CLR(LRADC_CTRL1, LRADC6_IRQ);
            break;
        case LRADC_CH7:
            BF_CLR(LRADC_CTRL1, LRADC7_IRQ);
            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_lradc_GetAccumValue(hw_lradc_Channel_t eChannel)
{
    // Read the accumulator value of the channel
    return (uint16_t)(BF_RDn(LRADC_CHn, eChannel, VALUE));
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
