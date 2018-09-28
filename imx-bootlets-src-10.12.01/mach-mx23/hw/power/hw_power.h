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

#ifndef __HW_POWER_H
#define __HW_POWER_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "hw/lradc/hw_lradc.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! \brief The inverse of 6.25e-3 used in the DCFUNCV calculations
#define DCFUNCV_CONST_160 160
//! \brief Constant value for 1000 used in the DCFUNCV calculations
#define DCFUNCV_CONST_1000 1000
//! \brief Maximum possible value for DCFUNC bitfield
#define DCFUNCV_MAX_VALUE 1023

//! \brief Base voltage to start battery calculations for LiIon
#define BATT_BRWNOUT_LIION_BASE_MV_378x 2400
//! \brief Constant to help with determining whether to round up or not during
//! calculation
#define BATT_BRWNOUT_LIION_CEILING_OFFSET_MV 39
//! \brief Number of mV to add if rounding up in LiIon mode
#define BATT_BRWNOUT_LIION_LEVEL_STEP_MV 40
//! \brief Constant value to be calculated by preprocessing
#define BATT_BRWNOUT_LIION_EQN_CONST \
	(BATT_BRWNOUT_LIION_BASE_MV - BATT_BRWNOUT_LIION_CEILING_OFFSET_MV)
//! \brief Base voltage to start battery calculations for Alkaline/NiMH
#define BATT_BRWNOUT_ALKAL_BASE_MV 800
//! \brief Constant to help with determining whether to round up or not
#define BATT_BRWNOUT_ALKAL_CEILING_OFFSET_MV 19
//! \brief Number of mV to add if rounding up in Alkaline/NiMH mode
#define BATT_BRWNOUT_ALKAL_LEVEL_STEP_MV 20
//! \brief Constant value to be calculated by preprocessing
#define BATT_BRWNOUT_ALKAL_EQN_CONST \
	(BATT_BRWNOUT_ALKAL_BASE_MV - BATT_BRWNOUT_ALKAL_CEILING_OFFSET_MV)
//! \brief Max register code for battery brownout field.
#define BATT_BRWNOUT_MAX_REG_CODE_378x 0x1F

//! \brief Constant value for 8mV steps used in battery translation
#define BATT_VOLTAGE_8_MV 8
//! \brief Resolution in milliVolts of the BATT_VAL field.
#define BATT_VAL_FIELD_RESOL 8

//! \brief Register key value to allow writes to chip reset register
#define POWERDOWN_KEY 0x3e77

//! \brief The first LRADC channel to use
#define DIE_TEMP_CHAN_0 LRADC_CH2
//! \brief The second LRADC channel to use
#define DIE_TEMP_CHAN_1 LRADC_CH3

//! \brief Initial current limit for 4p2 rail when it is first powered on.
#define INITIAL_CHARGE_4P2_CURRENT_LIMIT 1
#define START_CAP_CHARGE_4P2_CURRENT_LIMIT 2

extern bool bEnableMaster5vBrownout;
extern bool bEnableMasterBattBrownout;
extern bool bEnableMasterCharge;
extern bool bEnableMaster4p2;
extern bool bMasterFlagsInitialized;
////////////////////////////////////////////////////////////////////////////////
// Enumerates
////////////////////////////////////////////////////////////////////////////////

//! \brief Important voltage values for VDDD, VDDA, and VDDIO
typedef enum _hw_power_VoltageValues_t
{
    //! \brief Size of voltage steps in mV.
    VOLTAGE_STEP_MV     = 25,
    //! \brief Minimum brownout offset from target voltage.
    BO_MIN_OFFSET_MV    = 25,
    //! \brief Maximum brownout offset from target voltage.
    BO_MAX_OFFSET_MV    = VOLTAGE_STEP_MV * 7,

    //! \brief Number of steps between target and brownout
    VDDD_MARGIN_STEPS   = 3,
    //! \brief Minimum margin between VDDD target and brownout
    VDDD_TO_BO_MARGIN  = (VDDD_MARGIN_STEPS * VOLTAGE_STEP_MV),
    //! \brief Default VDDD voltage at power on.
    VDDD_DEFAULT_MV    = 1200,
    //! \brief Default VDDD BO level at power on.
    VDDD_DEFAULT_BO    = (VDDD_DEFAULT_MV - VDDD_TO_BO_MARGIN),
    //! \brief Absolute minimum VDDD voltage.
    VDDD_ABS_MIN_MV    = 800,
    //! \brief Absolute maximum VDDD voltage.
    VDDD_ABS_MAX_MV    = 1550,
    //! \brief Maximum safe VDDD voltage.
    VDDD_SAFE_MAX_MV   = 1550,
    //! \brief Minimum safe VDDD voltage.
    VDDD_SAFE_MIN_MV   = (VDDD_ABS_MIN_MV + BO_MIN_OFFSET_MV),
    //! \brief Maximum voltage for alkaline battery
    VDDD_ALKALINE_MAX_MV = 1300,
    //! \brief Minimum possible VDDD brownout voltage
    VDDD_BO_MIN_MV     = 800,
    //! \brief Maximum possible VDDD brownout voltage
    VDDD_BO_MAX_MV     = 1475,
    //! \brief Base VDDD voltage for calculations.  Equivalent to 0 value reg.
    VDDD_BASE_MV       = VDDD_ABS_MIN_MV,

    //! \brief Number of steps between target and brownout
    VDDA_MARGIN_STEPS   = 4,
    //! \brief Minimum margin between VDDA target and brownout
    VDDA_TO_BO_MARGIN  = (VDDA_MARGIN_STEPS * VOLTAGE_STEP_MV),
    //! \brief Default VDDA voltage at power on.
    VDDA_DEFAULT_MV    = 1750,
    //! \brief Default VDDA BO level at power on.
    VDDA_DEFAULT_BO    = (VDDA_DEFAULT_MV - VDDA_TO_BO_MARGIN),
    //! \brief Absolute minimum VDDA voltage.
    VDDA_ABS_MIN_MV    = 1500,
    //! \brief Absolute maximum VDDA voltage.
    VDDA_ABS_MAX_MV    = 2100,
    //! \brief Maximum safe VDDA voltage.
    VDDA_SAFE_MAX_MV   = 1950,
    //! \brief Minimum safe VDDA voltage.
    VDDA_SAFE_MIN_MV   = 1725,
    //! \brief Minimum possible VDDA brownout voltage
    VDDA_BO_MIN_MV     = 1625,
    //! \brief Maximum possible VDDA brownout voltage
    VDDA_BO_MAX_MV     = 2175,
    //! \brief Base VDDA voltage for calculations. Equivalent to zero value reg.
    VDDA_BASE_MV       = VDDA_ABS_MIN_MV,

    //! \brief Number of steps between target and brownout
    VDDIO_MARGIN_STEPS  = 4,
    //! \brief Minimum margin between VDDIO target and brownout
    VDDIO_TO_BO_MARGIN  = (VDDIO_MARGIN_STEPS * VOLTAGE_STEP_MV),
    //! \brief Default VDDIO voltage at power on.
    VDDIO_DEFAULT_MV    = 3100,
    //! \brief Default VDDIO BO level at power on.
    VDDIO_DEFAULT_BO    = (VDDIO_DEFAULT_MV - VDDIO_TO_BO_MARGIN),
    //! \brief Absolute minimum VDDIO voltage.
    VDDIO_ABS_MIN_MV    = 2800,
    //! \brief Absolute maximum VDDIO voltage.
    VDDIO_ABS_MAX_MV    = 3575,
    //! \brief Maximum safe VDDIO voltage.
    VDDIO_SAFE_MAX_MV   = 3575,
    //! \brief Minimum safe VDDIO voltage.
    VDDIO_SAFE_MIN_MV   = (VDDIO_ABS_MIN_MV + BO_MIN_OFFSET_MV),
    //! \brief Minimum possible VDDIO brownout voltage
    VDDIO_BO_MIN_MV     = 2700,
    //! \brief Maximum possible VDDIO brownout voltage
    VDDIO_BO_MAX_MV     = 3475,
    //! \brief Base VDDIO voltage for calculations.  Equivalent to 0 value reg.
    VDDIO_BASE_MV       = VDDIO_ABS_MIN_MV,
} hw_power_VoltageValues_t;

//! \brief Threshold values for Vbus valid comparator
typedef enum _hw_power_VbusValidThresh_t
{

    //! \brief 2.900V threshold on insertion.
    VBUS_VALID_THRESH_2900 = 0,
    //! \brief 4.000V threshold on insertion.
    VBUS_VALID_THRESH_4000 = 1,
    //! \brief 4.100V threshold on insertion.
    VBUS_VALID_THRESH_4100 = 2,
    //! \brief 4.200V threshold on insertion.
    VBUS_VALID_THRESH_4200 = 3,
    //! \brief 4.300V threshold on insertion.
    VBUS_VALID_THRESH_4300 = 4,
    //! \brief 4.400V threshold on insertion.
    VBUS_VALID_THRESH_4400 = 5,
    //! \brief 4.500V threshold on insertion.
    VBUS_VALID_THRESH_4500 = 6,
    //! \brief 4.600V threshold on insertion.
    VBUS_VALID_THRESH_4600 = 7,
    //! \brief Maximum threshold value for the register setting.
    VBUS_VALID_THRESH_MAX  = 7,

    //! \brief Use under normal operating conditions.
    VBUS_VALID_THRESH_NORMAL = VBUS_VALID_THRESH_4300,
    //! \brief Use when a lower than normal threshold is needed.
    VBUS_VALID_THRESH_LOW    = VBUS_VALID_THRESH_4000,
    //! \brief Use when a higher than normal threshold is needed.
    VBUS_VALID_THRESH_HIGH   = VBUS_VALID_THRESH_4600,
    //! \brief Use only for testing, or under SigmaTel guidance.
    VBUS_VALID_THRESH_TEST   = VBUS_VALID_THRESH_2900

} hw_power_VbusValidThresh_t;

