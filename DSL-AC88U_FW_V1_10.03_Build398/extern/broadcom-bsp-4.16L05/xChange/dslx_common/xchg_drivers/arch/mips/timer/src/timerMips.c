/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: timerMips.c
*
****************************************************************************
*    Description:
*
*     Implements timer module for MIPS core, based upon CP0 count and
*     compare registers. Implements to the generic API defined by xdrvTimer.h.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvTimer.h>
#include <xdrvGlobalInterrupt.h>
#include <timerMips.h>
#include <intCtrlMips.h>
#include <xchgAssert.h>
#include <stddef.h>
#include "timerClockMips.h"


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/* MIPS core clock frequency in hertz. */
static unsigned int  gCpuClockFreqHz;

static XDRV_BOOL     gTimerInstalled;


/* ---- Private Function Prototypes -------------------------------------- */

static unsigned int TimerInterruptHandler( void *userArg );


/* ---- Functions -------------------------------------------------------- */



/*
*****************************************************************************
** FUNCTION:   timerInitMips
**
** PURPOSE:    Initialize the MIPS timer module.
**
** PARAMETERS: cpuClockFreqHz (in)  MIPS clock frequency in hertz.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void timerInitMips( unsigned int cpuClockFreqHz )
{
   gCpuClockFreqHz   = cpuClockFreqHz;
   gTimerInstalled   = XDRV_FALSE;
}


/*
*****************************************************************************
** FUNCTION:   timerDeinitMips
**
** PURPOSE:    De-initializes the MIPS timer module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void timerDeinitMips( void )
{
}


/*
*****************************************************************************
** FUNCTION:   xdrvTimerPeriodicInstall
**
** PURPOSE:    Register a periodic timer.
**
** PARAMETERS: isrHandler  (in)  ISR callback to be invoked periodically.
**             periodMs    (in)  Rate at which ISR will be invoked.
**             userArg     (in)  Passed to ISR callback.
**             timer       (out) Periodic timer struct to initialize.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:       xdrvTimerPeriodicStart() must be called to start the timer,
**             this function simply registers the callback.
*****************************************************************************
*/
XDRV_STATUS xdrvTimerPeriodicInstall
(
   XDRV_TIMER_PERIODIC_ISR    isrHandler,
   unsigned int               periodMs,
   void                      *userArg,
   XDRV_TIMER_PERIODIC       *timer
)
{
   /* We only support a single periodic timer instance. */
   XCHG_ASSERT( gTimerInstalled == XDRV_FALSE );
   gTimerInstalled = XDRV_TRUE;


   /*
   ** Store per timer state.
   **
   ** I'm actually going to use the 'periodMs' field to store the number of
   ** clock cycles required for the periodic timer. We use the 'count' and
   ** 'compare' registers to implement the timer. The 'count' register
   ** increments at half the MIPS core frequency. So, divide the CPU freq
   ** by 2, and then divide this by the frequency of the required timer
   ** interrupt to get the number of clock cycles between periodic interupts.
   */
   timer->isrHandler = isrHandler;
   timer->periodMs   = ( gCpuClockFreqHz / 2 / ( 1000 / periodMs ) );
   timer->userArg    = userArg;



   /*
   ** Register our internal interrupt handler with the interrupt control module.
   ** Initially, the timer should be disabled. It needs to be explicitly started
   ** with the xdrvTimerPeriodicStart API.
   */
   xdrvIntCtrlDisableInterrupt( INT_CTRL_MIPS_TIMER_INTERRUPT );

   xdrvIntCtrlInstallInterrupt( INT_CTRL_MIPS_TIMER_INTERRUPT,
                                TimerInterruptHandler,
                                (void *) timer );


   return ( XDRV_STATUS_OK );
}


/*
*****************************************************************************
** FUNCTION:   xdrvTimerPeriodicUninstall
**
** PURPOSE:    To de-register a periodic timer.
**
** PARAMETERS: timer (mod) Periodic timer struct.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS xdrvTimerPeriodicUninstall( XDRV_TIMER_PERIODIC *timer )
{
   /*
   ** De-register the timer interrupt with the interrupt control module.
   */
   xdrvIntCtrlDisableInterrupt( INT_CTRL_MIPS_TIMER_INTERRUPT);

   xdrvIntCtrlUninstallInterrupt( INT_CTRL_MIPS_TIMER_INTERRUPT );

   gTimerInstalled = XDRV_FALSE;

   return ( XDRV_STATUS_OK );
}


