//*****************************************************************************
//
//! \addtogroup systick_api
//! @{
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "hw_ints.h"
#include "hw_nvic.h"
#include "hw_types.h"
#include "debug.h"
#include "interrupt.h"
#include "systick.h"

//*****************************************************************************
//
//! Enables the SysTick counter.
//!
//! This function starts the SysTick counter.  If an interrupt handler has been
//! registered, it is called when the SysTick counter rolls over.
//!
//! \note Calling this function causes the SysTick counter to (re)commence
//! counting from its current value.  The counter is not automatically reloaded
//! with the period as specified in a previous call to SysTickPeriodSet().  If
//! an immediate reload is required, the \b NVIC_ST_CURRENT register must be
//! written to force the reload.  Any write to this register clears the SysTick
//! counter to 0 and causes a reload with the supplied period on the next
//! clock.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickEnable(void)
{
    //
    // Enable SysTick.
    //
    HWREG(NVIC_ST_CTRL) |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_ENABLE;
}

//*****************************************************************************
//
//! Disables the SysTick counter.
//!
//! This function stops the SysTick counter.  If an interrupt handler has been
//! registered, it is not called until SysTick is restarted.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickDisable(void)
{
    //
    // Disable SysTick.
    //
    HWREG(NVIC_ST_CTRL) &= ~(NVIC_ST_CTRL_ENABLE);
}

//*****************************************************************************
//
//! Registers an interrupt handler for the SysTick interrupt.
//!
//! \param pfnHandler is a pointer to the function to be called when the
//! SysTick interrupt occurs.
//!
//! This function registers the handler to be called when a SysTick interrupt
//! occurs.
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickIntRegister(void (*pfnHandler)(void))
{
    //
    // Register the interrupt handler, returning an error if an error occurs.
    //
    IntRegister(FAULT_SYSTICK, pfnHandler);

    //
    // Enable the SysTick interrupt.
    //
    HWREG(NVIC_ST_CTRL) |= NVIC_ST_CTRL_INTEN;
}

//*****************************************************************************
//
//! Unregisters the interrupt handler for the SysTick interrupt.
//!
//! This function unregisters the handler to be called when a SysTick interrupt
//! occurs.
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickIntUnregister(void)
{
    //
    // Disable the SysTick interrupt.
    //
    HWREG(NVIC_ST_CTRL) &= ~(NVIC_ST_CTRL_INTEN);

    //
    // Unregister the interrupt handler.
    //
    IntUnregister(FAULT_SYSTICK);
}

//*****************************************************************************
//
//! Enables the SysTick interrupt.
//!
//! This function enables the SysTick interrupt, allowing it to be
//! reflected to the processor.
//!
//! \note The SysTick interrupt handler is not required to clear the SysTick
//! interrupt source because it is cleared automatically by the NVIC when the
//! interrupt handler is called.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickIntEnable(void)
{
    //
    // Enable the SysTick interrupt.
    //
    HWREG(NVIC_ST_CTRL) |= NVIC_ST_CTRL_INTEN;
}

//*****************************************************************************
//
//! Disables the SysTick interrupt.
//!
//! This function disables the SysTick interrupt, preventing it from being
//! reflected to the processor.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickIntDisable(void)
{
    //
    // Disable the SysTick interrupt.
    //
    HWREG(NVIC_ST_CTRL) &= ~(NVIC_ST_CTRL_INTEN);
}

//*****************************************************************************
//
//! Sets the period of the SysTick counter.
//!
//! \param ui32Period is the number of clock ticks in each period of the
//! SysTick counter and must be between 1 and 16,777,216, inclusive.
//!
//! This function sets the rate at which the SysTick counter wraps, which
//! equates to the number of processor clocks between interrupts.
//!
//! \note Calling this function does not cause the SysTick counter to reload
//! immediately.  If an immediate reload is required, the \b NVIC_ST_CURRENT
//! register must be written.  Any write to this register clears the SysTick
//! counter to 0 and causes a reload with the \e ui32Period supplied here on
//! the next clock after SysTick is enabled.
//!
//! \return None.
//
//*****************************************************************************
void
SysTickPeriodSet(uint32_t ui32Period)
{
    //
    // Check the arguments.
    //
    ASSERT((ui32Period > 0) && (ui32Period <= 16777216));

    //
    // Set the period of the SysTick counter.
    //
    HWREG(NVIC_ST_RELOAD) = ui32Period - 1;
}

//*****************************************************************************
//
//! Gets the period of the SysTick counter.
//!
//! This function returns the rate at which the SysTick counter wraps, which
//! equates to the number of processor clocks between interrupts.
//!
//! \return Returns the period of the SysTick counter.
//
//*****************************************************************************
uint32_t
SysTickPeriodGet(void)
{
    //
    // Return the period of the SysTick counter.
    //
    return(HWREG(NVIC_ST_RELOAD) + 1);
}

//*****************************************************************************
//
//! Gets the current value of the SysTick counter.
//!
//! This function returns the current value of the SysTick counter, which is
//! a value between the period - 1 and zero, inclusive.
//!
//! \return Returns the current value of the SysTick counter.
//
//*****************************************************************************
uint32_t
SysTickValueGet(void)
{
    //
    // Return the current value of the SysTick counter.
    //
    return(HWREG(NVIC_ST_CURRENT));
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
