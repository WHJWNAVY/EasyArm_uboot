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
RtStatus_t hw_lradc_InitLadder(hw_lradc_Channel_t       eChannel,
                                hw_lradc_DelayTrigger_t eTrigger,
                                uint16_t                u16SamplingInterval)
{
    RtStatus_t    Rtn = SUCCESS;

    //
    // Check if the lradc channel is present in this product
    //
    if( hw_lradc_GetChannelPresent(eChannel) == 0 )
        return (ERROR_HW_LRADC_CH_NOT_PRESENT);

    if ( (BF_RD(LRADC_CTRL0, SFTRST) != 0) ||
		(BF_RD(LRADC_CTRL0, CLKGATE) != 0) )
    {
        // Clean On-chip ground ref and set LRADC clock
        hw_lradc_Init(FALSE, LRADC_CLOCK_6MHZ);
    }

    // Disable the channel interrupt
    hw_lradc_EnableInterrupt(eChannel, FALSE);

    // Configure the specified lradc channel
    hw_lradc_ConfigureChannel(  eChannel,   //Lradc channel
                                TRUE,       //DIVIDE_BY_TWO
                                FALSE,      //ACCUMULATE
                                0);         //NUM_SAMPLES

    // Setup the trigger loop forever,
    hw_lradc_SetDelayTrigger( eTrigger,          // Trigger Index
                              (1 << eChannel),  // Lradc channels
                              (1 << eTrigger),  // Restart the triggers
                              0,                // No loop count
                              u16SamplingInterval); // 0.5*N msec on 2khz

    // Clear the accumulator & NUM_SAMPLES
    HW_LRADC_CHn_CLR(eChannel, 0xFFFFFFFF);

    return Rtn;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_lradc_MeasureVddio(void)
{
    // Clear the Soft Reset for normal operation
    BF_CLR(LRADC_CTRL0, SFTRST);

    // Clear the Clock Gate for normal operation
    BF_CLR(LRADC_CTRL0, CLKGATE);

     // Clear the divide by two for channel 6 since it has a HW divide-by-two
     // built in.
     BF_CLRV(LRADC_CTRL2, DIVIDE_BY_TWO, 1 << VDDIO_VOLTAGE_CH);

     // Clear the accumulator & NUM_SAMPLES
     HW_LRADC_CHn_CLR(VDDIO_VOLTAGE_CH, 0xFFFFFFFF);

     // Get VddIO; this is the max scale value for the button resistor ladder.
     // schedule ch 6:
     BF_WR( LRADC_CTRL0, SCHEDULE, (1 << VDDIO_VOLTAGE_CH) );

     // wait for completion
     while (BF_RD( LRADC_CTRL1, LRADC6_IRQ ) !=
	BV_LRADC_CTRL1_LRADC6_IRQ__PENDING)
     {
	 // Wait for channel 6 completion.
     }

     // Clear the interrupt flag
     BF_CLR(LRADC_CTRL1, LRADC6_IRQ);

     // read ch 6 value.
    return BF_RDn( LRADC_CHn, VDDIO_VOLTAGE_CH, VALUE);
}
