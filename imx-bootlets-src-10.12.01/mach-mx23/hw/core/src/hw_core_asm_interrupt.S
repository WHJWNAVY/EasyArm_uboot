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

  .file	"hw_core_asm_interrupt.S"
	.text

    .global hw_core_EnableIrqInterrupt
    .global hw_core_EnableFiqInterrupt

/*
//!
//!     \brief       Enable or disable the IRQ Interrupt.
//!
//!     \fntype:     Non-reentrant.
//!
//!     Description  Enable or disable the IRQ Interrupt.
//!
//!     \param[in]	 bool r0 holds the enable/disable flag (true = enable)
//!
//!     \retval      bool previous state of IRQ Interrupt.
//!
//!     Notes:
//!
//////////////////////////////////////////////////////////////////////////////*/
hw_core_EnableIrqInterrupt:
    MRS     r2,CPSR            @Save Current Program Status Register
    teq     r0,#0
    bicNE   r1,r2,#0x80        @Disable IRQ if clear
    orrEQ   r1,r2,#0x80        @Enable IRQ if set.
    MSR     CPSR_c,r1
    tst     r2,#0x80
    movne   r0,#0
    moveq   r0,#1
    bx      lr

/*
//!
//!     \brief       Enable or disable the FIQ Interrupt.
//!
//!     \fntype:     Non-reentrant.
//!
//!     Description  Enable or disable the FIQ Interrupt.
//!
//!     \param[in]	 bool r0 holds the enable/disable flag (true = enable)
//!
//!     \retval      bool previous state of FIQ Interrupt.
//!
//!     Notes:
//!
//////////////////////////////////////////////////////////////////////////////*/
hw_core_EnableFiqInterrupt:
    MRS     r2,CPSR            @Save Current Program Status Register.
    teq     r0,#0
    bicNE   r1,r2,#0x40        @Disable FIQ if clear.
    orrEQ   r1,r2,#0x40        @Enable FIQ if set.
    MSR     CPSR_c,r1
    tst     r2,#0x40
    movne   r0,#0
    moveq   r0,#1
    bx      lr
