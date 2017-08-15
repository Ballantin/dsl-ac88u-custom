/**********************************************************************************
** Copyright (c) 2009-2012 Broadcom Corporation
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This module implements date control utilities for the Call Manager.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* Some common define for date/time calculation.
*/
#define CMDATE_MONTH_IN_YEAR              12
#define CMDATE_HRS_IN_DAY                 24
#define CMDATE_MIN_IN_HR                  60  

/* Used for leap year calculation.
*/
#define CMDATE_YEAR_OFFSET                1900
#define CMDATE_LEAP_DIV_FOUR              4
#define CMDATE_LEAP_DIV_CENT              100
#define CMDATE_FEBRUARY_OFFSET            1

/* Used for offset calculation into the SNMP DateAndTime type of
** information.
*/
#define CMDATE_DATETIME_OFFSET_MONTH      2
#define CMDATE_DATETIME_OFFSET_DAY        3
#define CMDATE_DATETIME_OFFSET_HOUR       4
#define CMDATE_DATETIME_OFFSET_MIN        5


/* Note: the 'days in month' for February is adjusted on initialization
**       of the date module functionality based on leap year determination.
*/
BOS_UINT32 guDaysInMonth [ CMDATE_MONTH_IN_YEAR ] = 
            { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; 


/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmDateInit
**
** PURPOSE:    Initializes the date utility module.
**
** PARAMETERS: None.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmDateInit ( void )
{
   struct tm locTime;
   time_t calTime;
   BOS_BOOL bLeap = BOS_FALSE;

   calTime = time ( NULL );
   bosTimeGetGmTime ( &calTime, &locTime );

   /* If current year divides by 4, it is leap year candidate.
   */
   if ( ( ( locTime.tm_year + CMDATE_YEAR_OFFSET ) % CMDATE_LEAP_DIV_FOUR ) == 0 )
   {
      bLeap = BOS_TRUE;
   }

   /* If leap year cadidate divides by 100, then it must divide by 400 as well,
   ** otherwise not a leap year.
   */
   if ( bLeap )
   {
      if ( ( ( locTime.tm_year + CMDATE_YEAR_OFFSET ) %
                                       CMDATE_LEAP_DIV_CENT ) == 0 )
      {
         if ( ( ( locTime.tm_year + CMDATE_YEAR_OFFSET ) %
                     ( CMDATE_LEAP_DIV_FOUR * CMDATE_LEAP_DIV_CENT ) ) != 0 )
         {
            bLeap = BOS_FALSE;
         }
      }
   }

   /* If leap year detected, adjust the number of days in February.
   */
   if ( bLeap )
   {
      guDaysInMonth [ CMDATE_FEBRUARY_OFFSET ] += 1;   
   }
}


/***********************************************************************************
** FUNCTION:   cmDateOffsetMin
**
** PURPOSE:    Offset the current date/time according to the information passed.
**
** PARAMETERS: pTime - the current local time.
**             sMinDelta - the offset (in minutes) to adjust the time to.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmDateOffsetMin ( struct tm *pTime, BOS_SINT32 sMinDelta )
{
   BOS_UINT32 uDay;
   BOS_UINT32 uHr;
   BOS_UINT32 uMin;
   BOS_UINT32 uDelta;

   if ( !sMinDelta )
   {
      /* No adjustment needed.
      */
      return;
   }

   uDelta = ( sMinDelta > 0 ) ? sMinDelta : (sMinDelta * (-1));

   uDay = uDelta / (CMDATE_HRS_IN_DAY * CMDATE_MIN_IN_HR);
   uHr = ( uDelta % (CMDATE_HRS_IN_DAY * CMDATE_MIN_IN_HR) ) / CMDATE_MIN_IN_HR;
   uMin = uDelta % CMDATE_MIN_IN_HR;

   if ( sMinDelta > 0 )
   {
      /* Adjust the date and time forward.
      */
      cmDateAdjDay ( pTime, uDay, BOS_TRUE );
      cmDateAdjHour ( pTime, uHr, BOS_TRUE );
      cmDateAdjMin ( pTime, uMin, BOS_TRUE );
   }
   else
   {
      /* Adjust the date and time backward.
      */
      cmDateAdjDay ( pTime, uDay, BOS_FALSE );
      cmDateAdjHour ( pTime, uHr, BOS_FALSE );
      cmDateAdjMin ( pTime, uMin, BOS_FALSE );
   }
}


