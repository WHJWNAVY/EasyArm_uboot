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

#ifndef _DDI_POWER_H
#define _DDI_POWER_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

// This is only here because of the dependency in the USb driver.  When that is
// cleaned up, we can remove this.
#include "hw/lradc/hw_lradc.h"
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! \brief TBD
#define DDI_PWR_HANDOFF_DEBOUNCE_TICKS  3
#define DDI_POWER_HANDOFF_DEBOUNCE_TICKS 3
//! \brief TBD

#define DDI_POWER_MAX_VOLTAGE_STEP_MV     100    // in millivolts

//! \brief The minimum voltage we will consider for initial battery detection.
#define BATT_TEST__MIN_BATT_VOLT 2800
//! \brief The voltage we would see on the battery pin with an open circuit.
#define BATT_TEST__MIN_OPEN_CIRC_VOLT 4000
//! \brief Time to allow battery pin to rise if there is an open circuit.  After
//! this time elapses, we should know whether or not we have a battery.
#define BATT_TEST__WAIT_TIME_US 300000

//! \brief Voltage threshold where 4P2 comparison switches between 85% and 105%.
#define BATT_VOLT__CMPTRIP_THRESHOLD 3700
//! \brief Voltage threshold where 4P2 target switches between high and low setting.
#define BATT_VOLT__4P2TRG_THRESHOLD 3700

//! \brief Maximum time (mS) to wait for the lock/unlock function to complete.
#define MAX_LOCK_WAIT_MS 100



//! \brief Possible power sources for each power supply
typedef enum _ddi_power_PowerSource_t
{
	//! \brief Powered by linear regulator.
    DDI_POWER_LINEAR_REGULATOR,
	//! \brief Powered by DCDC converter.
    DDI_POWER_DCDC
}ddi_power_PowerSource_t;


//! \brief Available 5V detection methods.
typedef enum _ddi_power_5vDetection_t
{
    //! \brief VBUSVALID will be used for 5V/USB detection.
    DDI_POWER_VBUSVALID,
    //! \brief VDD5V_GT_VDDIO will be used for 5V/USB detection.
    DDI_POWER_VDD5V_GT_VDDIO,
    //! \brief Uses VBUSVALID for 5V detection, but does not rely on the interrupt.
    DDI_POWER_VBUSVALID_WITH_POLLING
} ddi_power_5vDetection_t;

//! \brief Possible internal FET configurations for DCDC converter.
typedef enum _ddi_power_FetStrength_t
{
    //! \brief Internal power FETs will have approximately half their strength.
    DDI_POWER_FET_STRENGTH_HALF,
    //! \brief Internal power FETs will be normal strength.
    DDI_POWER_FET_STRENGTH_NORMAL,
    //! \brief Internal power FETs will have approximately double their strength.
    DDI_POWER_FET_STRENGTH_DOUBLE,
} ddi_power_FetStrength_t;

//! \brief typedef for the power event message generator routine
typedef void ddi_power_MsgHandler_t(void);

////////////////////////////////////////////////////////////////////////////////
// Structures
////////////////////////////////////////////////////////////////////////////////
typedef struct _ddi_power_InitValues_t
{
    //! \brief Time in milliseconds between battery voltage samples.
    uint32_t                    u32BatterySamplingInterval;
    //! \brief 5V detection used by software.
    ddi_power_5vDetection_t     e5vDetection;
    //! \brief 5V detection used by DCDC hardware.
    ddi_power_5vDetection_t     eDcdc5vDetection;
    //! \brief Selects whether or not to use 4p2 rail.
    bool                        bEnable4p2;
    //! \brief Voltage level for 5V to be considered valid. (mV)
    uint16_t                    u16Valid5vThreshold;
} ddi_power_InitValues_t;


////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////

extern uint8_t g_ddi_power_SafeBatteryVoltageCode;
extern uint8_t g_ddi_power_BatteryBrownOutVoltageCode;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

