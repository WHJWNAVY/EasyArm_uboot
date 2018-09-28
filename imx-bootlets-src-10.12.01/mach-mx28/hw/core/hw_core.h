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

#ifndef __CORE_H
#define __CORE_H

#include "types.h"

#if defined(HW_TARGET_BRAZOS)
//! \brief TBD
#define VECTOR_OFFSET 0x0000
#elif defined(HW_TARGET_SIMULATOR)
//! \brief TBD
#define VECTOR_OFFSET 0x0000
#else
//! \brief TBD
#define VECTOR_OFFSET 0x80000000
#endif

//! \brief TBD
typedef enum
{
    //! \brief TBD
    VECTOR_RESET                   = 0,
    //! \brief TBD
    VECTOR_UNDEFINED_INSTRUCTION   = 1,
    //! \brief TBD
    VECTOR_SWI                     = 2,
    //! \brief TBD
    VECTOR_PREFETCH_ABORT          = 3,
    //! \brief TBD
    VECTOR_DATA_ABORT              = 4,
//  VECTOR_RESERVED                = 5,  // Comment out since not real.
    //! \brief TBD
    VECTOR_IRQ                     = 6,
    //! \brief TBD
    VECTOR_FIQ                     = 7,
    //! \brief TBD
    NUMBER_VECTORS                 = 8
} HwCoreVector_t;

//! \brief TBD
typedef void (*pvoid_void_t)(void);

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
//!     \brief       Enable or disable the IRQ Interrupt.
//!     \fntype:     Non-reentrant.
//!     \param[in]	 bEnable holds the enable/disable flag (true = enable)
//!     \retval      bool previous state of IRQ Interrupt.
////////////////////////////////////////////////////////////////////////////////
bool hw_core_EnableIrqInterrupt(bool bEnable);

////////////////////////////////////////////////////////////////////////////////
//!     \brief       Enable or disable the FIQ Interrupt.
//!     \fntype:     Non-reentrant.
//!     \param[in]	 bEnable holds the enable/disable flag (true = enable)
//!     \retval      bool previous state of FIQ Interrupt.
////////////////////////////////////////////////////////////////////////////////
bool hw_core_EnableFiqInterrupt(bool bEnable);



#ifdef __cplusplus
} // extern "C"
#endif

#endif //__CORE_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}

