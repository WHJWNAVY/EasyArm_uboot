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
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "hw/core/hw_core.h"
#include "hw/icoll/hw_icoll.h"
#include "hw_icoll_internal.h"
#include "registers/regsicoll.h"


////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//! Convert ddi priority typedef value to an integer for use with this chip.
///////////////////////////////////////////////////////////////////////////////
inline static uint32_t hw_icoll_Priority_Ddi2Hw( IcollPriority_t ePriority )

{
    uint32_t    uPriority;

	switch ( ePriority )
	{
		case ICOLL_PRIORITY_LEVEL_0 :
			uPriority = BV_ICOLL_INTERRUPTn_PRIORITY__LEVEL0;
			break;

		case ICOLL_PRIORITY_LEVEL_1 :
			uPriority = BV_ICOLL_INTERRUPTn_PRIORITY__LEVEL1;
			break;

		case ICOLL_PRIORITY_LEVEL_2 :
			uPriority = BV_ICOLL_INTERRUPTn_PRIORITY__LEVEL2;
			break;

		case ICOLL_PRIORITY_LEVEL_3 :
			uPriority = BV_ICOLL_INTERRUPTn_PRIORITY__LEVEL3;
			break;
	}

    return uPriority;
}


///////////////////////////////////////////////////////////////////////////////
//! Convert an integer for use with this chip to a ddi priority typedef value
///////////////////////////////////////////////////////////////////////////////
inline static IcollPriority_t hw_icoll_Priority_Hw2Ddi( uint32_t uPriority  )

{
    IcollPriority_t tPriority;

	switch ( uPriority )
	{
		case BV_ICOLL_INTERRUPTn_PRIORITY__LEVEL0 :
			tPriority = ICOLL_PRIORITY_LEVEL_0;
			break;

		case BV_ICOLL_INTERRUPTn_PRIORITY__LEVEL1 :
			tPriority = ICOLL_PRIORITY_LEVEL_1;
			break;

		case BV_ICOLL_INTERRUPTn_PRIORITY__LEVEL2 :
			tPriority = ICOLL_PRIORITY_LEVEL_2;
			break;

		case BV_ICOLL_INTERRUPTn_PRIORITY__LEVEL3 :
			tPriority = ICOLL_PRIORITY_LEVEL_3;
			break;
	}

    return tPriority;
}



#pragma ghs section text=".ocram.text"
///////////////////////////////////////////////////////////////////////////////
//! See hw_icoll.h for details.
///////////////////////////////////////////////////////////////////////////////
bool hw_icoll_EnableVector(ICOLL_IRQ_enums_t eIrqVectorNumber, bool bEnable)
{
    bool b_previousEnableState;

	b_previousEnableState=
		(HW_ICOLL_INTERRUPTn(eIrqVectorNumber).B.ENABLE)?true:false;
	if (bEnable)
		HW_ICOLL_INTERRUPTn_SET(eIrqVectorNumber,
			BM_ICOLL_INTERRUPTn_ENABLE);
	else
		HW_ICOLL_INTERRUPTn_CLR(eIrqVectorNumber,
			BM_ICOLL_INTERRUPTn_ENABLE);

    return b_previousEnableState;
}


#pragma ghs section


////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}