//Power supply driver public API.
////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Initializes the power driver
//!
//! \fntype Function
//! This function initializes the power-supply hardware.
//!
//! \param[in]  InitValues Structure with initial values for the power driver.
//!
//! \return     SUCCESS No error
//!             Others  Error
//!
//! \internal
//! \see To view the function definition, see ddi_power_init.c.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t  ddi_power_Init(ddi_power_InitValues_t* pInitValues);

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
bool IsPowerDriverInitialized( void );

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
bool IsBatteryMonitorInitialized( void );

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
bool IsPowerSupplyInitialized( void );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Returns status of the 5V initialization.
//!
//! \fntype Function
//!
//! \retval True 5V detection has been initialized.
//! \retval False 5V detection has not been initialized.
/////////////////////////////////////////////////////////////////////////////////
bool Is5vDetectionInitialized( void );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Returns status of the 5V IRQ initialization.
//!
//! \fntype Function
//!
//! \retval True 5V IRQs have been initialized.
//! \retval False 5V IRQs have not been initialized.
/////////////////////////////////////////////////////////////////////////////////
bool Is5vIrqInitialized( void );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Returns status of the hardware power driver initialization.
//!
//! \fntype Function
//!
//! \retval True Hardware power driver has been initialized.
//! \retval False Hardware power driver has not been initialized.
/////////////////////////////////////////////////////////////////////////////////
bool IsHwPowerInitialized( void );


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set the VDDD and its brownout voltages.
//!
//! \fntype     Non-reentrant Function
//!
//! This function sets the VDDD value and VDDD brownout level specified by the
//! input parameters. If the new brownout level is equal to the current setting
//! it'll only update the VDDD setting. If the new brownout level is less than
//! the current setting, it will update the VDDD brownout first and then the VDDD.
//! Otherwise, it will update the VDDD first and then the brownout. This
//! arrangement is intended to prevent from false VDDD brownout. This function
//! will not return until the output VDDD is stable.
//!
//! \param[in]  u16Vddd_mV              Vddd voltage in millivolts
//! \param[in]  u16VdddBrownout_mV      Vddd brownout in millivolts
//!
//! \return SUCCESS.
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t  ddi_power_SetVddd(uint16_t  u16Vddd_mV, uint16_t  u16VdddBrownout_mV);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDD voltage
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present value of the VDDD voltage
//!
//! \retval Present VDDD voltage in mV
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVddd(void);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDD brownout level
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present value VDDD brownout
//!
//! \retval Present VDDD brownout voltage in mV
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVdddBrownout(void);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set the VDDA and its brownout voltages.
//!
//! \fntype     Non-reentrant Function
//!
//! This function sets the VDDA value and VDDA brownout level specified by the
//! input parameters. If the new brownout level is equal to the current setting
//! it'll only update the VDDA setting. If the new brownout level is less than
//! the current setting, it will update the VDDA brownout first and then the VDDA.
//! Otherwise, it will update the VDDA first and then the brownout. This
//! arrangement is intended to prevent from false VDDA brownout. This function
//! will not return until the output VDDA stable.
//!
//! \param[in]  u16Vdda_mV              Vdda voltage in millivolts
//! \param[in]  u16VddaBrownout_mV      Vdda brownout in millivolts
//!
//! \return SUCCESS.
//!
//! \internal
//! \see To view the function definition, see ddi_power.c.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t  ddi_power_SetVdda(uint16_t  u16Vdda_mV, uint16_t  u16VddaBrownout_mV);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDA voltage
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present value of the VDDA voltage
//!
//! \retval Present VDDA voltage in mV
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVdda(void);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDA brownout level
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present value VDDA brownout
//!
//! \retval Present VDDA brownout voltage in mV
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVddaBrownout(void);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set the VDDIO and its brownout voltages.
//!
//! \fntype     Non-reentrant Function
//!
//! This function sets the VDDIO value and VDDIO brownout level specified by the
//! input parameters. If the new brownout level is equal to the current setting
//! it'll only update the VDDIO setting. If the new brownout level is less than
//! the current setting, it will update the VDDIO brownout first and then the VDDIO.
//! Otherwise, it will update the VDDIO first and then the brownout. This
//! arrangement is intended to prevent from false VDDIO brownout. This function
//! will not return until the output VDDIO stable.
//!
//! \param[in]  u16Vddio_mV              Vddio voltage in millivolts
//! \param[in]  u16VddioBrownout_mV      Vddio brownout in millivolts
//!
//! \return SUCCESS.
//!
//! \internal
//! \see To view the function definition, see ddi_power.c.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t  ddi_power_SetVddio(uint16_t  u16Vddio_mV, uint16_t  u16VddioBrownout_mV);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDIO voltage
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present value of the VDDIO voltage
//!
//! \retval Present VDDIO voltage in mV
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVddio(void);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Read the VDDIO brownout level
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present value VDDIO brownout
//!
//! \retval Present VDDIO brownout voltage in mV
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetVddioBrownout(void);


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Get the battery voltage
//!
//! \fntype     Reentrant Function
//!
//! This function returns the present value of the battery voltage The battery
//! value is from LRADC channel -7 battery measurement.
//!
//! \retval Present battery voltage in mV
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t ddi_power_GetBattery(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable 5V-to-battery handoff
//!
//! \fntype Function
//!
//! This function prepares the hardware for a 5V-to-battery handoff.  It assumes
//! the current configuration is using 5V as the power source.  The 5V
//! interrupt will be set up for a 5V removal.
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_Enable5VoltsToBatteryHandoff(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Transfers the power source from 5V to the battery.
//!
//! \fntype Function
//!
//! This function will handle all the power rail transitions necesarry to power
//! the chip from the battery when it was previously powered from the 5V power
//! source.
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_Execute5VoltsToBatteryHandoff(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable battery-to-5V handoff
//!
//! \fntype Function
//!
//! This function sets up battery-to-5V handoff. The power switch from
//! battery to 5V is automatic. This funtion enables the 5V present detection
//! such that the 5V interrupt can be generated if it is enabled. (The interrupt
//! handler can inform software the 5V present event.) To deal with noise or
//! a high current, this function enables DCDC1/2 based on the battery mode.
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_EnableBatteryTo5VoltsHandoff(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Transfers the  power source from battery to 5V.
//!
//! \fntype Function
//!
//! This function handles the transitions on each of the power rails necessary
//! to power the chip from the 5V power supply when it was previously powered
//! from the 5V power supply.
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_ExecuteBatteryTo5VoltsHandoff(void);




////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Waits until the VDDD power supply is stable
//!
//! \fntype Function
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_WaitForVdddStable(void);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Applies floor and ceiling to target and brownout
//!
//! \fntype Function
//!
//! \param[in] pu16Vddd_mV - proposed target voltage
//! \param[in] pu16Bo_mV - proposed brownout voltage
//!
//! \param[out] - pu16Vddd_mV - valid target voltage
//! \param[out] - pu16Bo_mV - valid brownout voltage
//!
//! \retval SUCCESS - no adjustments made to proposed voltages
//! \retval ERROR_DDI_POWER_VDDD_PARAM_ADJUSTED - adjustments made to one or both
//! \retval                                       proposed voltages
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_LimitVdddAndBo(uint16_t *pu16Vddd_mV, uint16_t *pu16Bo_mV);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Waits until the VDDIO power supply is stable
//!
//! \fntype Function
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_WaitForVddioStable(void);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Applies floor and ceiling to target and brownout
//!
//! \fntype Function
//!
//! \param[in] pu16Vddio_mV - proposed target voltage
//! \param[in] pu16Bo_mV - proposed brownout voltage
//!
//! \param[out] - pu16Vddio_mV - valid target voltage
//! \param[out] - pu16Bo_mV - valid brownout voltage
//!
//! \retval SUCCESS - no adjustments made to proposed voltages
//! \retval ERROR_DDI_POWER_VDDIO_PARAM_ADJUSTED - adjustments made to one or both
//! \retval                                       proposed voltages
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_LimitVddioAndBo(uint16_t *pu16Vddio_mV, uint16_t *pu16Bo_mV);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Waits until the VDDA power supply is stable
//!
//! \fntype Function
//!
////////////////////////////////////////////////////////////////////////////////
void ddi_power_WaitForVddaStable(void);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Applies floor and ceiling to target and brownout
//!
//! \fntype Function
//!
//! \param[in] pu16Vdda_mV - proposed target voltage
//! \param[in] pu16Bo_mV - proposed brownout voltage
//!
//! \param[out] - pu16Vdda_mV - valid target voltage
//! \param[out] - pu16Bo_mV - valid brownout voltage
//!
//! \retval SUCCESS - no adjustments made to proposed voltages
//! \retval ERROR_DDI_POWER_VDDA_PARAM_ADJUSTED - adjustments made to one or both
//! \retval                                       proposed voltages
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_LimitVddaAndBo(uint16_t *pu16Vdda_mV, uint16_t *pu16Bo_mV);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the strength of the internal power FETs
//!
//! This function will set the strength of the FETs depending on the current
//! system requirements.  Generally, stronger FETs will result in more power
//! usage and better performance.  Weaker FETs will result in lower performance
//! and much less power usage.
//!
//! \param [in] Strength FET strength setting
//!
//! \retval SUCCESS
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_SetPowerFetStrength( ddi_power_FetStrength_t Strength );

/////////////////////////////////////////////////////////////
//! \brief Returns the logical state of the 4p2 rail.
//!
//! This returns a logical state, not a hardware state.  Unforunatley
//! we cannot determine if 4p2 id definitely active just by reading
//! the registers.
//!
//! \retval True The 4p2 rail is enabled
//! \retval False The 4p2 rail is disabled.
//!
/////////////////////////////////////////////////////////////
bool ddi_power_Is4p2Enabled(void);

/////////////////////////////////////////////////////////////
//! \brief Saves the logical state of the 4p2 rail.
//!
//! This returns a logical state, not a hardware state.  Unforunatley
//! we cannot determine if 4p2 id definitely active just by reading
//! the regiesters.
//!
//! \param[in] b4p2IsEnabled Logical state of the 4p2 rail.  Set to
//! true if 4p2 is enabled and false if disabled.
//!
/////////////////////////////////////////////////////////////
void ddi_power_Save4p2Enabled( bool b4p2IsEnabled );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Enables the 4p2 rail and readies it for use.
//!
//! \fntype Function
//!
//! This function enables the 4p2 rail and DCDC from 4p2 if 5V is connected.  If
//! 5V is not connected, this functionality will cause 4p2 and DCDC from 4p2 to
//! become immediately active once 5V is connected and valid.  If successful,
//! the logical state of the 4p2 rail will be changed to "enabled"
//!
//! \retval SUCCESS The 4p2 rail is ready for use.
//! \retval ERROR_DDI_FAILED_TO_PREPARE_4P2_RAIL The system could not prepare
//! power supplies to use the 4p2 rail when 5V becomes present.
//! \retval ERROR_DDI_FAILED_TO_START_4P2_RAIL The 4p2 could not start properly.
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Enable4p2( void );

///////////////////////////////////////////////////////////////////////////////
//! \brief Disables the 4p2 rail.
//!
//! The 4p2 rail will not be used as power source until it is enabled again.
//! The logical state of the 4p2 rail will be changed to "disabled".
//!
//! \retval SUCCESS
///////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Disable4p2( void );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Prepare the 4p2 rail for use when 5V becomes valid.
//!
//! \fntype Function
//!
//! When 5V is not valid, the 4p2 rail will be prepared so it can turn on
//! when 5V does become valid.
//!
//! \retval SUCCESS The 4p2 rail is ready to turn on when 5V is valid.
//! \retval ERROR_DDI_FAILED_TO_PREPARE_4P2_RAIL One of the three power sources,
//! VDDD, VDDA, or VDDIO, was not set properly.
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_PrepareToStart4p2( void );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Activates the 4p2 rail to allow the DCDC to use it as a power source.
//!
//! \fntype Function
//!
//! The 4p2 rail will be taken from a completely off state to a fully powered
//! state.  When this function completes, the DCDC will be powered from the
//! 4p2 rail.
//!
//! \retval SUCCESS
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Start4p2( void );

///////////////////////////////////////////////////////////////////////////////
//! \brief Stops DCDC 4p2, Powers down 4p2, and powers off internal 4p2 circuit
//!
//! \fntype Function
//!
//! Turns off the 4p2 rail.  The 4p2 rail will be powered down, but it is
//! ready to be turned on again.
//!
//! \retval SUCCESS The 4p2 rail was turned off successfully.
///////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Stop4p2( void );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Charges the 4p2 rail.
//!
//! \fntype Function
//!
//! Brings the rail's power up to a level that is usable by the DCDC.  The step
//! size can be adjusted to speed up the charge, but 5V must be monitored to
//! ensure a brownout does not occur.
//!
//! \param[in] u32NewChargeCurrentLimit New current limit, in milliamps.
//! \param[in] u32ChargeCurrentStepSize Amount of current, in milliamps, to
//! increment in each iteration.
//!
//! \retval SUCCESS
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Charge4p2Rail( uint32_t u32NewChargeCurrentLimit,
                                    uint32_t u32ChargeCurrentStepSize );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Charges the 4p2 capacitor.
//!
//! \fntype Function
//!
//! Applies an initial charge and voltage to the 4p2 capacitor.  This is
//! primarily for very low or dead battery conditions since the 4p2 pin will
//! be at a very low level.
//!
//! \param[in] u32NewChargeCurrentLimit The new current limit, in milliamps, that
//! should be set in hardware after the 4p2 capacitor has been charged.
//! \param[in] u32ChargeCurrentStepSize The amount of current, in milliamps, that
//! each iteration should increment by.
//!
//! \retval SUCCESS
//!
//! \note  This isn't the only thing needed because you can't enable DCDC for 4p2
//! until the circuiry is ready regardless of what voltage is present on 4p2.
//! This is mostly for the very low or dead battery cases.
//! \note  If there is external load on 4p2, we could get stuck here.
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Charge4p2Cap( uint32_t u32NewChargeCurrentLimit,
                                   uint32_t u32ChargeCurrentStepSize );



////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Determines if the battery is charged enough to power DCDC's after
//! a handoff.
//!
//! \fntype Function
//!
//! \retval true Battery is sufficiently charged to power DCDC.
//! \retval false Battery is not sufficient to power DCDC.
////////////////////////////////////////////////////////////////////////////////
bool ddi_power_IsBattValidForHandoff( void );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Saves the battery connected status.
//!
//! \fntype Function
//!
//! \param[in] bConnected True if battery is connected, false if no battery is
//! present.
/////////////////////////////////////////////////////////////////////////////////
void ddi_power_BatteryConnected( bool bConnected );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Returns status of battery connection.
//!
//! \fntype Function
//!
//! This function will return the status of the battery connection.  If the
//! status is unknown, the function will run the battery connection test.
//!
//! \retval True A battery is connected to the device.
//! \retval False A battery is not connected to the device.
/////////////////////////////////////////////////////////////////////////////////
bool ddi_power_IsBatteryConnected( void );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Runs the test to determine if a battery is connected to device.
//!
//! \fntype Function
//!
//! The test will turn on the charger with minimal charge current to determine
//! if a battery is present.  This test should only be run once since subsequent
//! tests may be invalid due to the charge left on the battery pin from
//! capacitance.
//!
//! \retval True Test determined battery is connected.
//! \retval False Test determined battery is not connected.
/////////////////////////////////////////////////////////////////////////////////
bool ddi_power_TestBatteryConnection( void );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Determines if the current application has already run the
//! battery connection test.
//!
//! \retval True Current application has already run the test once.
//! \retval False Current application has not yet run the test.
/////////////////////////////////////////////////////////////////////////////////
bool DidCurrAppRunBattTest( void );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Determines if a previous application has already run the
//! battery connection test.
//!
//! \retval True A previous application has already run the test once.
//! \retval False A previous application has not yet run the test.
/////////////////////////////////////////////////////////////////////////////////
bool DidPrevAppRunBattTest( void );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Saves the battery connection test result for a previous test.
//!
//! \param[in] bResult True if battery is present, false if not present.
/////////////////////////////////////////////////////////////////////////////////
void SetPrevAppTestResult( bool bResult );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Returns the battery connection test result for a previous test.
//!
//! \retval True Battery detected in a previous test.
//! \retval False Battery not detected in a previous test.
/////////////////////////////////////////////////////////////////////////////////
bool GetPrevAppTestResult( void );

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Enable or Disable safe voltage limits.  Only disable for testing
//! needs
//!
//! \fntype Function
//!
//! \retval none
////////////////////////////////////////////////////////////////////////////////
void ddi_power_EnableSafeVoltageLimits(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Determines if the battery is charged enough to power DCDC's after
//! a handoff.
//!
//! \fntype Function
//!
//! \retval true Safe voltage limits are enabled.
//! \retval false Safe voltage limits are disabled.
////////////////////////////////////////////////////////////////////////////////
bool ddi_power_GetSafeVoltageLimitsStatus(void);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the DCDC and software 5V detection methods.
//!
//! \fntype Function
//!
//! Configures the 5V detection method for the DCDC and software.  Also sets
//! the VBUSVALID 5V valid level.
//!
//! \param[in]  eDcdc5vDetection DCDC control logic 5V detection method.
//! \param[in]  eSw5vDetection Software's 5V detection method.
//! \param[in]  u16VbusValidThresh Voltage level in millivolts where 5V will be
//! considered valid.
//!
//! \retval SUCCESS Detection methods and thresholds set.
/////////////////////////////////////////////////////////////////////////////////
RtStatus_t ddi_power_Configure5vDetection( ddi_power_5vDetection_t eDcdc5vDetection,
                                           ddi_power_5vDetection_t eSw5vDetection,
                                           uint16_t u16VbusValidThresh );
#endif // _DDI_POWER_H



////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