/***********************************************************************************
** FUNCTION:   cmDateAdjMonth
**
** PURPOSE:    Adjust the current month by one.
**
** PARAMETERS: pTime - the current local time.
**             bUp - BOS_TRUE to add one month to current local time, BOS_FALSE to
**                   substract one month to current local time.
**
** RETURNS:    None.
**
** NOTE:       May adjust the year accordingly as well.
**
**             This function is only used to add/remove one month at the time,
**             it is not anticipated that we would need more than this in the
**             scope of use of this function.
***********************************************************************************/
void cmDateAdjMonth ( struct tm *pTime, BOS_BOOL bUp )
{
   if ( pTime == NULL )
   {
      return;
   }

   /* Add one month to the current date.
   */
   if ( bUp )
   {
      if ( ( pTime->tm_mon + 1 ) >= CMDATE_MONTH_IN_YEAR )
      {
         /* Go to January next year.
         */
         pTime->tm_year += 1;
         pTime->tm_mon = 0;
      }
      else
      {
         pTime->tm_mon += 1;
      }
   }
   /* Substract one month from current date.
   */
   else
   {
      if ( ( pTime->tm_mon - 1 ) < 0 )
      {
         /* Go to December previous year.
         */
         pTime->tm_year -= 1;
         pTime->tm_mon = CMDATE_MONTH_IN_YEAR - 1;
      }
      else
      {
         pTime->tm_mon -= 1;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmDateAdjDay
**
** PURPOSE:    Adjust the current day according to the offset information passed.
**
** PARAMETERS: pTime - the current local time.
**             uDay - the number of days to offset the current local time.
**             bUp - BOS_TRUE to add uDay to current local time, BOS_FALSE to
**                   substract uDay to current local time.
**
** RETURNS:    None.
**
** NOTE:       May adjust the month and year accordingly as well.
***********************************************************************************/
void cmDateAdjDay ( struct tm *pTime, BOS_UINT32 uDay, BOS_BOOL bUp )
{
   if ( ( pTime == NULL ) || !uDay )
   {
      return;
   }

   /* Add day offset to current date.
   */
   if ( bUp )
   {
      if ( pTime->tm_mday + (int) uDay > (int) guDaysInMonth [ pTime->tm_mon ] )
      {
         pTime->tm_mday =
            pTime->tm_mday + (int) uDay - (int ) guDaysInMonth [ pTime->tm_mon ];
         /* Adjust the month afterwards as we need current month for the
         ** above calculation.
         */
         cmDateAdjMonth ( pTime, BOS_TRUE );
      }
      else
      {
         pTime->tm_mday += (int) uDay;
      }
   }
   /* Substract day offset from current date.
   */
   else
   {
      if ( pTime->tm_mday - (int) uDay <= 0 )
      {
         /* Adjust the month fisrt as we will need the new month for the
         ** following calculation.
         */
         cmDateAdjMonth ( pTime, BOS_FALSE );
         pTime->tm_mday =
            (int) guDaysInMonth [ pTime->tm_mon ] - ( pTime->tm_mday - (int) uDay );
      }
      else
      {
         pTime->tm_mday -= (int) uDay;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmDateAdjHour
**
** PURPOSE:    Adjust the current hour according to the offset information passed.
**
** PARAMETERS: pTime - the current local time.
**             uHr - the number of hour to offset the current local time.
**             bUp - BOS_TRUE to add uHr to current local time, BOS_FALSE to
**                   substract uHr to current local time.
**
** RETURNS:    None.
**
** NOTE:       May adjust the day, month and year accordingly as well.
***********************************************************************************/
void cmDateAdjHour ( struct tm *pTime, BOS_UINT32 uHr, BOS_BOOL bUp )
{
   if ( ( pTime == NULL ) || !uHr )
   {
      return;
   }

   /* Add hour offset to current date.
   */
   if ( bUp )
   {
      if ( ( pTime->tm_hour + (int) uHr ) > ( CMDATE_HRS_IN_DAY - 1 ) )
      {
         cmDateAdjDay ( pTime, 1, BOS_TRUE );
         pTime->tm_hour = ( pTime->tm_hour + (int) uHr ) % CMDATE_HRS_IN_DAY;
      }
      else
      {
         pTime->tm_hour += (int) uHr;
      }
   }
   /* Substract hour offset from current date.
   */
   else
   {
      if ( ( pTime->tm_hour - (int) uHr ) < 0 )
      {
         cmDateAdjDay ( pTime, 1, BOS_FALSE );
         pTime->tm_hour = ( CMDATE_HRS_IN_DAY - (int) uHr + pTime->tm_hour );
      }
      else
      {
         pTime->tm_hour -= (int) uHr;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmDateAdjMin
**
** PURPOSE:    Adjust the current minute according to the offset information passed.
**
** PARAMETERS: pTime - the current local time.
**             uMin - the number of minute to offset the current local time.
**             bUp - BOS_TRUE to add uMin to current local time, BOS_FALSE to
**                   substract uMin to current local time.
**
** RETURNS:    None.
**
** NOTE:       May adjust the hour, day, month and year accordingly as well.
***********************************************************************************/
void cmDateAdjMin ( struct tm *pTime, BOS_UINT32 uMin, BOS_BOOL bUp )
{
   if ( ( pTime == NULL ) || !uMin )
   {
      return;
   }

   /* Add minute offset to current date.
   */
   if ( bUp )
   {
      if ( ( pTime->tm_min + (int) uMin ) > ( CMDATE_MIN_IN_HR - 1 ) )
      {
         cmDateAdjHour ( pTime, 1, BOS_TRUE );
         pTime->tm_min = ( pTime->tm_min + (int) uMin ) % CMDATE_MIN_IN_HR;
      }
      else
      {
         pTime->tm_min += (int) uMin; 
      }
   }
   /* Substract minute offset from current date.
   */
   else
   {
      if ( ( pTime->tm_min - (int) uMin ) < 0 )
      {
         cmDateAdjHour ( pTime, 1, BOS_FALSE );
         pTime->tm_min = CMDATE_MIN_IN_HR - (int) uMin + pTime->tm_min;
      }
      else
      {
         pTime->tm_min -= (int) uMin; 
      }
   }
}
