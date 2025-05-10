//*****************************************************************************
//
//! \addtogroup sysexc_api
//! @{
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "hw_ints.h"
#include "hw_sysctl.h"
#include "hw_sysexc.h"
#include "hw_types.h"
#include "debug.h"
#include "interrupt.h"

//*****************************************************************************
//
//! Returns the interrupt number for a system exception.
//!
//! This function returns the interrupt number for a system exception.
//!
//! \return Returns the system exception interrupt number.
//
//*****************************************************************************
static uint32_t
_SysExcIntNumberGet(void)
{
    uint32_t ui32Int;

    //
    // Get the interrupt number based on the class.
    //
    if(CLASS_IS_TM4C123)
    {
        ui32Int = INT_SYSEXC_TM4C123;
    }
    else if(CLASS_IS_TM4C129)
    {
        ui32Int = INT_SYSEXC_TM4C129;
    }
    else
    {
        ui32Int = 0;
    }
    return(ui32Int);
}

//*****************************************************************************
//
//! Registers an interrupt handler for the system exception interrupt.
//!
//! \param pfnHandler is a pointer to the function to be called when the system
//! exception interrupt occurs.
//!
//! This function places the address of the system exception interrupt handler
//! into the interrupt vector table in SRAM.  This function also enables the
//! global interrupt in the interrupt controller; specific system exception
//! interrupts must be enabled via SysExcIntEnable().  It is the interrupt
//! handler's responsibility to clear the interrupt source.
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
SysExcIntRegister(void (*pfnHandler)(void))
{
    uint32_t ui32Int;

    //
    // Get the system exception interrupt number.
    //
    ui32Int = _SysExcIntNumberGet();

    ASSERT(ui32Int != 0);

    //
    // Register the interrupt handler.
    //
    IntRegister(ui32Int, pfnHandler);

    //
    // Enable the system exception interrupt.
    //
    IntEnable(ui32Int);
}

//*****************************************************************************
//
//! Unregisters the system exception interrupt handler.
//!
//! This function removes the system exception interrupt handler from the
//! vector table in SRAM.  This function also masks off the system exception
//! interrupt in the interrupt controller so that the interrupt handler is no
//! longer called.
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
SysExcIntUnregister(void)
{
    uint32_t ui32Int;

    //
    // Get the system exception interrupt number.
    //
    ui32Int = _SysExcIntNumberGet();

    ASSERT(ui32Int != 0);

    //
    // Disable the system exception interrupt.
    //
    IntDisable(ui32Int);

    //
    // Unregister the system exception interrupt handler.
    //
    IntUnregister(ui32Int);
}

//*****************************************************************************
//
//! Enables individual system exception interrupt sources.
//!
//! \param ui32IntFlags is the bit mask of the interrupt sources to be enabled.
//!
//! This function enables the indicated system exception interrupt sources.
//! Only the sources that are enabled can be reflected to the processor
//! interrupt; disabled sources have no effect on the processor.
//!
//! The \e ui32IntFlags parameter is the logical OR of any of the following:
//!
//! - \b SYSEXC_INT_FP_IXC - Floating-point inexact exception interrupt
//! - \b SYSEXC_INT_FP_OFC - Floating-point overflow exception interrupt
//! - \b SYSEXC_INT_FP_UFC - Floating-point underflow exception interrupt
//! - \b SYSEXC_INT_FP_IOC - Floating-point invalid operation interrupt
//! - \b SYSEXC_INT_FP_DZC - Floating-point divide by zero exception interrupt
//! - \b SYSEXC_INT_FP_IDC - Floating-point input denormal exception interrupt
//!
//! \return None.
//
//*****************************************************************************
void
SysExcIntEnable(uint32_t ui32IntFlags)
{
    //
    // Enable the specified interrupts.
    //
    HWREG(SYSEXC_IM) |= ui32IntFlags;
}

