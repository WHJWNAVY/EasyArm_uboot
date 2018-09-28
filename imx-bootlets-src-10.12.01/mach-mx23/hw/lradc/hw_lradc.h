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
#ifndef __HW_LRADC_H
#define __HW_LRADC_H

#include "hw_lradc_errordefs.h"


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define LRADC_SFTRST_ON        1
#define LRADC_SFTRST_OFF       !LRADC_SFTRST_ON
#define LRADC_CLKGATE_OPEN     0
#define LRADC_CLKGATE_CLOSE    !LRADC_CLKGATE_OPEN

#define GAIN_CORRECTION        1012    // 1.012


//! Enumeration of LRADC temperature sensor interface
typedef enum _hw_lradc_TempSensor
{
    TEMP_SENSOR0,
    TEMP_SENSOR1
} hw_lradc_TempSensor_t;

//! Enumeration of LRADC temperature sensor current magnitude
typedef enum _hw_lradc_CurrentMagnitude
{
    TEMP_SENSOR_CURRENT_0UA,
    TEMP_SENSOR_CURRENT_20UA,
    TEMP_SENSOR_CURRENT_40UA,
    TEMP_SENSOR_CURRENT_60UA,
    TEMP_SENSOR_CURRENT_80UA,
    TEMP_SENSOR_CURRENT_100UA,
    TEMP_SENSOR_CURRENT_120UA,
    TEMP_SENSOR_CURRENT_140UA,
    TEMP_SENSOR_CURRENT_160UA,
    TEMP_SENSOR_CURRENT_180UA,
    TEMP_SENSOR_CURRENT_200UA,
    TEMP_SENSOR_CURRENT_220UA,
    TEMP_SENSOR_CURRENT_240UA,
    TEMP_SENSOR_CURRENT_260UA,
    TEMP_SENSOR_CURRENT_280UA,
    TEMP_SENSOR_CURRENT_300UA,
    TEMP_SENSOR_CURRENT_LOW_READING = TEMP_SENSOR_CURRENT_20UA,
    TEMP_SENSOR_CURRENT_HIGH_READING = TEMP_SENSOR_CURRENT_300UA

} hw_lradc_CurrentMagnitude_t;

//! Enumeration of LRADC Channels
typedef enum _hw_lradc_Channel
{
    LRADC_CH0           = 0,
    LRADC_CH1           = 1,
    LRADC_CH2           = 2,
    LRADC_CH3           = 3,
    LRADC_CH4           = 4,
    LRADC_CH5           = 5,
    LRADC_CH6           = 6,
    LRADC_CH7           = 7,
    LRADC_CH8           = 8,
    LRADC_CH9           = 9,
    LRADC_CH10          = 10,
    LRADC_CH11          = 11,
    LRADC_CH12          = 12,
    LRADC_CH13          = 13,
    LRADC_CH14          = 14,
    LRADC_CH15          = 15,

    VDDIO_VOLTAGE_CH    = LRADC_CH6,
    BATTERY_VOLTAGE_CH  = LRADC_CH7,
    PMOS_THIN_CH        = LRADC_CH8,
    NMOS_THIN_CH        = LRADC_CH9,
    NMOS_THICK_CH       = LRADC_CH10,
    PMOS_THICK_CH       = LRADC_CH11,
    USB_DP_CH           = LRADC_CH12,
    USB_DN_CH           = LRADC_CH13,
    VBG_CH              = LRADC_CH14,
    VDD5V_INPUT_CH      = LRADC_CH15,

} hw_lradc_Channel_t;

//! Enumeration of LRADC Delay Triggers
typedef enum _hw_lradc_DelayTrigger
{
    LRADC_DELAY_TRIGGER0    = 0,
    LRADC_DELAY_TRIGGER1    = 1,
    LRADC_DELAY_TRIGGER2    = 2,
    LRADC_DELAY_TRIGGER3    = 3,
    LRADC_DELAY_TRIGGER_MAX = 3
} hw_lradc_DelayTrigger_t;

