/**********************************************************************************
** Copyright © 2009 Broadcom
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
**      This file contains the Call Manager date utilities.
**
***********************************************************************************/

#ifndef __CM_DATE__H__INCLUDED__
#define __CM_DATE__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>

/* ---- Constants and Types ----------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

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
void cmDateInit ( void );


/***********************************************************************************
** FUNCTION:   cmDateOffsetMin
**
** PURPOSE:    Adjust the current time according to the offset information passed.
**
** PARAMETERS: pTime - the current local time.
**             sMinDelta - the offset (in minutes) to adjust the time to.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmDateOffsetMin ( struct tm *pTime, BOS_SINT32 sMinDelta );


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
void cmDateAdjMonth ( struct tm *pTime, BOS_BOOL bUp );


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
void cmDateAdjDay ( struct tm *pTime, BOS_UINT32 uDay, BOS_BOOL bUp );


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
void cmDateAdjHour ( struct tm *pTime, BOS_UINT32 uHr, BOS_BOOL bUp );


/***********************************************************************************
** FUNCTION:   cmDateAdjMin
**
** PURPOSE:    Adjust the current minute according to the offset information passed.
**
** PARAMETERS: pTime - the current local time.
**             uHr - the number of minute to offset the current local time.
**             bUp - BOS_TRUE to add uMin to current local time, BOS_FALSE to
**                   substract uMin to current local time.
**
** RETURNS:    None.
**
** NOTE:       May adjust the hour, day, month and year accordingly as well.
***********************************************************************************/
void cmDateAdjMin ( struct tm *pTime, BOS_UINT32 uMin, BOS_BOOL bUp );


#if defined(__cplusplus)
}
#endif

#endif /* __CM_DATE__H__INCLUDED__ */

