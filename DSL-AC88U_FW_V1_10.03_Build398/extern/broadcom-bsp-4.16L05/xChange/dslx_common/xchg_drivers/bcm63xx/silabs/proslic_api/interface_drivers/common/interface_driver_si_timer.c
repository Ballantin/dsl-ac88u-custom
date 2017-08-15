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
*    Filename: interface_driver_timer.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the timer interface for
*      proslic api
*
****************************************************************************/

//#include "proslic_datatypes.h"
#include "si_voice_datatypes.h"
//#include "proslic_timer_intf.h"
#include "si_voice_timer_intf.h"
#include "interface_driver_si_timer.h"
#include <bosTime.h>
#include <bosSleep.h>



/*
******************************************************************************
** FUNCTION:  TimerInit
**
** PURPOSE:   Initializes the timer interface
**
** PARAMETERS: None
**
** RETURNS: None
**
*****************************************************************************
*/
void TimerInit (systemTimer_S *pTimerObj)
{
   (void) pTimerObj;
}


/*
******************************************************************************
** FUNCTION:  time_DelayWrapper
**
** PURPOSE:   Sleep function
**
** PARAMETERS: timeInMs       - Sleep time
**
** RETURNS: None
**
*****************************************************************************
*/
int time_DelayWrapper (void *hTimer, int timeInMs)
{
   (void) hTimer;
   bosSleep(timeInMs);
	return 0;
}


/*
******************************************************************************
** FUNCTION:  time_TimeElapsedWrapper
**
** PURPOSE:   calculate time elaped in ms between current and previous times
**
** PARAMETERS: startTime       - Previous time
**
** RETURNS: Time elapsed in ms
**
*****************************************************************************
*/
int time_TimeElapsedWrapper (void *hTimer, void *startTime, int *timeInMs)
{
   struct timeval currTime, initTime;

   (void) hTimer;
   
   initTime = *( struct timeval *)startTime;
   bosTimeGetTimeOfDay ((BOS_TIME_SEC *)&currTime.tv_sec, (BOS_TIME_USEC *)&currTime.tv_usec);

   *timeInMs = ELAPSED_MS( initTime, currTime) ;
	return 0;
}

/*
******************************************************************************
** FUNCTION:  time_GetTimeWrapper
**
** PURPOSE:   get current time
**
** PARAMETERS: time - pointer to copy current time to
**
** RETURNS: current time
**
*****************************************************************************
*/
int time_GetTimeWrapper (void *hTimer, void *time)
{
   struct timeval tm;
   
   (void) hTimer;
   bosTimeGetTimeOfDay ((BOS_TIME_SEC *)&tm.tv_sec, (BOS_TIME_USEC *)&tm.tv_usec);
   *(struct timeval *)time = tm;
	return 0;
}



