/****************************************************************************
*  
*  Copyright (c) 2000-2008 Broadcom Corporation
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
*    Filename: lhapiOS.c
*
****************************************************************************
*    Description:
*
*     Implements the "Low-level HAPI" API that is required to provide
*     common application supports (such as semas, error logging) to LDX
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <lhapi.h>
#include <vrgLog.h>
#include <stdio.h>

#include <bosSem.h>
#include <xchgAssert.h>

#include <bosCritSect.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
#define LHAPI_NUM_SEMAPHORES_MAX   50

/* ---- Private Variables ------------------------------------------------- */
static BOS_SEM lhapi_gBosSem[LHAPI_NUM_SEMAPHORES_MAX];
static unsigned lhapi_gSemCount=0;

/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   lhapiOsSemCreate
**
** PURPOSE:    Creates a semaphore
**
** PARAMETERS: ppSem - pointer to created semaphore
**
** RETURNS:    HAPISUCCESS / HAPIFAILURE
**
*****************************************************************************
*/
int lhapiOsSemCreate( void **ppSem )
{
   BOS_STATUS  status;
   char        semName[12];

   if( lhapi_gSemCount >=  LHAPI_NUM_SEMAPHORES_MAX )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapi: Attempting to allocate too many semaphores: Max %u",
                 LHAPI_NUM_SEMAPHORES_MAX ));

      return( HAPIFAILURE );
   }

   /*
   ** Create a unique semaphore name.
   */
   sprintf( semName, "semHapi%u", lhapi_gSemCount );

   status = bosSemCreate( semName, 0, 1, &lhapi_gBosSem[lhapi_gSemCount] );
   if( status == BOS_STATUS_OK )
   {
      *ppSem = &lhapi_gBosSem[lhapi_gSemCount];

      lhapi_gSemCount++;
      return( HAPISUCCESS );
   }

   return( HAPIFAILURE );
}


/*
*****************************************************************************
** FUNCTION:   lhapiOsSemGive
**
** PURPOSE:    Give a semaphore
**
** PARAMETERS: pSem - semaphore to give
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
int lhapiOsSemGive( void *pSem )
{
   BOS_STATUS  status;

   status = bosSemGive( (BOS_SEM *)pSem );

   return ( status );
}


/*
*****************************************************************************
** FUNCTION:   lhapiOsSemTake
**
** PURPOSE:    Take a semaphore
**
** PARAMETERS: pSem - semaphore to take
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
int lhapiOsSemTake( void *pSem, int timeoutMs )
{
   BOS_STATUS  status;

   status = bosSemTimedTake( (BOS_SEM *)pSem, (BOS_TIME_MS) timeoutMs );

   return ( status );
}


/*
*****************************************************************************
** FUNCTION:   lhapiOsSemReset
**
** PURPOSE:    Reset a semaphore
**
** PARAMETERS: pSem - semaphore to take
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
int lhapiOsSemReset( void *pSem )
{
      return( HAPISUCCESS );
   }

/*
*****************************************************************************
** FUNCTION:   lhapiOsSemDestroy
**
** PURPOSE:    Destroy all the semaphores and reset the counter
**
** PARAMETERS: none
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
void lhapiOsSemDestroy(void)
   {
   int i;

   for(i = 0; i < lhapi_gSemCount; i++)
      {
      bosSemDestroy( &lhapi_gBosSem[i] );  
   }

   /* Reset semaphore counter */
   lhapi_gSemCount = 0;   
}