//! Enumeration of LRADC clock frequency
typedef enum _hw_lradc_ClockFreq_t
{
    LRADC_CLOCK_6MHZ,
    LRADC_CLOCK_4MHZ,
    LRADC_CLOCK_3MHZ,
    LRADC_CLOCK_2MHZ
} hw_lradc_ClockFreq_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

//Function prototypes of the LRADC common
////////////////////////////////////////////////////////////////////////////////
//! \brief Initializes the LRADC block.
//! \fntype Non-Reentrant Function
//! \param[in] bEnableOnChipGroungRef 1 = Enable the onchip ground reference,
//!                                   0 = Disable the onchip ground reference
//! \param[in] eFreq                  LRADC conversion clock frequency
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_Init(bool bEnableOnChipGroungRef,  hw_lradc_ClockFreq_t eFreq);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Get the present state of a specified LRADC channel converter.
//! \fntype     Non-Reentrant Function
//  This function returns the value of CHANNELn_PRESENT bit of a specified LRADC
//  channel in HW_LRADC_STATUS register.
//! \param[in]  eChannel: A LRADC channel number
//! \retval 1: The converter is present on the chip.
//! \retval 0: The converter is not present on the chip.
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetChannelPresent(hw_lradc_Channel_t eChannel);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Schedule a specified LRADC channel.
//! \fntype     Non-Reentrant Function
//  This function sets the analog-to-digital conversion schedule for a specified
//  LRADC channel.
//! \param[in]  eChannel: A LRADC channel number.
//! \retval     None
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_ScheduleChannel(hw_lradc_Channel_t eChannel);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Enable or disable the interrupt of a LRADC channel.
//! \fntype     Non-Reentrant Function
//  This function enables or disables the interrupt of a specified LRADC channel
//  by setting or clearing the LRADCn_IRQ_EN bit of HW_LRADC_CTRL1 register.
//! \param[in]  eChannel A LRADC channel number
//! \param[in]  bValue 1 = Enable interrupt, 0 = Disable interrupt)
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableInterrupt(hw_lradc_Channel_t eChannel, bool bValue);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Gets the LRADC channel interrupt flag.
//! \fntype     Non-Reentrant Function
//  This function returns the value of the LRADCn_IRQ bit of HW_LRADC_CTRL1
//  register for a specified channel.
//! \param[in]  eChannel: A LRADC channel number
//! \retval     The value of LRADCn_IRQ bit
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetInterruptFlag(hw_lradc_Channel_t eChannel);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Clears the interrupt flag of a specified LRADC channel.
//! \fntype     Non-Reentrant Function
//  This function clears the interrupt flag of a specified LRADC channel in the
//  HW_LRADC_CTRL1 register.
//! \param[in]  eChannel: A LRADC channel number
//! \retval     None
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_ClearInterruptFlag(hw_lradc_Channel_t eChannel);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Configure a specified LRADC channel.
//! \fntype     Non-Reentrant Function
//  This function configures a specified LRADC channel with the input paramters.
//! \param[in]  eChannel            LRADC channel number.
//! \param[in]  bEnableDivideByTwo  1 = Enable the divide-by-two circuit,
//!                                 0 = Disable the divide-by-two circuit.
//! \param[in]  bEnableAccum        1 = Enable the channel accumulation,
//!                                 0 = Disable the channel accumulation.
//! \param[in]  u8NumSamples        Number of conversion cycles to sum together
//!                                 to the accumulator.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_ConfigureChannel(hw_lradc_Channel_t  eChannel,
                               bool                bEnableDivideByTwo,
                               bool                bEnableAccum,
                               uint8_t             u8NumSamples);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Maps the physical LRADC channel to the virtual channel
//! \fntype     Non-Reentrant Function
//  This function configures a virtual LRADC channel with a physical
//  channel.  Allows the use channel 8 - 15
//! \param[in]  eVirtualChannel     Virtual channel number. Channels 0-7
//! \param[in]  ePhysicalChannel    Physical channel number. Channels 0-15
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_AssignChannel( hw_lradc_Channel_t eVirtualChannel,
                             hw_lradc_Channel_t ePhysicalChannel );

