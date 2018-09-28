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
bool hw_lradc_GetChannelPresent(hw_lradc_Channel_t eChannel)
{
    bool bRtn;

    // Read a bit field of HW_LRADC_STATUS register
    switch (eChannel)
    {
        case LRADC_CH0:
            bRtn =  BF_RD(LRADC_STATUS, CHANNEL0_PRESENT);
            break;
        case LRADC_CH1:
            bRtn =  BF_RD(LRADC_STATUS, CHANNEL1_PRESENT);
            break;
        case LRADC_CH2:
            bRtn =  BF_RD(LRADC_STATUS, CHANNEL2_PRESENT);
            break;
        case LRADC_CH3:
            bRtn =  BF_RD(LRADC_STATUS, CHANNEL3_PRESENT);
            break;
        case LRADC_CH4:
            bRtn =  BF_RD(LRADC_STATUS, CHANNEL4_PRESENT);
            break;
        case LRADC_CH5:
            bRtn =  BF_RD(LRADC_STATUS, CHANNEL5_PRESENT);
            break;
        case LRADC_CH6:
            bRtn =  BF_RD(LRADC_STATUS, CHANNEL6_PRESENT);
            break;
        case LRADC_CH7:
            bRtn =  BF_RD(LRADC_STATUS, CHANNEL7_PRESENT);
            break;
        default:
            bRtn = 0;
            break;
    }
    return bRtn;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_ScheduleChannel(hw_lradc_Channel_t eChannel)  // one channel only
{
    // Set the SCHEDULE bitfield of HW_LRADC_CTRL0 register
    BF_SETV(LRADC_CTRL0, SCHEDULE, (1 << eChannel));
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_AssignChannel( hw_lradc_Channel_t eVirtualChannel,
                             hw_lradc_Channel_t ePhysicalChannel )
{
    // Maps the physical channel to virtual channel
    switch ( eVirtualChannel )
    {
        case LRADC_CH0:
            BF_WR( LRADC_CTRL4, LRADC0SELECT, ePhysicalChannel );
            break;
        case LRADC_CH1:
            BF_WR( LRADC_CTRL4, LRADC1SELECT, ePhysicalChannel );
            break;
        case LRADC_CH2:
            BF_WR( LRADC_CTRL4, LRADC2SELECT, ePhysicalChannel );
            break;
        case LRADC_CH3:
            BF_WR( LRADC_CTRL4, LRADC3SELECT, ePhysicalChannel );
            break;
        case LRADC_CH4:
            BF_WR( LRADC_CTRL4, LRADC4SELECT, ePhysicalChannel );
            break;
        case LRADC_CH5:
            BF_WR( LRADC_CTRL4, LRADC5SELECT, ePhysicalChannel );
            break;
        case LRADC_CH6:
            BF_WR( LRADC_CTRL4, LRADC6SELECT, ePhysicalChannel );
            break;
        case LRADC_CH7:
            BF_WR( LRADC_CTRL4, LRADC7SELECT, ePhysicalChannel );
            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableInterrupt(hw_lradc_Channel_t eChannel, bool bValue)
{
    if(bValue)
    {
        // Enable the interrupt of a LRADC channel
        switch (eChannel)
        {
            case LRADC_CH0:
                BF_SET(LRADC_CTRL1, LRADC0_IRQ_EN);
                break;
            case LRADC_CH1:
                BF_SET(LRADC_CTRL1, LRADC1_IRQ_EN);
                break;
            case LRADC_CH2:
                BF_SET(LRADC_CTRL1, LRADC2_IRQ_EN);
                break;
            case LRADC_CH3:
                BF_SET(LRADC_CTRL1, LRADC3_IRQ_EN);
                break;
            case LRADC_CH4:
                BF_SET(LRADC_CTRL1, LRADC4_IRQ_EN);
                break;
            case LRADC_CH5:
                BF_SET(LRADC_CTRL1, LRADC5_IRQ_EN);
                break;
            case LRADC_CH6:
                BF_SET(LRADC_CTRL1, LRADC6_IRQ_EN);
                break;
            case LRADC_CH7:
                BF_SET(LRADC_CTRL1, LRADC7_IRQ_EN);
                break;
            default:
            	break;
        }
    }
    else
    {
        // Disable the interrupt of a LRADC channel
        switch (eChannel)
        {
            case LRADC_CH0:
                BF_CLR(LRADC_CTRL1, LRADC0_IRQ_EN);
                break;
            case LRADC_CH1:
                BF_CLR(LRADC_CTRL1, LRADC1_IRQ_EN);
                break;
            case LRADC_CH2:
                BF_CLR(LRADC_CTRL1, LRADC2_IRQ_EN);
                break;
            case LRADC_CH3:
                BF_CLR(LRADC_CTRL1, LRADC3_IRQ_EN);
                break;
            case LRADC_CH4:
                BF_CLR(LRADC_CTRL1, LRADC4_IRQ_EN);
                break;
            case LRADC_CH5:
                BF_CLR(LRADC_CTRL1, LRADC5_IRQ_EN);
                break;
            case LRADC_CH6:
                BF_CLR(LRADC_CTRL1, LRADC6_IRQ_EN);
                break;
            case LRADC_CH7:
                BF_CLR(LRADC_CTRL1, LRADC7_IRQ_EN);
                break;
            default:
            	break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetInterruptFlag(hw_lradc_Channel_t eChannel)
{
    bool bRtn;

    // Returns LRADC interrupt flag
    switch (eChannel)
    {
        case LRADC_CH0:
            bRtn = BF_RD(LRADC_CTRL1, LRADC0_IRQ);
            break;
        case LRADC_CH1:
            bRtn = BF_RD(LRADC_CTRL1, LRADC1_IRQ);
            break;
        case LRADC_CH2:
            bRtn = BF_RD(LRADC_CTRL1, LRADC2_IRQ);
            break;
        case LRADC_CH3:
            bRtn = BF_RD(LRADC_CTRL1, LRADC3_IRQ);
            break;
        case LRADC_CH4:
            bRtn = BF_RD(LRADC_CTRL1, LRADC4_IRQ);
            break;
        case LRADC_CH5:
            bRtn = BF_RD(LRADC_CTRL1, LRADC5_IRQ);
            break;
        case LRADC_CH6:
            bRtn = BF_RD(LRADC_CTRL1, LRADC6_IRQ);
            break;
        case LRADC_CH7:
            bRtn = BF_RD(LRADC_CTRL1, LRADC7_IRQ);
            break;
        default:
            bRtn = 0;
            break;
    }
    return bRtn;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_ConfigureChannel( hw_lradc_Channel_t  eChannel,
                                bool                bEnableDivideByTwo,
                                bool                bEnableAccum,
                                uint8_t             u8NumSamples)
{
    // Set the analog divide-by two function
    if(bEnableDivideByTwo)
    {
        // Enable the divide-by-two of a LRADC channel
        BF_SETV(LRADC_CTRL2, DIVIDE_BY_TWO, (1 << eChannel));
    }
    else
    {
        // Disable the divide-by-two of a LRADC channel
        BF_CLRV(LRADC_CTRL2, DIVIDE_BY_TWO, (1 << eChannel));
    }

    // Clear the accumulator & NUM_SAMPLES
    HW_LRADC_CHn_CLR(eChannel, 0xFFFFFFFF);

    // Sets NUM_SAMPLES bitfield of HW_LRADC_CHn register.
    BF_WRn(LRADC_CHn, eChannel, NUM_SAMPLES, (u8NumSamples & 0x1f));

    // Set ACCUMULATE bit of HW_LRADC_CHn register
    if(bEnableAccum)
    {
        // Enable the accumulation of a LRADC channel
        BF_SETn(LRADC_CHn, eChannel, ACCUMULATE);
    }
    else
    {
        // Disable the accumulation of a LRADC channel
        BF_CLRn(LRADC_CHn, eChannel, ACCUMULATE);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetToggleFlag(hw_lradc_Channel_t eChannel)
{
    // Return the TOGGLE flag of a LRADC channel
    return ((bool)(BF_RDn(LRADC_CHn, eChannel, TOGGLE)));
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_ClearAccum(hw_lradc_Channel_t eChannel)
{
    // Write zero to VALUE bitfield of HW_LRADC_CHn register
    BF_CLRn(LRADC_CHn, eChannel, VALUE);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_SetDelayTrigger(hw_lradc_DelayTrigger_t   eDelayTrigger,
                              uint32_t                  u32TriggerLradcs,
                              uint32_t                  u32DelayTriggers,
                              uint32_t                  u32LoopCount,
                              uint32_t                  u32DelayCount)
{
    //Set the TRIGGER_LRADCS bitfield of HW_LRADC_DELAYn register
    BF_SETVn(LRADC_DELAYn, eDelayTrigger, TRIGGER_LRADCS, u32TriggerLradcs);

    //Set the TRIGGER_DELAYS bitfield of HW_LRADC_DELAYn register
    BF_SETVn(LRADC_DELAYn, eDelayTrigger, TRIGGER_DELAYS, u32DelayTriggers);

    //Write the LOOP_COUNT bitfield of HW_LRADC_DELAYn register
    BF_WRn(LRADC_DELAYn, eDelayTrigger, LOOP_COUNT, u32LoopCount);

    //Write the DEALY bitfield of HW_LRADC_DELAYn register
    BF_WRn(LRADC_DELAYn, eDelayTrigger, DELAY, u32DelayCount);
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_SetDelayTriggerKick(  hw_lradc_DelayTrigger_t eDelayTrigger,
                                    bool bValue)
{
    if(bValue)
    {
        // Start the delay trigger
        BF_SETn(LRADC_DELAYn, eDelayTrigger, KICK);
    }
    else
    {
        // Stop the delay trigger
        BF_CLRn(LRADC_DELAYn, eDelayTrigger, KICK);
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_lradc_EnableBatteryMeasurement( hw_lradc_DelayTrigger_t eTrigger,
                                              uint16_t u16SamplingInterval)
{
    hw_lradc_Channel_t       eChannel = BATTERY_VOLTAGE_CH;

    //
    // Check if the lradc channel is present in this product
    //
    if( hw_lradc_GetChannelPresent(eChannel) == 0 )
        return (ERROR_HW_LRADC_CH_NOT_PRESENT);

    // Disable the channel interrupt
    hw_lradc_EnableInterrupt(eChannel, FALSE);

    hw_lradc_ClearInterruptFlag(eChannel);

    // Configure the battery conversion register
    BF_WR(LRADC_CONVERSION, SCALE_FACTOR, 2);


    // Enable the automatic update mode of BATT_VALUE field in HW_POWER_MONITOR
    BF_SET(LRADC_CONVERSION, AUTOMATIC);

    hw_lradc_ConfigureChannel(  eChannel,   //Lradc channel
                                    FALSE,      //DIVIDE_BY_TWO
                                    FALSE,      //ACCUMULATE
                                    0);         //NUM_SAMPLES

    // schedule a conversion before the setting up of the delay channel
    // so the user can have a good value right after the function returns
    hw_lradc_ScheduleChannel(eChannel);

    // Setup the trigger loop forever,
    hw_lradc_SetDelayTrigger( eTrigger,         // Trigger Index
                              (1 << eChannel),  // Lradc channels
                              (1 << eTrigger),  // Restart the triggers
                              0,                // No loop count
                              u16SamplingInterval); // 0.5*N msec on 2khz

    // Clear the accumulator & NUM_SAMPLES
    HW_LRADC_CHn_CLR(eChannel, 0xFFFFFFFF);

    // Kick off the LRADC battery measurement
    hw_lradc_SetDelayTriggerKick(eTrigger, TRUE);


    /* Wait for first measurement of battery.  Should occur in 13 LRADC clock
     * cycles from the time of channel kickoff.  Also add some wait time for
     * copy to the power supply BATT_VAL field to occur.
     */
    hw_digctl_MicrosecondWait(10);



    return SUCCESS;
}


void hw_lradc_EnableAutomaticBatteryUpdate(void)
{
    BF_SET(LRADC_CONVERSION, AUTOMATIC);
}

void hw_lradc_DisableAutomaticBatteryUpdate(void)
{
    BF_CLR(LRADC_CONVERSION, AUTOMATIC);
}

hw_lradc_DelayTrigger_t hw_lradc_AcquireBatteryMonitorDelayTrigger(void)
{
#define BATTERY_MONITOR_LRADC_TRIGGER LRADC_DELAY_TRIGGER3

    return BATTERY_MONITOR_LRADC_TRIGGER;
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
