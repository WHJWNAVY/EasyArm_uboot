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
#include "hw/digctl/hw_digctl.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define TEMP_SENSOR_CURRENT_RAMP_DELAY 300
#define NUM_TEMP_READINGS_TO_AVG 5
#define TEMP_SENSOR_CONVERSION_CONSTANT 1104

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetTempSensorPresent(hw_lradc_TempSensor_t eSensor)
{
    bool bRtn;
    // Read a bit field of HW_LRADC_STATUS register
    switch (eSensor)
    {
        case TEMP_SENSOR0:
            bRtn  =  BF_RD(LRADC_STATUS, TEMP0_PRESENT);
            break;
        case TEMP_SENSOR1:
            bRtn =  BF_RD(LRADC_STATUS, TEMP1_PRESENT);
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
void hw_lradc_SetTempSensorCurrentValue(hw_lradc_TempSensor_t       eSensor,
                                        hw_lradc_CurrentMagnitude_t eValue)
{
    switch (eSensor)
    {
        case TEMP_SENSOR0:
            // Set the sensor current value
            BF_WR(LRADC_CTRL2, TEMP_ISRC0, eValue);
            break;
        case TEMP_SENSOR1:
            // Set the sensor current value
            BF_WR(LRADC_CTRL2, TEMP_ISRC1, eValue);
            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTempSensorCurrent(hw_lradc_TempSensor_t  eSensor,
                                      bool                   bValue)
{
    switch (eSensor)
    {
        case TEMP_SENSOR0:
            if(bValue)
            {// Enable the sensor current source
                BF_SET(LRADC_CTRL2, TEMP_SENSOR_IENABLE0);
            }
            else
            {// Disable the sensor current source
                BF_CLR(LRADC_CTRL2, TEMP_SENSOR_IENABLE0);
            }
            break;
        case TEMP_SENSOR1:
            if(bValue)
            {// Enable the sensor current source
                BF_SET(LRADC_CTRL2, TEMP_SENSOR_IENABLE1);
            }
            else
            {// Disable the sensor current source
                BF_CLR(LRADC_CTRL2, TEMP_SENSOR_IENABLE1);
            }
            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_lradc_MeasureInternalDieTemperature(hw_lradc_Channel_t eChannel0,
                                                hw_lradc_Channel_t eChannel1)
{

    uint16_t  ch8Value, ch9Value;

    // Clear the Soft Reset for normal operation
    BF_CLR(LRADC_CTRL0, SFTRST);
    // Clear the Clock Gate for normal operation
    BF_CLR(LRADC_CTRL0, CLKGATE);

    // connect to internal temp sensing
    BF_CLR(LRADC_CTRL2, TEMPSENSE_PWD);
    switch (eChannel0)
    {
        case LRADC_CH0:
            BF_WR(LRADC_CTRL4, LRADC0SELECT, 8);
            break;
        case LRADC_CH1:
            BF_WR(LRADC_CTRL4, LRADC1SELECT, 8);
            break;
        case LRADC_CH2:
            BF_WR(LRADC_CTRL4, LRADC2SELECT, 8);
            break;
        case LRADC_CH3:
            BF_WR(LRADC_CTRL4, LRADC3SELECT, 8);
            break;
        case LRADC_CH4:
            BF_WR(LRADC_CTRL4, LRADC4SELECT, 8);
            break;
        case LRADC_CH5:
            BF_WR(LRADC_CTRL4, LRADC5SELECT, 8);
            break;
        case LRADC_CH6:
            BF_WR(LRADC_CTRL4, LRADC6SELECT, 8);
            break;
        case LRADC_CH7:
            BF_WR(LRADC_CTRL4, LRADC7SELECT, 8);
            break;
        default:
            break;

    }

    switch (eChannel1)
    {
        case LRADC_CH0:
            BF_WR(LRADC_CTRL4, LRADC0SELECT, 9);
            break;
        case LRADC_CH1:
            BF_WR(LRADC_CTRL4, LRADC1SELECT, 9);
            break;
        case LRADC_CH2:
            BF_WR(LRADC_CTRL4, LRADC2SELECT, 9);
            break;
        case LRADC_CH3:
            BF_WR(LRADC_CTRL4, LRADC3SELECT, 9);
            break;
        case LRADC_CH4:
            BF_WR(LRADC_CTRL4, LRADC4SELECT, 9);
            break;
        case LRADC_CH5:
            BF_WR(LRADC_CTRL4, LRADC5SELECT, 9);
            break;
        case LRADC_CH6:
            BF_WR(LRADC_CTRL4, LRADC6SELECT, 9);
            break;
        case LRADC_CH7:
            BF_WR(LRADC_CTRL4, LRADC7SELECT, 9);
            break;
        default:
            break;
    }

    hw_lradc_ScheduleChannel(eChannel0);
    // Wait for conversion complete
    while(hw_lradc_GetInterruptFlag(eChannel0) == 0);
    //Clear interrupt flag
    hw_lradc_ClearInterruptFlag(eChannel0);
    // read temperature value
    ch8Value = hw_lradc_GetAccumValue(eChannel0);
    hw_lradc_ClearAccum(eChannel0);

    hw_lradc_ScheduleChannel(eChannel1);
    // Wait for conversion complete
    while(hw_lradc_GetInterruptFlag(eChannel1) == 0);
    //Clear interrupt flag
    hw_lradc_ClearInterruptFlag(eChannel1);
    // read temperature value
    ch9Value = hw_lradc_GetAccumValue(eChannel1);
    hw_lradc_ClearAccum(eChannel1);

    BF_SET(LRADC_CTRL2, TEMPSENSE_PWD);

    return (uint16_t)((ch9Value-ch8Value)*GAIN_CORRECTION/4000);

}


////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_lradc_MeasureBatteryTemperature(hw_lradc_Channel_t BattTempChannel)
{
    uint32_t u32HighCurrentReading = 0;
    uint32_t u32LowCurrentReading = 0;
    uint16_t u16Temp;
    uint16_t i;
    hw_lradc_TempSensor_t TempSensor;

    //--------------------------------------------------------------------------
    // Check the channel.  Only channel 0 or 1 should be used since the others
    // are reserved or cannot induce current.
    //--------------------------------------------------------------------------
    if(BattTempChannel == LRADC_CH0)
    {
        TempSensor = TEMP_SENSOR0;
    }
    else if(BattTempChannel == LRADC_CH1)
    {
        TempSensor = TEMP_SENSOR1;
    }
    else
    {
        return 0;
    }


    //--------------------------------------------------------------------------
    // Enable the temperature sensor.
    //--------------------------------------------------------------------------
    hw_lradc_EnableTempSensor(true);


    //--------------------------------------------------------------------------
    // Setup the temperature sensor for high current measurement.  Wait while
    // the current ramps up.
    //--------------------------------------------------------------------------
    hw_lradc_SetTempSensorCurrent(TempSensor,TEMP_SENSOR_CURRENT_HIGH_READING);
    hw_digctl_MicrosecondWait(TEMP_SENSOR_CURRENT_RAMP_DELAY);


    //--------------------------------------------------------------------------
    // Set up a loop to take a specified number of samples.  Then take the
    // average of the samples.
    //--------------------------------------------------------------------------
    for(i=0;i<NUM_TEMP_READINGS_TO_AVG;i++)
    {
        // Schedule the measurement and wait for it to complete.
        hw_lradc_ScheduleChannel(BattTempChannel);
        while(hw_lradc_GetInterruptFlag(BattTempChannel) == 0);
        hw_lradc_ClearInterruptFlag( BattTempChannel );

        // Read the measurement and then clear the accumulator to prepare
        // for the next reading.
        u32HighCurrentReading += hw_lradc_GetAccumValue(BattTempChannel);
        hw_lradc_ClearAccum( BattTempChannel );
    }
    // Take the average the high current readings.
    u32HighCurrentReading /= NUM_TEMP_READINGS_TO_AVG;


    //--------------------------------------------------------------------------
    // Setup the temperature sensor for low current measurement.  Wait while
    // the current ramps up.
    //--------------------------------------------------------------------------
    hw_lradc_SetTempSensorCurrent(TempSensor,TEMP_SENSOR_CURRENT_LOW_READING);
    hw_digctl_MicrosecondWait(TEMP_SENSOR_CURRENT_RAMP_DELAY);


    //--------------------------------------------------------------------------
    // Set up a loop to take a specified number of samples.  Then take the
    // average of the samples.
    //--------------------------------------------------------------------------
    for(i=0;i<NUM_TEMP_READINGS_TO_AVG;i++)
    {
        // Schedule the measurement and wait for it to complete.
        hw_lradc_ScheduleChannel(BattTempChannel);
        while(hw_lradc_GetInterruptFlag(BattTempChannel) == 0);
        hw_lradc_ClearInterruptFlag( BattTempChannel );

        // Read the measurement and then clear the accumulator to prepare
        // for the next reading.
        u32LowCurrentReading += hw_lradc_GetAccumValue(BattTempChannel);
        hw_lradc_ClearAccum( BattTempChannel );
    }
    // Take the average the low current readings.
    u32LowCurrentReading /= NUM_TEMP_READINGS_TO_AVG;


    //--------------------------------------------------------------------------
    // Turn off the current to the temperature sensor to save power, and disable
    // the temperature sensing block.
    //--------------------------------------------------------------------------
    hw_lradc_SetTempSensorCurrent(TempSensor,TEMP_SENSOR_CURRENT_0UA);
    hw_lradc_EnableTempSensor(false);


    //--------------------------------------------------------------------------
    // Do any LRADC reading to temperature conversions here...
    // Then return the temperature.
    //--------------------------------------------------------------------------
    {
        //----------------------------------------------------------------------
        // This is the conversion if using a 1N4148 diode without compensating
        // for any routing impedance to the diode.
        //----------------------------------------------------------------------
        {
            u16Temp = ((u32HighCurrentReading-u32LowCurrentReading) *
                        TEMP_SENSOR_CONVERSION_CONSTANT)/1000;
        }

    }


    return u16Temp;
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_SetTempSensorCurrent(hw_lradc_TempSensor_t TempSensor,
                                   hw_lradc_CurrentMagnitude_t Current)
{
    //--------------------------------------------------------------------------
    // Set channel 0 temperature sensor current.  Turn temp sensor on or off
    // depending on the current magnitude.
    //--------------------------------------------------------------------------
    if(TempSensor == TEMP_SENSOR0)
    {
        HW_LRADC_CTRL2.B.TEMP_ISRC0 = Current;

        if(Current > TEMP_SENSOR_CURRENT_0UA)
        {
            HW_LRADC_CTRL2.B.TEMP_SENSOR_IENABLE0 = 1;
        }
        else
        {
            HW_LRADC_CTRL2.B.TEMP_SENSOR_IENABLE0 = 0;
        }

    }

    //--------------------------------------------------------------------------
    // Set channel 1 temperature sensor current.  Turn temp sensor on or off
    // depending on the current magnitude.
    //--------------------------------------------------------------------------
    if(TempSensor == TEMP_SENSOR1)
    {
        HW_LRADC_CTRL2.B.TEMP_ISRC1 = Current;

        if(Current > TEMP_SENSOR_CURRENT_0UA)
        {
            HW_LRADC_CTRL2.B.TEMP_SENSOR_IENABLE1 = 1;
        }
        else
        {
            HW_LRADC_CTRL2.B.TEMP_SENSOR_IENABLE1 = 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//! See hw_lradc.h for details.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTempSensor(bool bEnable)
{
    HW_LRADC_CTRL2.B.TEMPSENSE_PWD = !bEnable;
}

