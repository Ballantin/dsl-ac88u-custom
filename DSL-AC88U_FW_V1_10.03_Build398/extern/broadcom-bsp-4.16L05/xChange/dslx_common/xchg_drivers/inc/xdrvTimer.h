/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: xdrvTimer.h
*
****************************************************************************
*    Description:
*
*     Public interface for CPU timers.
*
****************************************************************************/

#ifndef XDRV_TIMER_H
#define XDRV_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

typedef unsigned int ( *XDRV_TIMER_PERIODIC_ISR )( void* );


typedef struct XDRV_TIMER_PERIODIC
{
   /* Pointer to client ISR handler routine. */
   XDRV_TIMER_PERIODIC_ISR isrHandler;

   /* Period for interrupt in ms.            */
   unsigned int            periodMs;

   /* User parameter passed to ISR.          */
   void                   *userArg;

} XDRV_TIMER_PERIODIC;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   xdrvTimerPeriodicInstall
**
** PURPOSE:    Register a periodic timer.
**
** PARAMETERS: isrHandler  (in)  ISR callback to be invoked periodically.
**             periodMs    (in)  Rate at which ISR will be invoked.
**             userArg     (in)  Passed to ISR callback.
**             timer       (out) Periodic timer struct to intialize.
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
);


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
XDRV_STATUS xdrvTimerPeriodicUninstall( XDRV_TIMER_PERIODIC *timer );


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
XDRV_STATUS xdrvTimerPeriodicStart( XDRV_TIMER_PERIODIC *timer );


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
XDRV_STATUS xdrvTimerPeriodicStop( XDRV_TIMER_PERIODIC *timer );


/*
*****************************************************************************
** FUNCTION:   xdrvTimerPeriodicGetHvgMonitor
**
** PURPOSE:    Get the periodic timer that monitors the HVG overcurrent state.
**
** PARAMETERS: timer (mod) Periodic timer struct.
**
** RETURNS:    0 on success, else error code.
**
** NOTE:       This functionality is specific for those 33xx reference designs
**             that are required to maintain line voltage over reset/reboot.
*****************************************************************************
*/
int xdrvTimerPeriodicGetHvgMonitor( XDRV_TIMER_PERIODIC *timer );

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
** NOTE:
*****************************************************************************
*/
void xdrvTimerDelayUSec( unsigned int uSec );


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
void xdrvTimerDelayMSec( unsigned int mSec );


/*
*****************************************************************************
** FUNCTION:   xdrvTimerRead
**
** PURPOSE:    Reads value of CPU timer.
**
** PARAMETERS: None.
**
** RETURNS:    Timer value.
**
** NOTE:
*****************************************************************************
*/
unsigned int xdrvTimerRead( void );


#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_TIMER_H  */