////////////////////////////////////////////////////////////////////////////////
//! \brief      Gets the Toggle flag of a specified LRADC channel.
//! \fntype     Non-Reentrant Function
//  This function returns the value of the TOGGLE bit of HW_LRADC_CHn register.
//! \param[in]  eChannel: A LRADC channel number
//! \retval     The value of TOGGLE bit of a specified LRADC channel
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetToggleFlag(hw_lradc_Channel_t eChannel);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Gets the co-nversion value of a specified LRADC channel.
//! \fntype     Non-Reentrant Function
//  This function returns the most recent conversion value of a specified
//  channel. If the accumulation is enabled it is the sum of the most
//  recent N accumulated values.
//! \param[in]  eChannel:   A LRADC channel number
//! \retval     The value of the LRADC conversion.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_lradc_GetAccumValue(hw_lradc_Channel_t eChannel);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Clear the LRADC accumulator to zero.
//! \fntype     Non-Reentrant Function
//  This function clears the VALUE bitfield of HW_LRADC_CHn register.
//! \param[in]  eChannel:   A LRADC channel number
//! \retval     The value of the LRADC conversion.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_ClearAccum(hw_lradc_Channel_t eChannel);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Set a delay trigger controller for specified LRADC channel(s).
//! \fntype     Non-Reentrant Function
//  This function configures a specified LRADC delay trigger to trigger either
//  the LRADC channels or the delay triggers, or both
//! \param[in]  eDelayTrigger:      A LRADC delay trigger
//! \param[in]  u32TriggerLradcs:   The value of TRIGGER_LRADCS bitfield
//! \param[in]  u32DelayTriggers:   The value of TRIGGER_DELAYS bitfield
//! \param[in]  u32LoopCount:       The value of LOOP_COUNT bitfield
//! \param[in]  u32DelayCount:      The value of DEALY bitfield
//! \retval     None
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_SetDelayTrigger(hw_lradc_DelayTrigger_t   eDelayTrigger,
                              uint32_t                  u32TriggerLradcs,
                              uint32_t                  u32DelayTriggers,
                              uint32_t                  u32LoopCount,
                              uint32_t                  u32DelayCount);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Start or stop the delay trigger cycle of a specified LRADC
//!             delay trigger
//! \fntype     Non-Reentrant Function
//  This function starts or stops a LRADC delay trigger cycle by setting or
//  clearing the KICK bit of HW_LRADC_DELAYn register.
//! \param[in]  eDelayTrigger: A LRADC delay trigger
//! \param[in]  bValue (1 = Start the specified delay trigger,
//!                     0 = Stop the specified delay trigger)
//! \retval     None
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_SetDelayTriggerKick(hw_lradc_DelayTrigger_t eDelayTrigger,
                                  bool bValue);

//Function prototypes for the LRADC resistor ladder
////////////////////////////////////////////////////////////////////////////////
//! \brief      Init a LRADC channel for a resistor ladder
//! \fntype     Non-Reentrant Function
//  This function initializes a lradc channel.
//! \param[in]  eChannel:   A LRADC channel number
//! \param[in]  eTrigger:   A LRADC trigger to trigger the specified channel
//! \param[in]  u16SamplingInterval: It's the 11-bitfiled, DELAY, of
//!                 HW_LRADC_DELAYn register. Represents the sampling interval
//!                 from 0 to 2047 (1023.5 ms) with 0.5ms incremental step.
//!                 e.g.  u16SamplingInterval=20 is 10ms.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_lradc_InitLadder(hw_lradc_Channel_t      eChannel,
                               hw_lradc_DelayTrigger_t eTrigger,
                               uint16_t                u16SamplingInterval);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Measure Vddio voltage level for a resistor ladder
//! \fntype     Non-Reentrant Function
//  This function configures LRACD channel-6 conversion and retrives ADC value
//  of Vddio voltage level.
//! \retval     Vddio
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_lradc_MeasureVddio(void);

