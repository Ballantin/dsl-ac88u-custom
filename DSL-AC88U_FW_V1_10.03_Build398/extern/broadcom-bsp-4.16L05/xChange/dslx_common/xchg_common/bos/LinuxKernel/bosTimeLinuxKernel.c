/****************************************************************************
*
*  Copyright (c) 2001-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*  Filename: bosTimeLinuxKernel.c
*
****************************************************************************
*  Description:
*
*
****************************************************************************/
/**
*
*  @file    bosTimeLinuxKernel.c
*
*  @brief   LinuxKernel implementation of the BOS Time Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_TIME

#include <string.h>
#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <bosTime.h>
#include <bosTimePrivate.h>
#include <bosLinuxKernel.h>
#include <linux/jiffies.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* Largest bitmask that can be multiplied by 1000 without overflow
 *  ie. 0x00418937 * 1000 = 0xfffffed8 (127 from overflow)
 *      0x003fffff largest bitmask to create early tick wrap-around with no overflows
 *    = 4.85451736111 days at 1 tick = 10 ms
 *    = 4d 20h 30m 30.3s */
#define BOS_TIME_MAX_TICKS 0x003ffffful

/* ---- Private Variables ------------------------------------------------ */

static   BOS_BOOL gBosTimeInitialized = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------------------- */

static BOS_STATUS CheckTimeStatusLinuxKernel( unsigned long err, BOS_TIME_FUNC_ID funcId );

/* ---- Functions -------------------------------------------------------- */

/***************************************************************************/

BOS_STATUS bosTimeInit( void )
{
   BOS_ASSERT( !gBosTimeInitialized );

   bosTimeInitFunctionNames();

   gBosTimeInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosTimeInit */

/***************************************************************************/

BOS_STATUS bosTimeTerm( void )
{
   BOS_ASSERT( gBosTimeInitialized );

   gBosTimeInitialized = BOS_FALSE;

   return BOS_STATUS_OK;

} /* bosTimeTerm */

BOS_STATUS bosTimeGetTimeOfDay (BOS_TIME_SEC *osTimeSec, BOS_TIME_USEC *osTimeMicroSec)
{
   struct timeval tm;
   BOS_ASSERT( osTimeSec );
   BOS_ASSERT( osTimeMicroSec );
   do_gettimeofday(&tm);
   *osTimeSec = tm.tv_sec;
   *osTimeMicroSec = tm.tv_usec;
   return BOS_STATUS_OK;
}

/***************************************************************************/

BOS_STATUS bosTimeGetMs(BOS_TIME_MS *osTime)
{
   BOS_ASSERT( osTime );

   /* Convert to ms */
   *osTime = BOS_TICKS_TO_MS( (BOS_TIME_MAX_TICKS & (jiffies-INITIAL_JIFFIES)) ); /* wrapped around at BOS_TIME_MAX_TICKS*/

   return ( BOS_STATUS_OK );

} /* bosTimeGetMs */

/***************************************************************************/

BOS_STATUS bosTimeCalcDeltaMs( const BOS_TIME_MS *firstTime,
                                 const BOS_TIME_MS *secondTime,
                                 BOS_TIME_MS *deltaTime)
{
   BOS_ASSERT( firstTime != NULL );
   BOS_ASSERT( secondTime != NULL );
   BOS_ASSERT( deltaTime != NULL );

   /* Calculate delta time - take care of the wrap around case */
   if ( *secondTime < *firstTime )
   {
      /* Wrap-around delta calculation:
       * e.g: delta(0xfffffffe,0x02) = 4 ticks = 0xff - 0xfe + 0x02 + 1 */
      *deltaTime  = BOS_TICKS_TO_MS( BOS_TIME_MAX_TICKS ) - *firstTime + 1;
      *deltaTime += *secondTime;
   }
   else
   {
      /* Normal delta calculation */
      *deltaTime = *secondTime - *firstTime;
   }

   return ( BOS_STATUS_OK );

} /* bosTimeCalcDeltaMs */

/***************************************************************************/

BOS_STATUS bosTimeSetWallTime ( const time_t *wallTime )
{
   return CheckTimeStatusLinuxKernel( BOS_STATUS_ERR, BOS_TIME_SET_WALL_TIME_ID );

} /* bosTimeSetWallTime */

/***************************************************************************/

BOS_STATUS bosTimeGetGmTime ( const time_t *gmTime, struct tm *brokenDownTime )
{
   return CheckTimeStatusLinuxKernel( BOS_STATUS_ERR, BOS_TIME_GET_GMTIME );

} /* bosTimeGetGmTime */

/***************************************************************************/

BOS_STATUS bosTimeGetLocalTime ( const time_t *localTime, struct tm *brokenDownTime )
{
   return CheckTimeStatusLinuxKernel( BOS_STATUS_ERR, BOS_TIME_GET_LOCALTIME );

} /* bosTimeGetLocalTime */


/***************************************************************************/

BOS_STATUS bosTimeMkTime ( struct tm *brokenDownTime, time_t *timep )
{
   time_t   calendarTime;

   /*
   ** The Linux kernel implementation of mktime() appears to reentrant, there is no
   ** mktime_r version, so just use the standard ANSI version
   */
   calendarTime = mktime(  brokenDownTime->tm_year,
                           brokenDownTime->tm_mon,
                           brokenDownTime->tm_mday,
                           brokenDownTime->tm_hour,
                           brokenDownTime->tm_min,
                           brokenDownTime->tm_sec );

   *timep = calendarTime;
   return ( BOS_STATUS_OK );
}  /* bosTimeMkTime */


/**
 * @addtogroup bosTimeInternal
 * @{
 */

/***************************************************************************/
/**
*  Translate a LinuxKernel error code into a BOS error code and report any
*  errors encountered.
*
*  @param   err      (in)  LinuxKernel Error code.
*  @param   funcId   (in)  ID of the function that we're checking for.
*/

static BOS_STATUS CheckTimeStatusLinuxKernel( unsigned long err, BOS_TIME_FUNC_ID funcId )
{
   return bosErrorCheckStatusLinuxKernel( err,
                                   BOS_MAKE_ERROR_SOURCE( BOS_TIME_MODULE, funcId ));

} /* CheckTimeStatusLinuxKernel */

/** @} */

#endif   /* BOS_CFG_TIME */