//! \brief Threshold values for Vbus droop.
typedef enum _hw_power_VbusDroopThresh_t
{
    //! \brief 4.300V threshold for droop detection.
    VBUS_DROOP_THRESH_4300 = 0,
    //! \brief 4.400V threshold for droop detection.
    VBUS_DROOP_THRESH_4400 = 1,
    //! \brief 4.500V threshold for droop detection.
    VBUS_DROOP_THRESH_4500 = 2,
    //! \brief 4.700V threshold for droop detection.
    VBUS_DROOP_THRESH_4700 = 3,
    //! \brief Maximum threshold value for the register setting.
    VBUS_DROOP_THRESH_MAX  = 3,

    //! \brief Use under normal operating conditions.
    VBUS_DROOP_THRESH_NORMAL = VBUS_DROOP_THRESH_4400,
} hw_power_VbusDroopThresh_t;

//! \brief Possible power sources for each power supply
typedef enum _hw_power_PowerSource_t
{
	//! \brief Powered by linear regulator.  DCDC output is gated off and
    //! the linreg output is equal to the target.
    HW_POWER_LINREG_DCDC_OFF,
    //! \brief Powered by linear regulator.  DCDC output is not gated off
    //! and is ready for the automatic hardware transistion after a 5V
    //! event.  The converters are not enabled when 5V is present. LinReg output
    //! is 25mV below target.
    HW_POWER_LINREG_DCDC_READY,
	//! \brief Powered by DCDC converter and the LinReg is on. LinReg output
    //! is 25mV below target.
    HW_POWER_DCDC_LINREG_ON,
    //! \brief Powered by DCDC converter and the LinReg is off. LinReg output
    //! is 25mV below target.
    HW_POWER_DCDC_LINREG_OFF,
    //! \brief Powered by DCDC converter and the LinReg is ready for the
    //! automatic hardware transfer.  The LinReg output is not enabled and
    //! depends on the 5V presence to enable the LinRegs.  LinReg offset is 25mV
    //!	 below target.
    HW_POWER_DCDC_LINREG_READY,
    //! \brief Powered by an external source when 5V is present.  This does not
    //! necessarily mean the external source is powered by 5V,but the chip needs
    //! to be aware that 5V is present.
    HW_POWER_EXTERNAL_SOURCE_5V,
    //! \brief Powered by an external source when 5V is not present.This doesn't
    //! necessarily mean the external source is powered by the battery, but the
    //! chip needs to be aware that the battery is present
    HW_POWER_EXTERNAL_SOURCE_BATTERY,
    //! \brief Unknown configuration.  This is an error.
    HW_POWER_UNKNOWN_SOURCE,
}hw_power_PowerSource_t;

//! \brief Battery charging current magnitudes converted to register settings.
typedef enum _hw_power_BattChargeCurrentMag_t
{
 	//! \brief Current magniude 400mA
    HW_POWER_CURRENT_MAG_400 = 0x20,
 	//! \brief Current magniude 200mA
    HW_POWER_CURRENT_MAG_200 = 0x10,
 	//! \brief Current magniude 100mA
    HW_POWER_CURRENT_MAG_100 = 0x08,
 	//! \brief Current magniude 50mA
    HW_POWER_CURRENT_MAG_50  = 0x04,
 	//! \brief Current magniude 20mA
    HW_POWER_CURRENT_MAG_20  = 0x02,
 	//! \brief Current magniude 10mA
    HW_POWER_CURRENT_MAG_10  = 0x01
} hw_power_BattChargeCurrentMag_t;

//! \brief Linear regulator offset values
typedef enum _hw_power_LinRegOffsetStep_t
{
	//! \brief No offset.  Linear regualator output equals target.
    HW_POWER_LINREG_OFFSET_NO_STEP    = 0,
	//! \brief Linear regulator is one 25mV step above the target.
    HW_POWER_LINREG_OFFSET_STEP_ABOVE = 1,
	//! \brief Linear regulator is one 25mV step below the target.
    HW_POWER_LINREG_OFFSET_STEP_BELOW = 2,
	//! \brief Max offset. Linear regulator is one 25mV step above target.
    HW_POWER_LINREG_OFFSET_MAX        = 3,
    //! \brief No step is used for LinReg mode.
    HW_POWER_LINREG_OFFSET_LINREG_MODE = HW_POWER_LINREG_OFFSET_NO_STEP,
    //! \brief No step is used for LinReg mode.
    HW_POWER_LINREG_OFFSET_DCDC_MODE = HW_POWER_LINREG_OFFSET_STEP_BELOW

} hw_power_LinRegOffsetStep_t;

//! \brief Possible RC Scale values to increase transient load response
typedef enum _hw_power_RcScaleLevels_t
{
	//! \brief Use nominal response
    HW_POWER_RCSCALE_DISABLED   = 0,
	//! \brief Increase response by factor of 2
    HW_POWER_RCSCALE_2X_INCR    = 1,
	//! \brief Increase response by factor of 4
    HW_POWER_RCSCALE_4X_INCR    = 2,
	//! \brief Increase response by factor of 8
    HW_POWER_RCSCALE_8X_INCR    = 3
} hw_power_RcScaleLevels_t;

//! \brief Interrupt polarities for power hardware
typedef enum _hw_power_InterruptPolarity_t
{
    //! \brief Generate interrupt when 5V is disconnected
    HW_POWER_CHECK_5V_DISCONNECTED      = 0,
    //! \brief Generate interrupt when 5V is connected
    HW_POWER_CHECK_5V_CONNECTED         = 1,
    //! \brief Generate interrupt when linear regulators are stable
    HW_POWER_CHECK_LINEAR_REGULATORS_OK = 1,
    //! \brief Generate interrupt when Pswitch goes high
    HW_POWER_CHECK_PSWITCH_HIGH         = 1,
    //! \brief Generate interrupt when Pswitch goes low
    HW_POWER_CHECK_PSWITCH_LOW          = 0
} hw_power_InterruptPolarity_t;

//! \brief Possible sources for Pswitch IRQ source
typedef enum _hw_power_PswitchIrqSource_t
{
    //! \brief Use bit 0 for Pswitch source
    HW_POWER_STS_PSWITCH_BIT_0,
    //! \brief Use bit 1 for Pswitch source
    HW_POWER_STS_PSWITCH_BIT_1
} hw_power_PswitchIrqSource_t;

//! \brief Possible 5V detection methods
typedef enum _hw_power_5vDetection_t
{
    //! \brief Uninitialized detection method.
    HW_POWER_5V_UNINITIALIZED,
    //! \brief Use VBUSVALID comparator for detection
    HW_POWER_5V_VBUSVALID,
    //! \brief Use VDD5V_GT_VDDIO comparison for detection
    HW_POWER_5V_VDD5V_GT_VDDIO,
    //! \brief Uses VBUSVALID 5V detection, but does not rely on the interrupt.
    HW_POWER_5V_VBUSVALID_WITH_POLLING
} hw_power_5vDetection_t;

//! \brief Register settings to use as initialization flags
typedef enum _hw_power_InitFlagSettings_t
{
    //! \brief Value of speed sensor control when driver uninitialized.
    POWER_REGS_UNINITIALIZED = 0,
    //! \brief Valud of speed sensor control when driver is initialized.
    POWER_REGS_INITIALIZED = 2
} hw_power_InitFlagSettings_t;

//! \brief Brownout offset settings
typedef enum _hw_power_BrownoutOffset_t
{
    //! \brief 0 mV offset from target
    BO_OFFSET_0MV   = 0,
    //! \brief 25 mV offset from target
    BO_OFFSET_25MV  = 1,
    //! \brief 50 mV offset from target
    BO_OFFSET_50MV  = 2,
    //! \brief 75 mV offset from target
    BO_OFFSET_75MV  = 3,
    //! \brief 100 mV offset from target
    BO_OFFSET_100MV = 4,
    //! \brief 125 mV offset from target
    BO_OFFSET_125MV = 5,
    //! \brief 150 mV offset from target
    BO_OFFSET_150MV = 6,
    //! \brief 175 mV offset from target
    BO_OFFSET_175MV = 7,


    //! \brief 0 mV offset from target
    BO_OFFSET_MIN = BO_OFFSET_0MV,
    //! \brief 0 mV offset from target
    BO_OFFSET_MAX = BO_OFFSET_175MV,
} hw_power_BrownoutOffset_t;

//! \brief Available dropout margins.
typedef enum _hw_power_4p2DropoutMargin_t
{
    //! \brief 25 mV drop margin
    DROPOUT_MARGIN_25,
    //! \brief 50 mV drop margin
    DROPOUT_MARGIN_50,
    //! \brief 100 mV drop margin
    DROPOUT_MARGIN_100,
    //! \brief 200 mV drop margin
    DROPOUT_MARGIN_200,
} hw_power_4p2DropoutMargin_t;