//Function prototypes for the LRADC touch screen I/F
////////////////////////////////////////////////////////////////////////////////
//! \brief      Get the present state of the LRADC touch panel controller.
//! \fntype     Non-Reentrant Function
//  This function returns the value of TOUCH_PANEL_PRESENT bit of the LRADC
//  HW_LRADC_STATUS register.
//! \retval     1: The touch panel controller is present on the chip.
//!             0: The touch panel controller is not present on the chip.
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetTouchDetectPresent(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Enable or disable the LRADC touch detector.
//! \fntype     Non-Reentrant Function
//  This function enables or disables the touch panel touch detector by setting
//  or clearing the TOUCH_DETECT_ENABLE bit of HW_LRADC_CTRL1 register.
//! \param[in]  bValue (1 = Enable interrupt, 0 = Disable interrupt)
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTouchDetect(bool bValue);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Enable or disable the interrupt of a LRADC touch detector.
//! \fntype     Non-Reentrant Function
//  This function enables or disables the interrupt of the LRADC touch detector
//  comparator by setting or clearing the TOUCH_DETECT_IRQ_EN bit of
//  HW_LRADC_CTRL1 register.
//! \param[in]  bValue (1 = Enable interrupt, 0 = Disable interrupt)
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTouchDetectInterrupt(bool bValue);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Get the interrupt flag of the LRADC touch detector.
//! \fntype     Non-Reentrant Function
//  This function returns the interrupt flag of the LRADC touch detector,
//  which is the TOUCH_DETECT_IRQ bit in the HW_LRADC_CTRL1 register.
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetTouchDetectInterruptFlag(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Clears the interrupt flag of the LRADC touch detector.
//! \fntype     Non-Reentrant Function
//  This function clears the interrupt flag of the LRADC touch detector by
//  clearing the TOUCH_DETECT_IRQ bit in the HW_LRADC_CTRL1 register.
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_ClearTouchDetectInterruptFlag(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Enable or disable the X-drive of a LRADC touch detector.
//! \fntype     Non-Reentrant Function
//  This function enables or disables the X-drive of the LRADC touch detector
//  by setting or clearing the XMINUS_ENABLE bit and XPLUS_ENABLE bit of
//  HW_LRADC_CTRL0 register.
//! \param[in]  bValue (1 = Enable X-Drive, 0 = Disable X-Xrive)
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTouchDetectXDrive(bool bValue);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Enable or disable the Y-drive of a LRADC touch detector.
//! \fntype     Non-Reentrant Function
//  This function enables or disables the Y-drive of the LRADC touch detector
//  by setting or clearing the YMINUS_ENABLE bit and YPLUS_ENABLE bit of
//  HW_LRADC_CTRL0 register.
//! \param[in]  bValue (1 = Enable Y-Drive, 0 = Disable Y-Xrive)
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTouchDetectYDrive(bool bValue);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Get the status of the LRADC touch detect comparator in the
//!             analog section.
//! \fntype     Non-Reentrant Function
//  This function returns the value of TOUCH_DETECT_RAW bit of the LRADC
//  HW_LRADC_STATUS register.
//! \retval     1: Hit (Someone is touching the panel)
//!             0: Open (No contact, open connection)
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetTouchDetect(void);

//Function prototypes for battery and Vddio voltage measurement I/F
void hw_lradc_SetLradc6Select(hw_lradc_Channel_t eChannel);
void hw_lradc_SetLradc7Select(hw_lradc_Channel_t eChannel);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Setup and start LRADC battery measurement
//! \fntype     Non-Reentrant Function
//  This function configures the LRADC for battery measurement and then starts
//  the battery measurement. The result will automatically write to the
//  BATT_VALUE field in HW_POWER_MONITOR register
//! \param[in]  eTrigger:            A LRADC delay trigger
//! \param[in]  u16SamplingInterval: A-to-D sample interval from 0 to 2047
//!                                  (1023.5 ms) with 0.5ms incremental step.
//!                                  e.g.  u16SamplingInterval=200 is 100ms.
//! \param[in]  eBatteryMode:        Battery mode
//! \retval     SUCCESS             Battery measurement setup successful
//!             ERROR_HW_LRADC_CH_NOT_PRESENT LRADC-7 is not present in hardware
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_lradc_EnableBatteryMeasurement(
	hw_lradc_DelayTrigger_t   eTrigger,uint16_t u16SamplingInterval);

hw_lradc_DelayTrigger_t hw_lradc_AcquireBatteryMonitorDelayTrigger(void);

//Function prototypes for the LRADC temperature sensor I/F
////////////////////////////////////////////////////////////////////////////////
//! \brief Get the present state of a specified LRADC temperture sensor I/F.
//! \fntype Non-Reentrant Function
//  This function returns the value of TEMPn_PRESENT bit of a specified LRADC
//  temperature sensor interface in HW_LRADC_STATUS register.
//! \param[in] eSensor TBD
//! \return Boolean value that indicates whether the temperature sensor I/F is
//!  present on this chip.
//! \retval 1 Temperature sensor is present.
//! \retval 0 Temperature sensor is not present.
//! \todo [PUBS] Replace TBD(s) with definition
////////////////////////////////////////////////////////////////////////////////
bool hw_lradc_GetTempSensorPresent(hw_lradc_TempSensor_t eSensor);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Set the current value to a specified LRADC temperture sensor
//! \fntype     Non-Reentrant Function
//  This function sets the current magnitude for a specified temperature sensor.
//! \param[in]  eSensor: A LRADC sensor interface number
//! \param[in]  eValue:  the current magnitude to the sensor.
//! \retval     None
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_SetTempSensorCurrentValue(hw_lradc_TempSensor_t       eSensor,
                                        hw_lradc_CurrentMagnitude_t eValue);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Enable or disable the current source of a specified LRADC
//  temperture sensor
//! \fntype     Non-Reentrant Function
//  This function enables or disables the current source of a specified LRADC
//  temperature sensor by setting or clearing the TEMP_SENSOR bit of
//  HW_LRADC_CTRL2 register.
//! \param[in]  eSensor: A LRADC sensor interface number
//! \param[in]  bValue:  1 = Enable the sensor current
//!                      0 = Disable the sensor current
//! \retval     None
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTempSensorCurrent(hw_lradc_TempSensor_t  eSensor,
                                      bool                   bValue);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Measure the internal die temperature using 2 specified LRADC
//  channels
//! \fntype     Non-Reentrant Function
//  This function measures the internal die temperature using 2 LRADC channels
//! \param[in]  eChannel0: the first LRADC Channel
//! \param[in]  eChannel1: the second LRADC Channel
//! \retval     internal die temperature in degree Kelvin
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_lradc_MeasureInternalDieTemperature(hw_lradc_Channel_t eChannel0,
                                                hw_lradc_Channel_t eChannel1);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Measure the battery temperature with the specified channel.
//!
//! \fntype     Non-Reentrant Function
//!
//! \param[in]  BattTempChannel Channel the function will check for the
//! battery temperature.
//!
//! \retval     battery temperature
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_lradc_MeasureBatteryTemperature(hw_lradc_Channel_t BattTempChannel);


////////////////////////////////////////////////////////////////////////////////
//! \brief      Sets the temperature sensor current.
//!
//! \fntype     Non-Reentrant Function
//!
//! \param[in]  TempSensor The channel to measure the battery temperature with.
//! \param[in]  Current The magnitude of the temperature sensor current.
//!
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_SetTempSensorCurrent(hw_lradc_TempSensor_t TempSensor,
                                   hw_lradc_CurrentMagnitude_t Current);

////////////////////////////////////////////////////////////////////////////////
//! \brief      Enables/disables the temperature sensor block.
//!
//! \fntype     Non-Reentrant Function
//!
//! \param[in]  bEnable True to enable the block, false to disable
////////////////////////////////////////////////////////////////////////////////
void hw_lradc_EnableTempSensor(bool bEnable);


void hw_lradc_EnableAutomaticBatteryUpdate(void);
void hw_lradc_DisableAutomaticBatteryUpdate(void);

#endif //__HW_LRADC_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}
