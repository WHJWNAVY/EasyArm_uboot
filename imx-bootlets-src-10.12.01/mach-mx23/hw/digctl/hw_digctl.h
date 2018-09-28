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

#ifndef _HW_DIGCTL_H
#define _HW_DIGCTL_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    HW_PACKAGE_TYPE_169BGA,
    HW_PACKAGE_TYPE_128TQFP
} hw_digctl_packageType_t;

typedef enum
{
    HW_UNKNOWN_CHIP_OR_REVISION=0,
    HW_3780_TA1=0x378000a1,
    HW_3780_TA2=0x378000a2,
    HW_3780_TA3=0x378000a3,
    HW_3780_TA4=0x378000a4
} hw_digctl_ChipAndRevision;

//! Indeces for chipStr array
#define CHIP_STR_TA1    0
#define CHIP_STR_TA2    1
#define CHIP_STR_TA3    2
#define CHIP_STR_TA4    3
#define CHIP_STR_TA5    4
#define CHIP_STR_TA6    5
#define CHIP_STR_TB1    6
#define CHIP_STR_TC1    7
#define CHIP_STR_UNK    8


////////////////////////////////////////////////////////////////////////////////
// Function prototypes (public API)
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
//! \brief Read the current value of the one microsecond hardware timer
//! \fntype Function
//!  This function returns the current value of the 32-bit 1-microsecond count
//!  in the digital control hardware interface.
//! \return     32-bit 1-microsecond hardware timer count.
////////////////////////////////////////////////////////////////////////////////
uint32_t hw_digctl_GetCurrentTime(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Checks the time out with one microsecond hardware timer
//! \fntype Function
//  This function detects timeout by comparing the 1-microsecond timer count
//  in the digital control hardware block with the specified timeout value.
//! \param[in]  StartTime   Start value of the 1-microsecond hardware timer
//! \param[in]  TimeOut     Expected timout (elapsed time) in microseconds.
//! \return     TRUE        Timeout occured
//!             FALSE       Not timeout yet
////////////////////////////////////////////////////////////////////////////////
bool     hw_digctl_CheckTimeOut(uint32_t StartTime, uint32_t TimeOut);

////////////////////////////////////////////////////////////////////////////////
//! \brief Waits a specified number of microseconds
//! \fntype Function
//! \param[in] u32Microseconds Number of microseconds to wait.
////////////////////////////////////////////////////////////////////////////////
void hw_digctl_MicrosecondWait(uint32_t u32Microseconds);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns package type
//! \fntype Function
//  This function detects the package type by enabling PACKAGE_SENSE_ENABLE
//  and then reading the PACKAGE_TYPE bit.
//! \return     Package Type
//! \retval     HW_PACKAGE_TYPE_100TQFP
//! \retval     HW_PACKAGE_TYPE_169BGA
//! \retval     HW_PACKAGE_TYPE_100BGA
//! \retval     HW_PACKAGE_TYPE_128TQFP
////////////////////////////////////////////////////////////////////////////////
hw_digctl_packageType_t hw_digctl_CheckPackageType(void);


////////////////////////////////////////////////////////////////////////////////
//! \brief Returns chip revision number
//! \fntype Function
//  This function returns the chip and revision number by inspecting internal
//  registers.
//! \return     Chip and revision
////////////////////////////////////////////////////////////////////////////////
hw_digctl_ChipAndRevision hw_digctl_GetChipRevision(void);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sets the ARM cache timing
//! \fntype Function
//! \param[in] u16Timing Two-bit cache timing value.
////////////////////////////////////////////////////////////////////////////////
void hw_digctl_SetArmCacheTiming(uint16_t u16Timing);

////////////////////////////////////////////////////////////////////////////////
//! \brief Returns chip revision string
//! \fntype Function
//! \param[in] revision Chip revision number (hexidecimal format)
//! \retval Pointer to string that contains chip revision.
////////////////////////////////////////////////////////////////////////////////
const char * hw_digctl_getChipRevStr(uint32_t revision);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //_HW_DIGCTL_H
////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