//! \brief Available configurations for 4p2 DCDC input.
typedef enum _hw_power_4p2SourceSelect_t
{
    //! \brief Always use the DCDC4P2 input regardless of the battery voltage.
    DCDC4P2_ALWAYS,
    //! \brief Use the DCDC4P2 input only when it is greater than the DCDC_BATT
    //! voltage and leave DCDC_BATT input unpowered.  Otherwise power on and use
    //! the DCDC_BATT input.
    DCDC4P2_GT_DCDCBATT,
    //! Use the source with the
    //! greater voltage reading.  Leave the DCDC_BATT input powered
    //! at all times.
    DCDC4P2_OR_DCDCBATT
} hw_power_4p2SourceSelect_t;

//! \brief Available DCDC4P2 and battery comparison trip points.
typedef enum _hw_power_4p2CmpTripPoints_t
{
    //! \brief Compare DCDC4P2 voltage with 85% of battery voltage.
    CMPTRIP_85PCT,
    //! \brief Compare DCDC4P2 voltage with 86% of battery voltage.
    CMPTRIP_86PCT,
    //! \brief Compare DCDC4P2 voltage with 100% of battery voltage.
    CMPTRIP_100PCT,
    //! \brief Compare DCDC4P2 voltage with 105% of battery voltage.
    CMPTRIP_105PCT
} hw_power_4p2CmpTripPoints_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! \brief Initializes the hw power block and battery charger
//!
//! \fntype Function
//!
//! (Description here)
//!
//!
//! \return Status of the call.
//! \retval SUCCESS                            Call was successful.
//! \retval ERROR_HW_DCDC_INVALID_INPUT_PARAM  Bad input parameter value
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_Init(void);


RtStatus_t hw_power_InitBatteryMonitor(hw_lradc_DelayTrigger_t eTrigger,
	 uint32_t u32SamplingInterval);
