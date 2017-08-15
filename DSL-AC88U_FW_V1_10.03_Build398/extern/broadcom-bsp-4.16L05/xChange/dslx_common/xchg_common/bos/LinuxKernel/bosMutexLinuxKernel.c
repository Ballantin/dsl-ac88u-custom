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
*  Filename: bosMutexLinuxKernel.c
*
****************************************************************************
*  Description:
*
*
****************************************************************************/
/**
*
*  @file    bosMutexLinuxKernel.c
*
*  @brief   LinuxKernel implementation of the BOS Mutex Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_MUTEX

#include <linux/sched.h>
#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <bosMutex.h>
#include <bosMutexPrivate.h>
#include <bosSem.h>
#include <bosLog.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

static   BOS_BOOL gBosMutexInitialized = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------------------- */

static BOS_STATUS CheckMutexStatusLinuxKernel( int err, BOS_MUTEX_FUNC_ID funcId );

/* ---- Functions -------------------------------------------------------- */

/***************************************************************************/

BOS_STATUS bosMutexInit( void )
{
   BOS_ASSERT( !gBosMutexInitialized );

   bosMutexInitFunctionNames();

   gBosMutexInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosMutexInit */

/***************************************************************************/

BOS_STATUS bosMutexTerm( void )
{
   BOS_ASSERT( gBosMutexInitialized );

   gBosMutexInitialized = BOS_FALSE;

   return BOS_STATUS_OK;

} /* bosMutexTerm */

/***************************************************************************/

BOS_STATUS bosMutexCreate( const char *name, BOS_MUTEX *mutex )
{
   BOS_STATUS status;

   BOS_ASSERT( gBosMutexInitialized );
   
   status = bosSemCreate( name, 1, BOS_CFG_SEM_MAX_COUNT, (BOS_SEM*)&(mutex->mutex) );
   if ( status != BOS_STATUS_OK )
   {
      blogStrErr(( gBosBlog, "bosMutexCreate() - memory cannot be allocated"));
      return ( BOS_STATUS_ERR );
   }
   mutex->owner = NULL;
   mutex->recurCnt = 0;
   
   return ( BOS_STATUS_OK );

} /* bosMutexCreate */

/***************************************************************************/

BOS_STATUS bosMutexDestroy( BOS_MUTEX *mutex )
{
   BOS_STATUS status;

   BOS_ASSERT( gBosMutexInitialized );

   status = bosSemDestroy( (BOS_SEM*)&(mutex->mutex) );
   if ( status != BOS_STATUS_OK )
   {
      return CheckMutexStatusLinuxKernel( status, BOS_MUTEX_DESTROY_FUNC_ID );
   }

   memset( mutex, 0, sizeof(*mutex) );
   // *mutex = NULL;
   return ( BOS_STATUS_OK );

} /* bosMutexDestroy */

/***************************************************************************/

BOS_STATUS bosMutexAcquire( BOS_MUTEX *mutex )
{
   BOS_STATUS status;

   BOS_ASSERT( gBosMutexInitialized );

   // try locking the mutex when mutex is not owned by anybody OR the current task is not the owner
   if( current != mutex->owner )
   {
       // try to take the mutex
       status = bosSemTake( (BOS_SEM*)&(mutex->mutex) );
       if ( status != BOS_STATUS_OK )
       {
           return CheckMutexStatusLinuxKernel( status, BOS_MUTEX_ACQUIRE_FUNC_ID );
       }
       // got the mutex, then become the owner
       mutex->owner = current;
   }
   // allows the mutex to be taken recursively
   mutex->recurCnt++;

   return ( BOS_STATUS_OK );

} /* bosMutexAcquire */

/***************************************************************************/

BOS_STATUS bosMutexTimedAcquire( BOS_MUTEX *mutex, BOS_TIME_MS timeoutMsec )
{
   BOS_STATUS status;

   BOS_ASSERT( gBosMutexInitialized );
   
   // try locking the mutex when mutex is not owned by anybody OR the current task is not the owner
   if( current != mutex->owner )
   {
       // try to take the mutex
        status = bosSemTimedTake( (BOS_SEM*)&(mutex->mutex), timeoutMsec );
        if ( status != BOS_STATUS_OK )
       {
           return CheckMutexStatusLinuxKernel( status, BOS_MUTEX_ACQUIRE_FUNC_ID );
       }
       // got the mutex, then become the owner
       mutex->owner = current;
   }
   // allows the mutex to be taken recursively
   mutex->recurCnt++;
   return ( BOS_STATUS_OK );
} /* bosMutexTimedAcquire */

/***************************************************************************/

BOS_STATUS bosMutexRelease( BOS_MUTEX *mutex )
{
   BOS_STATUS status;

   BOS_ASSERT( gBosMutexInitialized );

   if( current == mutex->owner )
   {
       // a mutex should be given the same number of times as it was taken before it can be released
       mutex->recurCnt--;
       if(mutex->recurCnt == 0)
       {
         /* Disown the mutex */
         mutex->owner = NULL;

         // try to give the mutex
         status = bosSemGive( (BOS_SEM*)&(mutex->mutex) );
         if ( status != BOS_STATUS_OK )
         {
            mutex->owner = current;
            mutex->recurCnt = 1;
            return CheckMutexStatusLinuxKernel( status, BOS_MUTEX_RELEASE_FUNC_ID );
         }
      }
   }
   else
   {
      blogStrErr(( gBosBlog, "bosMutexRelease() - bosMutex is either not locked or is released by non-owner"));
      return ( BOS_STATUS_ERR );
   }
   return ( BOS_STATUS_OK );

} /* bosMutexRelease */

/**
 * @addtogroup bosMutexInternal
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

static BOS_STATUS CheckMutexStatusLinuxKernel( int err, BOS_MUTEX_FUNC_ID funcId )
{
   return bosErrorCheckStatusLinuxKernel( err,
                                      BOS_MAKE_ERROR_SOURCE( BOS_MUTEX_MODULE, funcId ));

} /* CheckMutexStatusLinuxKernel */

/** @} */

#endif   /* BOS_CFG_MUTEX */

