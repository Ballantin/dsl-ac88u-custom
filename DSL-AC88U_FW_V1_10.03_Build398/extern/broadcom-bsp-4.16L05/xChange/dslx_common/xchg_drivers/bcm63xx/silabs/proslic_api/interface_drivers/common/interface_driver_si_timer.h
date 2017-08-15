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
*    Filename: interface_driver_timer.h
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the timer interface for
*      proslic api
*
****************************************************************************/
#ifndef TIME_TYPE_H
#define TIME_TYPE_H



/*
** System timer interface structure 
*/
typedef struct{
	int timerInfo;
} systemTimer_S;

/*
** System time stamp
*/
typedef struct{
	int timestamp;
} timeStamp;

#define ELAPSED_MS(S,E) (((E.tv_usec-S.tv_usec) + (1000000)*(E.tv_sec-S.tv_sec))/1000)

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
int time_GetTimeWrapper (void *hTimer, void *time);

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
int time_TimeElapsedWrapper (void *hTimer, void *startTime, int *timeInMs);

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
int time_DelayWrapper (void *hTimer, int timeInMs);

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
void TimerInit (systemTimer_S *pTimerObj);



#endif