/*
*****************************************************************************
** FUNCTION:   xdrvTimerPeriodicStart
**
** PURPOSE:    Start a periodic timer.
**
** PARAMETERS: timer (mod) Periodic timer struct.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS xdrvTimerPeriodicStart( XDRV_TIMER_PERIODIC *timer )
{
   XDRV_GLOBAL_INTERRUPT_LOCK    lock;

   /*
   ** Setup the clock with the appropriate period. The 'periodMs' field
   ** actually contains the number of clock cycles that corresponds to the
   ** timer period. Add this value to the current count, and load the compare
   ** register.
   */
   xdrvGlobalInterruptDisable( &lock );
   {
      unsigned int cycleCount;

      cycleCount  = timerGetCountMips();
      cycleCount += timer->periodMs;

      timerSetCompareMips( cycleCount );
   }
   xdrvGlobalInterruptRestore( &lock );


   /* Enable the interrupt. */
   xdrvIntCtrlEnableInterrupt( INT_CTRL_MIPS_TIMER_INTERRUPT );

   return ( XDRV_STATUS_OK );
}


/*
*****************************************************************************
** FUNCTION:   xdrvTimerPeriodicStop
**
** PURPOSE:    Stop a periodic timer.
**
** PARAMETERS: timer (mod) Periodic timer struct.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS xdrvTimerPeriodicStop( XDRV_TIMER_PERIODIC *timer )
{
   xdrvIntCtrlDisableInterrupt( INT_CTRL_MIPS_TIMER_INTERRUPT);

   return ( XDRV_STATUS_OK );
}



/*
*****************************************************************************
** FUNCTION:   TimerInterruptHandler
**
** PURPOSE:    Internal interrupt handler. This bridges the interrupt control
**             module with the user registered interrupt service route.
**             This function should re-start the periodic timer,
**             and invoke the user register interrupt handler.
**
** PARAMETERS: userArg  (in)  User registered interrupt parameter.
**
** RETURNS:    0
**
** NOTE:
*****************************************************************************
*/
static unsigned int TimerInterruptHandler( void *userArg )
{
   /* The user data is actually a pointer to the timer object. */
   XDRV_TIMER_PERIODIC *timer = (XDRV_TIMER_PERIODIC *) userArg;

   /*
   ** Reset the timer. This will also clear the interrupt.
   */
   timerResetMips( timer->periodMs );


   /*
   ** Invoke the user registered interrupt handler.
   */
   if ( timer->isrHandler != NULL )
   {
      timer->isrHandler( timer->userArg );
   }


   /*
   ** Re-enable the interrupt.
   */
   xdrvIntCtrlEnableInterrupt( INT_CTRL_MIPS_TIMER_INTERRUPT );


   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   xdrvTimerDelayUSec
**
** PURPOSE:    Disable interrupts and spin for the specified amount of
**             micro-seconds.
**
** PARAMETERS: uSec  (in) Number of micro-seconds to spin for.
**
** RETURNS:    Nothing.
**
** NOTE:       An overflow error can occur if this is called with a sufficiently
**             large delay value on a high speed processor. Currently, the
**             the fastest processor that we support is 300MHz, and would
**             require a requested delay value of 28 seconds. This isn't
**             a reasonable amount of time of disable interrupts for and spin.
**             If this assumption proves to be wrong, then we will need to
**             change the implementation to spin in small chunks of the
**             requested delay.
*****************************************************************************
*/
void xdrvTimerDelayUSec( unsigned int uSec )
{
   unsigned int                  uSecTicks;
   unsigned int                  countTicks;
   int                           diff;
   XDRV_GLOBAL_INTERRUPT_LOCK    lock;


   xdrvGlobalInterruptDisable( &lock );
   {

      /*
      ** We use the MIPS CP0 'count' register to implement the timer.
      ** The 'count' register increments at half the MIPS core frequency. So,
      ** divide the CPU freq by 2, and then multiple this by the delay in
      ** seconds to calculate the number of clock cycles that need to expire for
      ** the requested delay.
      */
      uSecTicks  = gCpuClockFreqHz / 2;
      uSecTicks  = ( uSecTicks / 1000000 ) * uSec;


      /* Calculate the expiration tick value. */
      countTicks = timerGetCountMips();
      uSecTicks += countTicks;


      /* Keep spinning until the counter has reached the desired value. */
      do
      {
         countTicks = timerGetCountMips();
         diff = uSecTicks - countTicks;
      }
      while( diff >= 0 );

   }
   xdrvGlobalInterruptRestore( &lock );
}


/*
*****************************************************************************
** FUNCTION:   xdrvTimerDelayMSec
**
** PURPOSE:    Disable interrupts and spin for the specified amount of
**             milli-seconds.
**
** PARAMETERS: mSec  (in) Number of milli-seconds to spin for.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvTimerDelayMSec( unsigned int mSec )
{
   xdrvTimerDelayUSec( mSec * 1000 );
}