//*****************************************************************************
//
//! Disables individual system exception interrupt sources.
//!
//! \param ui32IntFlags is the bit mask of the interrupt sources to be
//! disabled.
//!
//! This function disables the indicated system exception interrupt sources.
//! Only sources that are enabled can be reflected to the processor interrupt;
//! disabled sources have no effect on the processor.
//!
//! The \e ui32IntFlags parameter is the logical OR of any of the following:
//!
//! - \b SYSEXC_INT_FP_IXC - Floating-point inexact exception interrupt
//! - \b SYSEXC_INT_FP_OFC - Floating-point overflow exception interrupt
//! - \b SYSEXC_INT_FP_UFC - Floating-point underflow exception interrupt
//! - \b SYSEXC_INT_FP_IOC - Floating-point invalid operation interrupt
//! - \b SYSEXC_INT_FP_DZC - Floating-point divide by zero exception interrupt
//! - \b SYSEXC_INT_FP_IDC - Floating-point input denormal exception interrupt
//!
//! \return None.
//
//*****************************************************************************
void
SysExcIntDisable(uint32_t ui32IntFlags)
{
    //
    // Disable the specified interrupts.
    //
    HWREG(SYSEXC_IM) &= ~(ui32IntFlags);
}

//*****************************************************************************
//
//! Gets the current system exception interrupt status.
//!
//! \param bMasked is \b false if the raw interrupt status is required and
//! \b true if the masked interrupt status is required.
//!
//! This function returns the system exception interrupt status.  Either the
//! raw interrupt status or the status of interrupts that are allowed to
//! reflect to the processor can be returned.
//!
//! \return Returns the current system exception interrupt status, enumerated
//! as the logical OR of \b SYSEXC_INT_FP_IXC, \b SYSEXC_INT_FP_OFC,
//! \b SYSEXC_INT_FP_UFC, \b SYSEXC_INT_FP_IOC, \b SYSEXC_INT_FP_DZC, and
//! \b SYSEXC_INT_FP_IDC.
//
//*****************************************************************************
uint32_t
SysExcIntStatus(bool bMasked)
{
    //
    // Return either the interrupt status or the raw interrupt status as
    // requested.
    //
    if(bMasked)
    {
        return(HWREG(SYSEXC_MIS));
    }
    else
    {
        return(HWREG(SYSEXC_RIS));
    }
}

//*****************************************************************************
//
//! Clears system exception interrupt sources.
//!
//! \param ui32IntFlags is a bit mask of the interrupt sources to be cleared.
//!
//! This function clears the specified system exception interrupt sources, so
//! that they no longer assert.  This function must be called in the interrupt
//! handler to keep the interrupt from being recognized again immediately upon
//! exit.
//!
//! The \e ui32IntFlags parameter is the logical OR of any of the following:
//!
//! - \b SYSEXC_INT_FP_IXC - Floating-point inexact exception interrupt
//! - \b SYSEXC_INT_FP_OFC - Floating-point overflow exception interrupt
//! - \b SYSEXC_INT_FP_UFC - Floating-point underflow exception interrupt
//! - \b SYSEXC_INT_FP_IOC - Floating-point invalid operation interrupt
//! - \b SYSEXC_INT_FP_DZC - Floating-point divide by zero exception interrupt
//! - \b SYSEXC_INT_FP_IDC - Floating-point input denormal exception interrupt
//!
//! \note Because there is a write buffer in the Cortex-M processor, it may
//! take several clock cycles before the interrupt source is actually cleared.
//! Therefore, it is recommended that the interrupt source be cleared early in
//! the interrupt handler (as opposed to the very last action) to avoid
//! returning from the interrupt handler before the interrupt source is
//! actually cleared.  Failure to do so may result in the interrupt handler
//! being immediately reentered (because the interrupt controller still sees
//! the interrupt source asserted).
//!
//! \return None.
//
//*****************************************************************************
void
SysExcIntClear(uint32_t ui32IntFlags)
{
    //
    // Clear the requested interrupt sources.
    //
    HWREG(SYSEXC_IC) = ui32IntFlags;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
