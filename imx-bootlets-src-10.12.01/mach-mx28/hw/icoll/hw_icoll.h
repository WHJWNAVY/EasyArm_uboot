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

#ifndef __HW_ICOLL_H
#define __HW_ICOLL_H


////////////////////////////////////////////////////////////////////////////////
//   Includes
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "hw_icoll_errordefs.h"
#include "registers/hw_irq.h"
#include "registers/regsicoll.h"


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// These priorities match the IRQ priorities.  Highest priority is
// Level 3 while Level 0 is the lowest priority.
//! \brief TBD
typedef enum
{
    //! \brief lowest priority irq
    ICOLL_PRIORITY_LEVEL_0 = 0,
    //! \brief 2nd to the lowest priority irq
    ICOLL_PRIORITY_LEVEL_1,
    //! \brief 2nd to the highest priority irq
    ICOLL_PRIORITY_LEVEL_2,
    //! \brief highest priority irq
    ICOLL_PRIORITY_LEVEL_3
}
IcollPriority_t;

//! \brief TBD
typedef enum
{
    //! \brief TBD
    ICOLL_CTRL_IRQ_FINAL_ENABLE = BP_ICOLL_CTRL_IRQ_FINAL_ENABLE,
    //! \brief TBD
    ICOLL_CTRL_FIQ_FINAL_ENABLE = BP_ICOLL_CTRL_FIQ_FINAL_ENABLE,
    //! \brief TBD
    ICOLL_CTRL_ARM_RSE = BP_ICOLL_CTRL_ARM_RSE_MODE,
    //! \brief TBD
    ICOLL_CTRL_NO_NESTING = BP_ICOLL_CTRL_NO_NESTING,
    //! \brief TBD
    ICOLL_CTRL_BYPASS_FSM = BP_ICOLL_CTRL_BYPASS_FSM
}
IcollCtrlRegisterEnums_t;


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif


//enable the ICOLL vector.  Return previous enable state.
////////////////////////////////////////////////////////////////////////////////
//! \brief  Enable ICOLL Interrupt Vector.
//! \fntype Function
//!
//! This function enables the ICOLL interrupt. Set bEnable to 1, to enable
//! the Interrupt defined in eIrqVectorNumber.
//!
//! \param[in]	 eIrqVectorNumber  The ICOLL irq to enable/disable.
//! \param[in]	 bEnable Flag to enable or disable (TRUE = enable).
//! \retval      TRUE   Previous state was enabled.
//! \retval      FALSE   Previous state was disabled.
////////////////////////////////////////////////////////////////////////////////
bool hw_icoll_EnableVector(ICOLL_IRQ_enums_t eIrqVectorNumber, bool bEnable);

//use this function to set vectors 32-35 as FIQ enabled
////////////////////////////////////////////////////////////////////////////////
//! \brief  Set FIQ mode for ICOLL interrupt source.
//! \fntype Function
//!
//! This function enables the interrupt as a Fast Interrupt. Set bEnable to 1,
//! to enable Interrupt on the bit defined in eIrqVectorNumber.
//!
//! \param[in]   eIrqVectorNumber  The ICOLL irq that occurred.
//! \param[in]   bEnable Flag to enable or disable.
//! \note This only applies to Interrupts 32-35.
////////////////////////////////////////////////////////////////////////////////
void hw_icoll_SetFiqMode(ICOLL_FIQ_enums_t eIrqVectorNumber, bool bEnable);

//use this function to update the bits in the HW_ICOLL_CTRL register.
////////////////////////////////////////////////////////////////////////////////
//! \brief  Update the HW_ICOLL_CTRL register.
//! \fntype Function
//!
//! This function allows setting or clearing of the BYPASS_FSM, NO_NESTING,
//! ARM_RSE_MODE, FIQ_FINAL_ENABLE, and the IRQ_FINAL_ENABLE bits in the
//! HW_ICOLL_CTRL register.
//!
//! \param[in]   ControlRegisterField  The ICOLL_CTRL field to change.
//! \param[in]   bSet Flag to set or clear (1 = set).
//! \retval      previous field value.
//! \note Only 1 parameter can be set at a time.
////////////////////////////////////////////////////////////////////////////////
bool hw_icoll_CtrlRegisterUpdate(IcollCtrlRegisterEnums_t ControlRegisterField,
	 bool bSet);


#ifdef __cplusplus
} // extern "C"
#endif

#endif //__HW_ICOLL_H

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}

