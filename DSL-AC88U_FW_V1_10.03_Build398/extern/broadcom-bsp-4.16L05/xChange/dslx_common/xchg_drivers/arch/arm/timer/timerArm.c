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
*    Filename: timerArm.c
*
****************************************************************************
*    Description:
*
*     Implements timer module for ARM core. Implements to the generic API
*     defined by xdrvTimer.h.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvTimer.h>
#include <xdrvGlobalInterrupt.h>
#include <intCtrlArm.h>
#include <xchgAssert.h>
#include <unistd.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

static XDRV_BOOL     gTimerInstalled;

/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

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

   /* TODO - fill this in */

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
   /* TODO - fill this in */

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
   /* TODO */
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
   /* TODO */
   return ( XDRV_STATUS_OK );
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
   XDRV_GLOBAL_INTERRUPT_LOCK lock;

   xdrvGlobalInterruptDisable( &lock );
   {
      udelay( (unsigned long)uSec );
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