RtStatus_t hw_power_InitPowerSupplies(void);
RtStatus_t hw_power_InitFiq(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets new VDDD
//!
//! \fntype Function
//!
//! This function converts the millivolt value passed in to a register
//! setting and writes it to the target register.  This function will also
//! update the DCDC efficiency register field(s) related to Vddd.
//!
//! \param[in] 	 u16Vddd_mV - new voltage level in mV
//!
//! \notes This function does not adjust for LinReg offsets
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVdddValue(uint16_t u16Vddd_mV);

////////////////////////////////////////////////////////////////////////////////
//! \brief returns the current VDDD voltage
//!
//! \fntype Function
//!
//! This function converts the register setting for the target and converts
//! it to a millivolt value. The millivolt value is returned.
//!
//! \retval voltage of VDDD in mV
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVdddValue(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets new VDDD brownout voltage
//!
//! \fntype Function
//!
//! This function converts the brownout offset (as a millivolt value)
//! to a register setting and writes it to the brownout offset field.
//!
//! \param[in] 	 u16VdddBoOffset_mV - offset from target to set brownout
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVdddBrownoutValue(uint16_t u16VdddBoOffset_mV);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns the current VDDD brownout offset
//!
//! \fntype Function
//!
//! This function converts the register setting for the brownout offset and
//! converts it to a millivolt value. The millivolt value is returned.
//!
//! \retval current VDDD brownout offset
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVdddBrownoutValue(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the VDDD power source
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_SetVdddPowerSource(hw_power_PowerSource_t eSource);

////////////////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Returns the power source of VDDD
//!
//! \param[out]
//!
//! \retval HW_POWER_LINREG_DCDC_OFF - VDDD powered by linreg
//! \retval HW_POWER_DCDC_LINREG_OFF - VDDD powered by DCDC
////////////////////////////////////////////////////////////////////////////////
hw_power_PowerSource_t hw_power_GetVdddPowerSource(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets new VDDIO
//!
//! \fntype Function
//!
//  This function converts the millivolt value passed in to a register
//  setting and writes it to the target register.  This function will also
//  update the DCDC efficiency register field(s) related to Vddio.
//!
//! \param[in] 	 u16Vddio_mV - new VDDIO in mV
//  Notes: This function does not adjust for LinReg offsets
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddioValue(uint16_t u16Vddio_mV);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns current VDDIO voltage
//!
//! \fntype Function
//!
//  This function converts the register setting for the target and converts
//  it to a millivolt value. The millivolt value is returned.
//!
//! \param[in] 	 void
//!
//! \param[out]
//!
//! \retval Current VDDIO voltage in mV
//!
//! \notes None //  Notes: This function does not adjust for LinReg offsets
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVddioValue(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets new VDDIO brownout offset trigger
//!
//! \fntype Function
//!
//  This function converts the brownout offset (as a millivolt value)
//  to a register setting and writes it to the brownout offset field.
//!
//! \param[in] 	 u16VddioBoOffset_mV - offset in mV
//!
//! \param[out]
//!
//! \retval
//!
//! \notes This function does not adjust for LinReg offsets
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddioBrownoutValue(uint16_t u16VddioBoOffset_mV);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns current VDDIO brownout offset
//!
//! \fntype Function
//!
//! //  This function converts the register setting for the brownout offset and
//  converts it to a millivolt value. The millivolt value is returned.
//!
//! \param[in] 	 void
//!
//! \param[out]
//!
//! \retval Current brownout offset in mV
//!
//! \notes This function does not adjust for LinReg offsets
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVddioBrownoutValue(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the VDDIO power source
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_SetVddioPowerSource(hw_power_PowerSource_t eSource);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns VDDIO power source
//!
//! \fntype Function
//!
//! Returns the power source of VDDIO
//!
//! \param[out]
//!
//! \retval HW_POWER_LINREG_DCDC_OFF - VDDIO powered by linreg
//! \retval HW_POWER_DCDC_LINREG_OFF - VDDIO powered by DCDC
////////////////////////////////////////////////////////////////////////////////
hw_power_PowerSource_t hw_power_GetVddioPowerSource(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets new VDDA
//!
//! \fntype Function
//!
//! This function converts the millivolt value passed in to a register
//  setting and writes it to the target register.  This function will also
//  update the DCDC efficiency register field(s) related to Vdda.
//!
//! \param[in] 	 u16Vdda_mV - new VDDA in mV
//!
//! \param[out]
//!
//! \retval
//!
//! \notes This function does not adjust for LinReg offsets
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddaValue(uint16_t u16Vdda_mV);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns current VDDA
//!
//! \fntype Function
//!
//! This function converts the register setting for the target and converts
//  it to a millivolt value. The millivolt value is returned.
//!
//! \param[in] 	 void
//!
//! \param[out]
//!
//! \retval Current VDDA in mV
//!
//! \notes This function does not adjust for LinReg offsets
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVddaValue(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets new VDDA brownout offset
//!
//! \fntype Function
//!
//! This function converts the brownout offset (as a millivolt value)
//  to a register setting and writes it to the brownout offset field.
//!
//! \param[in] 	 u16VddaBoOffset_mV - new VDDA brownout in mV
//!
//! \notes This function does not adjust for LinReg offsets
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddaBrownoutValue(uint16_t u16VddaBoOffset_mV);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns current VDDA brownout
//!
//! \fntype Function
//!
//! This function converts the register setting for the brownout offset and
//  converts it to a millivolt value. The millivolt value is returned.
//!
//! \param[in] 	 void
//!
//! \param[out]
//!
//! \retval
//!
//! \notes This function does not adjust for LinReg offsets
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetVddaBrownoutValue(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the VDDA power source
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_SetVddaPowerSource(hw_power_PowerSource_t eSource);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns VDDA power source
//!
//! \fntype Function
//!
//! Returns the power source of VDDA
//!
//! \param[out]
//!
//! \retval HW_POWER_LINREG_DCDC_OFF - VDDA powered by linreg
//! \retval HW_POWER_DCDC_LINREG_OFF - VDDA powered by DCDC
////////////////////////////////////////////////////////////////////////////////
hw_power_PowerSource_t hw_power_GetVddaPowerSource(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Updates the DCDC efficincy register related to VDDD
//!
//! \fntype Function
//!
//! Writes the present VDDD supply voltage to the HW_POWER_DCFUNCV register's
//! VDDD field.  This provides information used to optimize DCDC converter
//! performance
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_UpdateDcFuncvVddd(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Updates the DCDC efficincy register related to VDDIO
//!
//! \fntype Function
//!
//! Writes the present VDDIO supply voltage to the HW_POWER_DCFUNCV register's
//! VDDIO field.  This provides information used to optimize DCDC converter
//! performance
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_UpdateDcFuncvVddio(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Disables the VDDIO linreg
//!
//! \fntype Function
//!
//! To disable the Vddio we set the current limit
//  of the Vddio linreg to zero.  So if bDisable is TRUE
//  we want the current limit to zero to be TRUE also.
//!
//! \param[in] 	 bDisable - TRUE to disable
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVddioLinearRegulator(bool bDisable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Disables the VDDA linreg
//!
//! \fntype Function
//!
//! To disable the VDDA Linreg, we clear the ENABLE_LINREG bit for VDDA
//!
//! \param[in] 	 bDisable - TRUE to disable
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVddaLinearRegulator(bool bDisable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Disables the VDDD linreg
//!
//! \fntype Function
//!
//! To disable the VDDD Linreg, we clear the ENABLE_LINREG bit for VDDD
//!
//! \param[in] 	 bDisable - TRUE to disable
//!/////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVdddLinearRegulator(bool bDisable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Disable all Linear Regulators
//!
//! \fntype Function
//!
//! Disables the VDDD, VDDA, and VDDIO linear regulators
//!
//! \param[in] 	 bDisable - TRUE to disable
//!
//! \notes None
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_Disable5vLinearRegulators(bool bDisable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable detection of 5V presense
//!
//! \fntype Function
//!
//! Enable hardware to check for VBUS Valid signal which is set high
//! when 5V is connected
//!
//! \retval SUCCESS - 5V detection enabled successfully
//!
//! \notes None
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_Enable5vDetection(hw_power_5vDetection_t eDetection);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the detection method to use for DCDC control logic.
//!
//! \fntype Function
//!
//! Selects the 5V detection method for DCDC control logic.  Use one of the
//! enumerated values from hw_power_5vDetection_t.
//!
//! \param[in] eDetection Selected 5V detection method.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetDcdc5vDetect( hw_power_5vDetection_t eDetection );

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the detection method to use for software 5V detection.
//!
//! \fntype Function
//!
//! Selects the 5V detection method for software.  Use one of the
//! enumerated values from hw_power_5vDetection_t.
//!
//! \param[in] eDetection Selected 5V detection method.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetSw5vDetect( hw_power_5vDetection_t eDetection );

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables the VBUSVALID 5V detection hardware.
//!
//! \fntype Function
//!
//! Enables or powers any hardware or components required to use VBUSVALID
//! 5V detection method.
//!
//! \param[in] True to enable, false to disable.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableVbusValid5vDetect( bool bEnable );

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables the USB 5V detection hardware.
//!
//! \fntype Function
//!
//! Enables or power any hardware components required to use USB 5V detection
//! method.
//!
//! \param[in] True to enable, false to disable.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableUsb5vDetect( bool bEnable );

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables the VDD5V_GT_VDDIO detection hardware.
//!
//! \fntype Function
//!
//! Enables or powers any hardware components required to use the
//! VDD5V_GT_VDDIO detection method.
//!
//! \param[in] True to enable, false to disable.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableVdd5vGtVddio5vDetect( bool bEnable );

////////////////////////////////////////////////////////////////////////////////
//! \brief Saves the detection method for software
//!
//! \param[in] eDetection One of the enumerations from hw_power_5vDetection_t
//! specifies which 5V detection method to use.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetDetectionMethod( hw_power_5vDetection_t eDetection );

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns the detection method for software
//!
//! \retval The current 5V detection method.
////////////////////////////////////////////////////////////////////////////////
hw_power_5vDetection_t hw_power_GetDetectionMethod( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief 5V interrupt triggers on 5V insertion
//!
//! \fntype Function
//!
//! Sets the interrupt polarity to check for 5V inserted
//!
//! \param[in] 	 bEnable - TRUE to enable 5V insertion detection
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_Enable5vPlugInDetect(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief 5V interrupt triggers on 5V removal
//!
//! \fntype Function
//!
//! Sets the interrupt polarity to check for 5V removed
//!
//! \param[in] 	 bEnable - TRUE to enable 5V removal detection
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_Enable5vUnplugDetect(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief returns the status of the DCDC voltage change.
//!
//! \fntype Function
//!
//! Checks the status of the DC_OK bit.
//!
//! \retval TRUE - DCDC has completed the voltage change
//! \retval FALSE - DCDC still tranferring to target voltage
//!
////////////////////////////////////////////////////////////////////////////////
bool hw_power_CheckDcdcTransitionDone(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Allow DCDC to be enabled while 5V is connected.
//!
//! \fntype Function
//!
//! Normally, the DCDC will turn off when a 5V connection is established.
//! This function will allow the DCDC to remain on while 5V is connected
//! to manage high power loads.
//!
//! \param[in] 	 bEnable - TRUE to enable
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableDcdcDuring5vConnection(bool bEnable);


////////////////////////////////////////////////////////////////////////////////
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVdddDcdcFet(bool bDisable);

////////////////////////////////////////////////////////////////////////////////
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVddaDcdcFet(bool bDisable);

////////////////////////////////////////////////////////////////////////////////
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableVddioDcdcFet(bool bDisable);


////////////////////////////////////////////////////////////////////////////////
// Battery
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! \brief Writes brownout register code to register.
//!
//! \fntype Function
//!
//! \param[in] BoRegCode Register code for battery brownout field.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetBatteryBrownoutCode( uint16_t BoRegCode );

////////////////////////////////////////////////////////////////////////////////
//! \brief Reads the brownout register code.
//!
//! \fntype Function
//!
//! \retval Register code for the battery brownout field.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetBatteryBrownoutCode( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Use hw_power_SetBatteryBrownoutVoltage.
//!
//! \fntype Function
//!
//! Deprecated function.  Calls hw_power_SetBatteryBrownoutVoltage().
//!
//! \param[in] 	 i16BatteryBrownout_mV - new battery brownout voltage in mV
//!
//! \retval SUCCESS - New battery brownout voltage has been set
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_SetBatteryBrownoutValue(int16_t i16BatteryBrownout_mV);

////////////////////////////////////////////////////////////////////////////////
//! \brief Use hw_power_GetBatteryBrownoutVoltage.
//!
//! \fntype Function
//!
//! Deprecated function.  Calls hw_power_GetBatteryBrownoutVoltage().
//!
//! \retval Battery brownout in mV
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetBatteryBrownoutValue(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets new battery brownout voltage level
//!
//! \fntype Function
//!
//! \param[in] BoVolt New battery brownout voltage (mV)
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetBatteryBrownoutVoltage( uint16_t BoVolt );

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns current brownout voltage in mV
//!
//! \fntype Function
//!
//! \retval Battery brownout in mV
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetBatteryBrownoutVoltage(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Current battery voltage in mV
//!
//! \fntype Function
//!
//! \retval Current battery voltage
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetBatteryVoltage(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief API to set battery monitor voltage.
//!
//! \fntype Function
//!
//! Hardware will automatically set the battery monitor voltage when the battery
//! charger is set up.  This is for debugging purposes. Normally, this is
//! automatically set by hardware.
//!
//! \param[in] 	 u16BattVolt - new battery monitor voltage
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetBatteryMonitorVoltage(uint16_t u16BattVolt);


////////////////////////////////////////////////////////////////////////////////
//! \brief Returns maximum battery charge current
//!
//! \fntype Function
//!
//! \retval Maximum battery charge current in mA
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetMaxBatteryChargeCurrent(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Minimum current that will stop the batter charging current.
//!
//! \fntype Function
//!
//! Set the minimum current that will stop the batter charging current.  When
//! the charing current reaches this value, the charger will turn off.
//! Note that the hardware has a minimum resolution of 10mA and a maximum
//! expressible value of 180mA (see the data sheet for details). If the given
//! current cannot be expressed exactly, then the largest expressible smaller
//! value will be used. The return reports the actual value that was effected.
//!
//! \param[in] 	 u16Threshold - requested threshold current in mA.
//!
//! \retval Actual threshold current
//!
//! \notes None
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t  hw_power_SetBatteryChargeCurrentThreshold(uint16_t u16Threshold);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns battery charge current threshold
//!
//! \fntype Function
//!
//! \retval Threshold current in mA
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetBatteryChargeCurrentThreshold(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable Pswitch interrupt
//!
//! \fntype Function
//!
//! Changes the Pswitch interrupt enable status.  The previous value of the
//! setting is returned to allow a quick disable and restore operation.
//!
//! \param[in] bEnable True to enable the interrupt, false to disable.
//!
//! \retval Previous setting of interrupt.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnablePswitchInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears the Pswitch interrupt status bit
//!
//! \fntype Function
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_ClearPswitchInterrupt(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Selects bit 0 or bit 1 for Pswitch interrupt source
//!
//! \fntype Function
//!
//! In HW_POWER_STS register field PSWITCH, bit 0 or 1
//! (bit 18 or 19 in register respectively),
//! can used to to trigger the Pswitch interrupt
//!
//! \param[in] 	 bSource - HW_POWER_STS_PSWITCH_BIT_0 _1
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetPswitchInterruptSource(bool bSource);

////////////////////////////////////////////////////////////////////////////////
//! \brief Select Pswitch low or high to trigger interrupt
//!
//! \fntype Function
//!
//! Pswitch interrupts can be triggered when Pswitch goes high or low.  This bit
//! selects the polarity.
//!
//! \param[in] 	 bPolarity - HW_POWER_CHECK_PSWITCH_LOW _HIGH
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetPswitchInterruptPolarity(bool bPolarity);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns polarity of Pswitch interrupt
//!
//! \fntype Function
//!
//! \retval - HW_POWER_CHECK_PSWITCH_LOW - when Pswitch is low
//! \retval - HW_POWER_CHECK_PSWITCH_HIGH - when Pswitch is high
//!
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetPswitchInterruptPolarity(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable LinReg okay interrupt
//!
//! \fntype Function
//!
//! Changes the LinReg OK interrupt enable status.  The previous value of the
//! setting is returned to allow a quick disable and restore operation.
//!
//! \param[in] bEnable True to enable the interrupt, false to disable.
//!
//! \retval Previous setting of interrupt.
bool hw_power_EnableLinregOkInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears the LinReg interrupt status bit
//!
//! \fntype Function
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_ClearLinregOkInterrupt(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Set LinReg interrupt polarity
//!
//! \fntype Function
//!
//! (Description here)
//!
//! \param[in] 	 bPolarity - HW_POWER_CHECK_5V_DISCONNECTED or
//!                          HW_POWER_CHECK_LINEAR_REGULATORS_OK
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetLinregOkInterruptPolarity(bool bPolarity);

////////////////////////////////////////////////////////////////////////////////
//! \brief Return polarity for LinReg interrupt
//!
//! \fntype Function
//!
//! \retval HW_POWER_CHECK_5V_DISCONNECTED - trigger when 5V disconnected
//! \retval HW_POWER_CHECK_LINEAR_REGULATORS_OK - trigger when LinRegs OK
//!
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetLinregOkInterruptPolarity(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable DC OK interrupt
//!
//! \fntype Function
//!
//! \param[in] 	 bEnable - TRUE to enable
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableDcOkInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears the DC OK interrupt status bit
//!
//! \fntype Function
////////////////////////////////////////////////////////////////////////////////
void hw_power_ClearDcOkInterrupt(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable battery interrupt
//!
//! \fntype Function
//!
//! Changes the battery interrupt enable status.  The previous value of the
//! setting is returned to allow a quick disable and restore operation.
//!
//! \param[in] bEnable True to enable the interrupt, false to disable.
//!
//! \retval Previous setting of interrupt.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableBatteryBrownoutInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears the battery interrupt status bit
//!
//! \fntype Function
////////////////////////////////////////////////////////////////////////////////
void hw_power_ClearBatteryBrownoutInterrupt(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable VDDIO brownout interrupt
//!
//! \fntype Function
//!
//! Changes the VDDIO brownout interrupt enable status.The previous value of the
//! setting is returned to allow a quick disable and restore operation.
//!
//! \param[in] bEnable True to enable the interrupt, false to disable.
//!
//! \retval Previous setting of interrupt.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableVddioBrownoutInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears the Vddio interrupt status bit
//!
//! \fntype Function
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_ClearVddioBrownoutInterrupt(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable VDDA brownout interrupt
//!
//! \fntype Function
//!
//! Changes the VDDA brownout interrupt enable status. The previous value of the
//! setting is returned to allow a quick disable and restore operation.
//!
//! \param[in] bEnable True to enable the interrupt, false to disable.
//!
//! \retval Previous setting of interrupt.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableVddaBrownoutInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//!
//! Name: hw_power_ClearVddaBrownoutInterrupt
//!
//! \brief Clears the Vdda interrupt status bit
//!
//! \fntype Function
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_ClearVddaBrownoutInterrupt(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable VDDD brownout interrupt
//!
//! \fntype Function
//!
//! Changes the VDDD brownout interrupt enable status. The previous value of the
//! setting is returned to allow a quick disable and restore operation.
//!
//! \param[in] bEnable True to enable the interrupt, false to disable.
//!
//! \retval Previous setting of interrupt.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableVdddBrownoutInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears the Vddd interrupt status bit
//!
//! \fntype Function
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_ClearVdddBrownoutInterrupt(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable VBUS valid interrupt
//!
//! \fntype Function
//!
//! Changes the VBUS valid interrupt enable status.  The previous value of the
//! setting is returned to allow a quick disable and restore operation.
//!
//! \param[in] bEnable True to enable the interrupt, false to disable.
//!
//! \retval Previous setting of interrupt.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableVbusValidInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears the VbusValid interrupt status bit
//!
//! \fntype Function
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_ClearVbusValidInterrupt(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Return polarity for VbusValid interrupt
//!
//! \retval HW_POWER_CHECK_5V_DISCONNECTED - trigger when 5V disconnected
//! \retval HW_POWER_CHECK_5V_CONNECTED - trigger when 5V connected
//!
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetVbusValidInterruptPolarity(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Set the VbusValid interrupt polarity
//!
//! \fntype Function
//!
//! \param[in] 	 bPolarity - HW_POWER_CHECK_5V_DISCONNECTED or
//!                          HW_POWER_CHECK_5V_CONNECTED
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVbusValidInterruptPolarity(bool bPolarity);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable VDD5V_GT_VDDIO interrupt
//!
//! \fntype Function
//!
//! Changes the VDD5V_GT_VDDIO interrupt enable status.The previous value of the
//! setting is returned to allow a quick disable and restore operation.
//!
//! \param[in] bEnable True to enable the interrupt, false to disable.
//!
//! \retval Previous setting of interrupt.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableVdd5vGtVddioInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears the VDD5V_GT_VDDIO interrupt status bit
//!
//! \fntype Function
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_ClearVdd5vGtVddioInterrupt(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Set the VDD5V_GT_VDDIO interrupt polarity
//!
//! \fntype Function
//!
//! \param[in] 	 bPolarity - HW_POWER_CHECK_5V_DISCONNECTED or
//!                          HW_POWER_CHECK_5V_CONNECTED
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVdd5vGtVddioInterruptPolarity(bool bPolarity);

////////////////////////////////////////////////////////////////////////////////
//! \brief Return polarity for VDD5V_GT_VDDIO interrupt
//!
//! \retval HW_POWER_CHECK_5V_DISCONNECTED - trigger when 5V disconnected
//! \retval HW_POWER_CHECK_5V_CONNECTED - trigger when 5V connected
//!
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetVdd5vGtVddioInterruptPolarity(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable 4P2 brownout interrupt
//!
//! \fntype Function
//!
//! Changes the 4P2 brownout interrupt enable status.  The previous value of the
//! setting is returned to allow a quick disable and restore operation.
//!
//! \param[in] bEnable True to enable the interrupt, false to disable.
//!
//! \retval Previous setting of interrupt.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_Enable4p2BrownoutInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears the 4.2V rail interrupt status bit
//!
//! \fntype Function
////////////////////////////////////////////////////////////////////////////////
void hw_power_Clear4p2BrownoutInterrupt(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable the 5V droop interrupt
//!
//! \fntype Function
//!
//! Changes the VDD5V droop interrupt enable status.  The previous value of the
//! setting is returned to allow a quick disable and restore operation.
//!
//! \param[in] bEnable True to enable the interrupt, false to disable.
//!
//! \retval Previous setting of interrupt.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableVdd5vDroopInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears the VDD5V droop interrupt status bit
//!
//! \fntype Function
////////////////////////////////////////////////////////////////////////////////
void hw_power_ClearVdd5vDroopInterrupt(void);

///////////////////////////////////////////////////////////////////////////////
//! \brief Sets/Clears CLKGATE bit in POWER_CTRL
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetPowerClkGate(bool bGate);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns CLKGATE bit in POWER_CTRL
//!
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetPowerClkGate(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets/Clears VBUSVALID_5VDETECT bit in POWER_5VCTRL
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVbusValidThresh(hw_power_VbusValidThresh_t eThresh);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns value of VBUSVALID_THRSH field.
//!
////////////////////////////////////////////////////////////////////////////////
hw_power_VbusValidThresh_t hw_power_GetVbusValidThresh( void );

void hw_power_SetVbusDroopThresh( hw_power_VbusDroopThresh_t eThresh );
hw_power_VbusDroopThresh_t hw_power_GetVbusDroopThresh( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears PWDN_BATTBRNOUT bit in POWER_BATTMONITOR
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisableBrownoutPowerdown(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Clears PWDN_5VBRNOUT bit in POWER_5VCTRL
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_Disable5vBrownoutPowerdown(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets/Clears ENABLE_DCDC bit in POWER_5VCTRL
//!
//! \param[in] bEnable - TRUE to set, FALSE to clear
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableDcdc(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns ENABLE_DCDC in POWER_5VCTRL
//!
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetEnableDcdc(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets/Clears DCDC_XFER bit in POWER_5VCTRL
//!
//! \param[in] bEnable - TRUE to set, FALSE to clear
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableAutoDcdcTransfer(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets/Clears ENABLE_ILIMIT bit in POWER_5VCTRL
//!
//! \param[in] bEnable - TRUE to set, FALSE to clear
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableCurrentLimit(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Control the battery charge circuit
//!
//! \fntype Function
//!
//! Powers down or powers up the battery charge circuit.  The circuit should
//! only be powered up when 5V is present.
//!
//! \param[in] 	 bPowerDown - TRUE to power down, FALSE to power up
//!
//! \notes The bit is a power down bit so setting the bit
//! \notes TRUE will power down the circuit
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetPowerDownBatteryCharger(bool bPowerDown);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the linear regulator offset from target voltage
//!
//! \fntype Function
//!
//! The linear regulator for each rail can be set equal to or 25 mV above or below
//! the target voltage.  This function sets the new value of the offset.
//!
//! \param[in] eOffset - HW_POWER_LINREG_OFFSET_NO_STEP
//!				LinReg output = target voltage
//!                      HW_POWER_LINREG_OFFSET_STEP_ABOVE
//!				LinReg output = target voltage + 25mV
//!                      HW_POWER_LINREG_OFFSET_STEP_BELOW
//!				LinReg output = target voltage - 25mV
//!                      HW_POWER_LINREG_OFFSET_MAX
//!			        LinReg output = target voltage - 25mV
//!
//! \notes None
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVdddLinRegOffset(hw_power_LinRegOffsetStep_t eOffset);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the linear regulator offset from target voltage
//!
//! \fntype Function
//!
//! The linear regulator for each rail can be set equal to or 25 mV above or below
//! the target voltage.  This function sets the new value of the offset.
//!
//! \param[in] eOffset - HW_POWER_LINREG_OFFSET_NO_STEP
//!				LinReg output = target voltage
//!                      HW_POWER_LINREG_OFFSET_STEP_ABOVE
//!				LinReg output = target voltage + 25mV
//!                      HW_POWER_LINREG_OFFSET_STEP_BELOW
//!				LinReg output = target voltage - 25mV
//!                      HW_POWER_LINREG_OFFSET_MAX
//!			        LinReg output = target voltage - 25mV
//!
//! \notes None
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddaLinRegOffset(hw_power_LinRegOffsetStep_t eOffset);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the linear regulator offset from target voltage
//!
//! \fntype Function
//!
//! The linear regulator for each rail can be set equal to or 25 mV above or below
//! the target voltage.  This function sets the new value of the offset.
//!
//! \param[in] eOffset - HW_POWER_LINREG_OFFSET_NO_STEP
//!				LinReg output = target voltage
//!                      HW_POWER_LINREG_OFFSET_STEP_ABOVE
//!				LinReg output = target voltage + 25mV
//!                      HW_POWER_LINREG_OFFSET_STEP_BELOW
//!				LinReg output = target voltage - 25mV
//!                      HW_POWER_LINREG_OFFSET_MAX
//!			        LinReg output = target voltage - 25mV
//!
//! \notes None
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetVddioLinRegOffset(hw_power_LinRegOffsetStep_t eOffset);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns offset from target voltage that the LinReg is currently set to
//!
//! \fntype Function
//!
//! The linear regulator for each rail can be set equal to or 25 mV above or below
//! the target voltage.  This function returns the current value of the offset.
//!
//! \param[in] eOffset - HW_POWER_LINREG_OFFSET_NO_STEP
//!				LinReg output = target voltage
//!                      HW_POWER_LINREG_OFFSET_STEP_ABOVE
//!				LinReg output = target voltage + 25mV
//!                      HW_POWER_LINREG_OFFSET_STEP_BELOW
//!				LinReg output = target voltage - 25mV
//!                      HW_POWER_LINREG_OFFSET_MAX
//!			        LinReg output = target voltage - 25mV
//!
//! \notes None
//!
////////////////////////////////////////////////////////////////////////////////
hw_power_LinRegOffsetStep_t hw_power_GetVdddLinRegOffset(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns offset from target voltage that the LinReg is currently set to
//!
//! \fntype Function
//!
//! The linear regulator for each rail can be set equal to or 25 mV above or below
//! the target voltage.  This function returns the current value of the offset.
//!
//! \param[in] eOffset - HW_POWER_LINREG_OFFSET_NO_STEP
//!				LinReg output = target voltage
//!                      HW_POWER_LINREG_OFFSET_STEP_ABOVE
//!				LinReg output = target voltage + 25mV
//!                      HW_POWER_LINREG_OFFSET_STEP_BELOW
//!				LinReg output = target voltage - 25mV
//!                      HW_POWER_LINREG_OFFSET_MAX
//!			        LinReg output = target voltage - 25mV
//!
//! \notes None
//!
////////////////////////////////////////////////////////////////////////////////
hw_power_LinRegOffsetStep_t hw_power_GetVddaLinRegOffset(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns offset from target voltage that the LinReg is currently set to
//!
//! \fntype Function
//!
//! The linear regulator for each rail can be set equal to or 25 mV above or below
//! the target voltage.  This function returns the current value of the offset.
//!
//! \param[in] eOffset - HW_POWER_LINREG_OFFSET_NO_STEP
//!				LinReg output = target voltage
//!                      HW_POWER_LINREG_OFFSET_STEP_ABOVE
//!				LinReg output = target voltage + 25mV
//!                      HW_POWER_LINREG_OFFSET_STEP_BELOW
//!				LinReg output = target voltage - 25mV
//!                      HW_POWER_LINREG_OFFSET_MAX
//!			        LinReg output = target voltage - 25mV
//!
//! \notes None
//!
////////////////////////////////////////////////////////////////////////////////
hw_power_LinRegOffsetStep_t hw_power_GetVddioLinRegOffset(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Change the DCDC converter response time
//!
//! \fntype Function
//!
//! Enable analog circuit of DCDC converter to respond faster under
//! transient load conditions.
//!
//! \param[in] 	 eLevel - HW_POWER_RCSCALE_DISABLED - normal response
//!                       HW_POWER_RCSCALE_2X_INCR  - 2 times faster response
//!                       HW_POWER_RCSCALE_4X_INCR  - 4 times faster response
//!                       HW_POWER_RCSCALE_8X_INCR  - 8 times faster response
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableRcScale(hw_power_RcScaleLevels_t eLevel);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns status of 5V presence
//!
//! \fntype Function
//!
//! Returns TRUE if 5V is detected.
//!
//! \retval - TRUE if 5V present
//!
////////////////////////////////////////////////////////////////////////////////
bool hw_power_Get5vPresentFlag(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns 5V status.
//!
//! \fntype Function
//!
//! \retval True 5V is present.
//! \retval False 5V is not present.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_Is5vPresent( void );


////////////////////////////////////////////////////////////////////////////////
//! \brief Powers down the chip
//!
//! \fntype Function
//!
//! This function will set the PWD in the HW_POWER_RESET register.
//! The chip will be powered down.
//!
//! \notes None
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_PowerDown(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Disable the chip powerdown bit
//!
//! \fntype Function
//!
//! Optional bit to disable all paths to power off the chip except the watchdog
//! timer. Setting this bit will be useful for preventing fast falling edges
//! on the PSWITCH pin from resetting the chip. It may also be useful increasing
//! system tolerance of noisy EMI environments.
//!
//! \param[in] 	 bDisable - TRUE to disable
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_DisablePowerDown(bool bDisable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Convert a current in mA to a hardware setting.
//!
//! \fntype Function
//!
//! This function converts a current measurement in mA to a hardware setting
//! used by HW_POWER_BATTCHRG.STOP_ILIMIT or HW_POWER_BATTCHRG.BATTCHRG_I.
//!
//! Note that the hardware has a minimum resolution of 10mA and a maximum
//! expressible value of 780mA (see the data sheet for details). If the given
//! current cannot be expressed exactly, then the largest expressible smaller
//! value will be used.
//!
//! \param[in]  u16Current  The current of interest.
//!
//! \retval  The corresponding setting.
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t  hw_power_ConvertCurrentToSetting(uint16_t u16Current);

////////////////////////////////////////////////////////////////////////////////
//! \brief Convert a hardware current setting to a value in mA.
//!
//! \fntype Function
//!
//! This function converts a setting used by HW_POWER_BATTCHRG.STOP_ILIMIT or
//! HW_POWER_BATTCHRG.BATTCHRG_I into an actual current measurement in mA.
//!
//! Note that the hardware current fields are 6 bits wide. The higher bits in
//! the 8-bit input parameter are ignored.
//!
//! \param[in]  u8Setting  A hardware current setting.
//!
//! \retval  The corresponding current in mA.
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t  hw_power_ConvertSettingToCurrent(uint16_t u16Setting);

////////////////////////////////////////////////////////////////////////////////
//! \brief Convert a mV value to the corresponding VDDD voltage setting
//!
//! \fntype Function
//!
//! This function converts the mV value passed in to the VDDD voltage setting to
//! be used in HW_POWER_VDDDCTRL.B.TRG.
//!
//! Note that the hardware target voltage fields are 5 bits wide. The higher
//! bits are ignored.
//!
//! \param[in]  u16Vddd  A hardware voltage setting for VDDD.
//!
//! \retval  The corresponding VDDD register setting.
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertVdddToSetting(uint16_t u16Vddd);

////////////////////////////////////////////////////////////////////////////////
//! \brief Convert a VDDD target register setting to the corresponding mV value
//!
//! \fntype Function
//!
//! This function converts the VDDD voltage setting that can be used in
//! HW_POWER_VDDDCTRL.B.TRG to the corresponding mV value
//!
//! Note that the hardware target voltage fields are 5 bits wide. The higher
//! bits are ignored.
//!
//! \param[in]  u16Vddd  A hardware voltage setting for VDDD.
//!
//! \retval  The corresponding VDDD voltage in mV.
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertSettingToVddd(uint16_t u16Setting);

////////////////////////////////////////////////////////////////////////////////
//! \brief Convert a mV value to the corresponding VDDA voltage setting
//!
//! \fntype Function
//!
//! This function converts the mV value passed in to the VDDA voltage setting to
//! be used in HW_POWER_VDDACTRL.B.TRG.
//!
//! Note that the hardware target voltage fields are 5 bits wide. The higher
//! bits are ignored.
//!
//! \param[in]  u16Vdda  A hardware voltage setting for VDDA.
//!
//! \retval  The corresponding VDDA register setting.
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertVddaToSetting(uint16_t u16Vdda);

////////////////////////////////////////////////////////////////////////////////
//! \brief Convert a VDDA target register setting to the corresponding mV value
//!
//! \fntype Function
//!
//! This function converts the VDDA voltage setting that can be used in
//! HW_POWER_VDDACTRL.B.TRG to the corresponding mV value
//!
//! Note that the hardware target voltage fields are 5 bits wide. The higher
//! bits are ignored.
//!
//! \param[in]  u16Vdda  A hardware voltage setting for VDDA.
//!
//! \retval  The corresponding VDDA voltage in mV.
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertSettingToVdda(uint16_t u16Setting);

////////////////////////////////////////////////////////////////////////////////
//! \brief Convert a mV value to the corresponding VDDIO voltage setting
//!
//! \fntype Function
//!
//! This function converts the mV value passed in to the VDDIO voltage setting to
//! be used in HW_POWER_VDDIOCTRL.B.TRG.
//!
//! Note that the hardware target voltage fields are 5 bits wide. The higher
//! bits are ignored.
//!
//! \param[in]  u16Vddio  A hardware voltage setting for VDDIO.
//!
//! \retval  The corresponding VDDIO register setting.
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertVddioToSetting(uint16_t u16Vddio);

////////////////////////////////////////////////////////////////////////////////
//! \brief Convert a VDDIO target register setting to the corresponding mV value
//!
//! \fntype Function
//!
//! This function converts the VDDIO voltage setting that can be used in
//! HW_POWER_VDDIOCTRL.B.TRG to the corresponding mV value
//!
//! Note that the hardware target voltage fields are 5 bits wide. The higher
//! bits are ignored.
//!
//! \param[in]  u16Vddio  A hardware voltage setting for VDDIO.
//!
//! \retval  The corresponding VDDIO voltage in mV.
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertSettingToVddio(uint16_t u16Setting);

////////////////////////////////////////////////////////////////////////////////
//! \brief Converts battery voltage to register setting.
//!
//! \fntype Function
//!
//! Converts the battery voltage in mV to the equivalent register setting.
//!
//! \param[in] u16Batt Battery voltage in milliVolts.
//! \param[in] u16BattMode LiIon or Alkaline battery mode
//!
//! \retval Register value for the given battery voltage.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertBattToSetting(uint16_t u16Batt, uint16_t u16BattMode);

////////////////////////////////////////////////////////////////////////////////
//! \brief Converts register setting to battery voltage.
//!
//! \fntype Function
//!
//! Converts the battery register setting to the equivalent battery voltage
//! in millivolts.
//!
//! \param[in] u16Setting Battery voltage register setting.
//! \param[in] u16BattMode LiIon or Alkaline battery mode.
//!
//! \retval Battery voltage for the given register value.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertSettingToBatt(uint16_t u16Setting,
	 uint16_t u16BattMode);

////////////////////////////////////////////////////////////////////////////////
//! \brief Converts battery brownout voltage to the register setting.
//!
//! \fntype Function
//!
//! Converts the battery brownout voltage in millivolts to the equivalent
//! battery register setting.
//!
//! \param[in] u16BattBo Battery brownout voltage in milliVolts.
//! \param[in] u16BattMode LiIon or Alkaline battery mode
//!
//! \retval Register value for the given battery brownout voltage.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertBattBoToSetting(uint16_t u16BattBo);

////////////////////////////////////////////////////////////////////////////////
//! \brief Converts register setting to battery brownout voltage..
//!
//! \fntype Function
//!
//! Converts the battery register setting to the equivalent battery voltage
//! in millivolts.
//!
//! \param[in] u16Setting Battery brownout voltage register setting.
//! \param[in] u16BattMode LiIon or Alkaline battery mode.
//!
//! \retval Battery brownout voltage for the given register value.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertSettingToBattBo(uint16_t u16Setting);

////////////////////////////////////////////////////////////////////////////////
//! \brief Converts 5V threshold voltage to register setting.
//!
//! \fntype Function
//!
//! Converts the 5V threshold in millivolts to the register setting that
//! best corresponds to the requested value.
//!
//! \param[in] u16ThreshVolt Voltage level considered valid for 5V detection.
//!
//! \retval Register setting that corresponds best to requested voltage.
////////////////////////////////////////////////////////////////////////////////
hw_power_VbusValidThresh_t hw_power_ConvertVoltToVbusThresh(
	uint16_t u16ThreshVolt );

/////////////////////////////////////////////////////////////////////////////////
//! \brief Converts VBUS threshold register setting to voltage.
//!
//! \fntype Function
//!
//! COnverts the VBUS threshold register setting to the equivalent voltage level
//! in millivolts.
//!
//! \param[in] eThresh Register setting to be converted to millivolts.
//!
//! \retval Voltage level in millivolts of requested register setting.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_ConvertVbusThreshToVolt( hw_power_VbusValidThresh_t eThresh );

////////////////////////////////////////////////////////////////////////////////
//! \brief Set the positive limit for the boost mode duty-cycle.
//!
//! \fntype Function
//!
//! This function sets the positive limit for the duty-cycle for
//! boost mode operation.  This limits the maximum time in the duty-cycle
//! any power supply can be charged in boost mode.
//!
//! \param[in] u16Limit Boost mode positive limit.
//!
//! \retval None.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetPosLimitBoost(uint16_t u16Limit);

////////////////////////////////////////////////////////////////////////////////
//! \brief Set the positive limit for the buck mode duty-cycle.
//!
//! \fntype Function
//!
//! This function sets the positive limit for the duty-cycle for
//! buck mode operation.  This limits the maximum time in the duty-cycle
//! any power supply can be charged in buck mode.
//!
//! \param[in] u16Limit Buck mode positive limit.
//!
//! \retval None.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetPosLimitBuck(uint16_t u16Limit);

////////////////////////////////////////////////////////////////////////////////
//! \brief Set the negative limit for the DCDC duty-cycle.
//!
//! \fntype Function
//!
//! This function sets the negative limit for the duty-cycle.  This limits the
//! time any rail does not get charged in the DCDC duty-cycle.
//!
//! \param[in] u16Limit Negative DCDC duty-cycle limit.
//!
//! \retval None.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetNegLimit(uint16_t u16Limit);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable double FETs for high load situations.
//!
//! \fntype Function
//!
//! \param[in] bEnable TRUE to enable.
//!
//! \retval None.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableDoubleFets(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable half FETs for low power situations.
//!
//! \fntype Function
//!
//! \param[in] bEnable TRUE to enable, FALSE to disable.
//!
//! \retval None.
////////////////////////////////////////////////////////////////////////////////
void hw_power_EnableHalfFets(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the ratio of integral control to proportional control for DCDC.
//!
//! \fntype Function
//!
//! \param[in] u16Value Ration of integral to proportional control
//!
//! \retval None.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetLoopCtrlDcC(uint16_t u16Value);

////////////////////////////////////////////////////////////////////////////////
//! \brief Brownout FIQ handler to check if brownouts interrupts are valid.
//!
//! \fntype FIQ handler
//!
//! Checks the brownout status bits and IRQ bits to  determine the validity of
//! the brownout IRQ.  If valid, this function shuts down the chip.  Otherwise,
//! it passes control back to the os_pmi_FiqHandler.
//!
//! \param[in] None.
//!
//! \retval TRUE The brownout is valid and it not a false brownout generated
//! by the supplies.  The PMI FIQ needs to handle it.
//! \retval FLSE The supplies generated a false brownout and the PMI FIQ
//! can ignore handling it.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_FiqHandler(void);

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
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetDcdcClkFreq(uint16_t u16Freq, bool bUsePll);

////////////////////////////////////////////////////////////////////////////////
//
//! \brief Returns the current polarity of the 5V interrupt.
//!
//! \fntype Function
//!
//! The 5V interrupt can be set to detect an insertion or removal of a 5V
//! signal.  This function returns the current setting in hardware.
//!
//! \return true if set to detect insertion event, false if set to detect
//! a removal event.
//!
////////////////////////////////////////////////////////////////////////////////
bool hw_power_Get5vInterruptPolarity(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief  Checks if USB VBUS is present
//!
//! \retval True if USB VBUS is present, false if not.
//!
//! \notes  For some applications VBUS can be 3V, so this function doesn't imply
//!         the presence of 5V. For 5V status use either
//!         hw_power_Get5vPresentFlag() or hw_power_GetVbusValid().
////////////////////////////////////////////////////////////////////////////////
bool hw_power_IsUsbVbusPresent(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns the VBUSVALID_STATUS value
//!
//! \retval True if 5V is present, false if 5V is not present.
//!
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetVbusValid(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns the VDD5V_GT_VDDIO status
//!
//! \retval True if 5V is present, false if 5V is not present.
//!
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetVdd5vGtVddio(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Set the maximum current used by 4p2 rails and the battery charger
//!
//! \fntype Function
//!
//! Set the maximum current limit that can be sourced from 5V to the battery
//! charger and the 4p2 rail.
//! The maximum current available is 780mA.  The current is limited to values
//! that can be created using 400, 200, 100, 50, 20, and 10 mA.  Each value is
//! represented by a bit.If the current value in the argument can't be achieved,
//! the closet value below the requested value will be used.
//!
//! \param[in] 	 u16Current - requested maximum battery charge current in mA
//!
//! \retval Actual maximum current that was set
//!
//! \notes We can't use the usual Clear/Set method of writing to this register
// field. If we did, then the current draw would drop to zero for a moment
// and then zoom back up to the new setting. That would be bad.
//
// Instead, we XOR the old and new settings, which gives us a mask where the
// set bits are the ones that need to change. We then write this mask to the
// toggle register and we're done.
//!
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_SetMax5vCurrentLimiterCurrent(uint16_t u16Current);

////////////////////////////////////////////////////////////////////////////////
//! \brief Initialize but don't start 4p2
////////////////////////////////////////////////////////////////////////////////
void hw_power_Init4p2EnabledPowerSupply(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Enable/Disable VDDMEM regulation
//!
//! Enables or disables the internal VDDMEM regulator
//!
//! \param[in] bEnable true to enable the power source, false to disable it.
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_Enable2p5(bool bEnable);
////////////////////////////////////////////////////////////////////////////////
//! \brief Returns the value of CHARGE_4P2_ILIMIT in the HW_POWER_5VCTRL reg.
//!
//! \fntype Function
//!
//! \retval Current limit for 4p2 and battery charger.  In milliamps.
////////////////////////////////////////////////////////////////////////////////
uint32_t hw_power_GetCharge4p2CurrentLimit( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the value of CHARGE_4P2_ILIMIT in the HW_POWER_5VCTRL register.
//!
//! \fntype Function
//!
//! \param[in] u32mA New current limit for 4p2 and battery charger in milliamps.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetCharge4p2CurrentLimit( uint32_t u32mA );

////////////////////////////////////////////////////////////////////////////////
//! \brief Turns on power to the 4p2 rail's components.
//!
//! \fntype Function
//!
//! \retval SUCCESS 4p2 rail powered on successfully.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_PowerOn4p2Rail( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Turns off power to the 4p2 rail's components.
//!
//! \fntype Function
//!
//! \retval SUCCESS 4p2 rail powered off successfully.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_PowerOff4p2Rail( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables the 4p2 rail.
//!
//! \fntype Function
//!
//! \retval SUCCESS 4p2 rail enabled successfully
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_Enable4p2Rail( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Disables the 4p2 rail.
//!
//! \fntype Function
//!
//! \retval SUCCESS 4p2 rail disabled successfully.
////////////////////////////////////////////////////////////////////////////////
RtStatus_t hw_power_Disable4p2Rail( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns the brownout status of the 4p2 rail.
//!
//! \fntype Function
//!
//! \retval True A brownout occurred on the 4p2 rail.
//! \retval False A brownout has not occurred on the 4p2 rail.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_GetDcdc4p2Brownout( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables/disables the hardware's ability to powerdown the chip because
//! of a 5V brownout.
//!
//! \fntype Function
//!
//! \param[in] bEnable True to enable the hardware's ability to powerdown the
//! chip, false to disable the ability.
//!
//! \retval Current state of hardware's ability before writing new state.
//!
//! \note PWD_OFF must also be cleared for a 5V brownout to powerdown the chip.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_Enable5vBrownoutPowerdown( bool bEnable );

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables/disables the hardware's ability to powerdown the chip because
//! of a battery brownout.
//!
//! \fntype Function
//!
//! \param[in] bEnable True to enable the hardware's ability to powerdown the
//! chip, false to disable the ability.
//!
//! \retval Current state of hardware's ability before writing new state.
//!
//! \note PWD_OFF must also be cleared for a battery brownout to powerdown the
//! chip.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableBattBrownoutPowerdown( bool bEnable );

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
//! \param[in] bEnable True to enable the hardware's ability to powerdown the
//! chip, false to disable the ability.  For debugger builds, this param is
//! ignored and the hardware is always enabled.
//!
//! \retval Current state of hardware's ability before writing new state.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableHardwarePowerdown( bool bEnable );

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the value of the speed sensor control bitfield.
//!
//! \fntype Function
//!
//! \param[in] u16Ctrl New control value
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetSpeedSensorControl( uint16_t u16Ctrl );

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns the value of the speed sensor control bitfield.
//!
//! \fntype Function
//!
//! \retval Value of the speed sensor control bitfield.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetSpeedSensorControl( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Writes a flag value to a register to save intialization information.
//!
//! \fntype Function
//!
//! The register the flag is written to can change depending on the chip.  The
//! bitfield used is not used during the application's normal operation.  The
//! main purpose of writing to a register is to save the information across
//! applications.
//!
//! \param[in] Value of flag
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetPowerRegInitFlag( uint16_t u16Flag );

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns the value of the saved initialization flag.
//!
//! \fntype Function
//!
//! \retval Value of the saved intialization flag.
////////////////////////////////////////////////////////////////////////////////
uint16_t hw_power_GetPowerRegInitFlag( void );

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
bool hw_power_EnableMaster5vBrownout( bool bEnable );

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
bool hw_power_EnableMasterBattBrownout( bool bEnable );

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
bool hw_power_ConfigureMasterBrownout( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the ENABLE_LOAD status for the 4.2V rail
//!
//! \fntype Function
//!
//! \retval Previous setting.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableLoadOn4p2( bool bEnable );

////////////////////////////////////////////////////////////////////////////////
//! \brief Configures the master enable switch for battery charger.
//!
//! \fntype Function
//!
//! The master enable switch must be managed between the charger and 4P2 rail.
//! The setting for the charger is saved in this function and passed on to the
//! master setting function.
//!
//! \retval Previous master switch setting.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableMasterCharge( bool bEnable );

////////////////////////////////////////////////////////////////////////////////
//! \brief Configures the master enable switch for the 4P2 rail.
//!
//! \fntype Function
//!
//! The master enable switch must be managed between the charger and 4P2 rail.
//! The setting for the 4P2 rail is saved in this function and passed on to the
//! master setting function.
//!
//! \retval Previous master switch setting.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_EnableMaster4p2( bool bEnable );

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables/disables the master enable switch for charger and 4P2 rail.
//!
//! \fntype Function
//!
//! The master enable switch must be managed between the charger and 4P2 rail.
//! The setting for the charger and 4P2 rail is passed to this function, and
//! this function determines how the master charge/4P2 switch should be set.
//!
//! The function uses OR-gate logic.  If either switch enabled, the master
//! switch will be enabled.  If both switches are disabled, the master switch
//! will be disabled.
//!
//! \retval Previous master enable setting.
////////////////////////////////////////////////////////////////////////////////
bool hw_power_ConfigureMasterChargeAnd4p2( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Enables/disables the 4P2 input to the DCDC.
//!
//! \fntype Function
//!
//! The 4P2 input to the DCDC is controlled by software.  The DCDC will not use
//! the 4P2 input until it has been configured to do so.
////////////////////////////////////////////////////////////////////////////////
void hw_power_Enable4p2DcdcInput( bool bEnable );

////////////////////////////////////////////////////////////////////////////////
//! \brief Configures the droput control for the 4P2 linear regualtor.
//!
//! \fntype Function
//!
//! The DCDC control logic uses these values to determine when current should
//! be taken from the charger and given to the 4P2 linear regulator.  Two
//! fields are present in one register setting:  the voltage margin allowed
//! before stealing current and the source comparison method.
//!
//! param[in] eDropoutMargin Allowed voltage drop in millivolts before stealing
//! current.  Use enumerated values from hw_power_4p2DropoutMargin_t.
//!
//! param[in] eSourceSelect Source and comparison method for selecting the
//! DCDC input source.  Use enumerated values from hw_power_4p2SourceSelect_t.
//!
////////////////////////////////////////////////////////////////////////////////
void hw_power_Configure4p2DropoutControl(
	hw_power_4p2DropoutMargin_t eDropoutMargin,
        hw_power_4p2SourceSelect_t  eSourceSelect);

////////////////////////////////////////////////////////////////////////////////
//! \brief Establishes the trip point for the DCDC control logic comparison
//! between DCDC4P2 and DCDCBATT.
//!
//! \fntype Function
//!
//! The trip point in the comparison between the DCDC4P2 and DCDCBATT voltage
//! can be adjusted from 85% to 105% of battery voltage.  Essentially, the
//! battery voltage is multiplied by the trip point percentage and that new
//! value is used to compare with DCDC4P2.
//!
//! \param[in] CmpTrip Requested trip point. See hw_power_4p2CmpTripPoints_t for
//! available values.
////////////////////////////////////////////////////////////////////////////////
void hw_power_Set4p2CmpTripPoint( hw_power_4p2CmpTripPoints_t CmpTrip );

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the headroom adjustment for 4P2 and charger optimizations.
//!
//! \fntype Function
//!
//! Writes the requested value to the headroom adjustment field.
//!
//! \param[in] Adj New adjustment value.  Valid values are 0 - 7 since the
//! bitfield in only 3 bits wide.
////////////////////////////////////////////////////////////////////////////////
void hw_power_SetHeadroomAdj( uint16_t Adj );

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
#endif // __HW_POWER_H








