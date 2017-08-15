/****************************************************************************
*
* Copyright (c) 2002 Broadcom, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom
* company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosTimerLinux.h
*
*  @brief   Linux specific definitions for the BOS Timer module.
*
****************************************************************************/

#if !defined( BOSTIMERLINUXUSER_H )
#define BOSTIMERLINUXUSER_H       /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXUSER_H )
#  include "bosLinuxUser.h"
#endif

#if !defined( BOSTIMER_H )
#  include <bosTimer.h>
#endif

#if !defined( BOSEVENT_H )
#  include <bosEvent.h>
#endif


/**
 * @addtogroup bosTimer
 * @{
 */


#define BOS_TIMER_LINUXUSER_TASK_NAME      "BOSTMTSK"

#define BOS_TIMER_LINUXUSER_TASK_PRIORITY  MTA_TASK_DATA

#define BOS_TIMER_LINUXUSER_TASK_STACK     (8 * 1024)






/* ---- Constants and Types ---------------------------------------------- */

/**
 * Linux User mode version of a BOS_TIMER.
 */

typedef struct BOS_TIMER
{

   /* Tells whether bosTimerStart() has been called. */
   BOS_EVENT         eventStart;
   
   /* Tells whether bosTimerStop() has been called. */
   BOS_EVENT         eventStop;

   /* Tells whether bosTimerRestart() has been called. */
   BOS_EVENT         eventRestart;

   BOS_EVENT_SET     apiEventSet;

   /* Tells whether timer is currently running. */
   BOS_BOOL          bIsRunning;

   /* Last value specified in call to bosTimerStart(), used by bosTimerRestart(). */
   unsigned int      lastTimeoutMsec;

   /* Last value specified in call to bosTimerStart(), used by bosTimerRestart(). */
   unsigned int /*BOS_TIMER_MODE*/    lastTimerMode;

   /* Event that will be triggered by the timer that expires. */
   BOS_EVENT         event;

   /* Task id of the creator of the timer. */
   BOS_TASK_ID       taskId;

   /* Stores the Id of the timer that is created . */
   BOS_TASK_ID       timerId;

} BOS_TIMER;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSTIMERLINUXUSER_H */

