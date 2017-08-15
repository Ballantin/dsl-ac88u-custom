/****************************************************************************
*
* Copyright (c) 2006-2011 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Description:
*
*      TURN Client request sending refresh.
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <bosTask.h>
#include <bosMutex.h>
#include <bosSleep.h>
#include <turnc.h>
#include <stun.h>
#include "turn.h"
#include "turnci.h"
#include "turntx.h"
#include "turnrx.h"
#include "turndbg.h"

/* ---- Public Variables ------------------------------------- */
/* ---- Private Constants and Types -------------------------- */
#define TRSH_SLEEP        1000    /* 1000 millisecond sleeping interval */

#define TRSH_TASK_STACK       (6 *1024)
#define TRSH_TASK_PRIORITY    BOS_TASK_CLASS_MED

#define RFQ_MUTEX_ACQ  if ( BOS_STATUS_OK != bosMutexAcquire( &rfqMtx ) ) { BOS_ASSERT( BOS_FALSE  ); }
#define RFQ_MUTEX_REL  if ( BOS_STATUS_OK != bosMutexRelease( &rfqMtx ) ) { BOS_ASSERT( BOS_FALSE  ); }
#define ADL_MUTEX_ACQ  if ( BOS_STATUS_OK != bosMutexAcquire( &activeDestListMtx ) ) { BOS_ASSERT( BOS_FALSE  ); }
#define ADL_MUTEX_REL  if ( BOS_STATUS_OK != bosMutexRelease( &activeDestListMtx ) ) { BOS_ASSERT( BOS_FALSE  ); }

#define REFRESH_PENDING       0
#define REFRESH_INPROGRESS    1
#define REFRESH_PURGE         2
#define REFRESH_TERMINATE     3

#define REFRESH_TIMEOUT_DEFAULT   30

/* Active destination states */
#define AD_NONESET            0
#define AD_TRANSITIONING      1
#define AD_SET                2

/* Message refresh data block */
typedef struct RFDATA
{
   BOS_UINT32           sid;        /* socket id */
   STUN_TRANSPORT       dest;       /* destination addr */
   BOS_UINT8            *pMsg;      /* TURN message to be refreshed */
   STUN_HMAC_PWD        password;   /* HMAC password for integrity */
   BOS_UINT8            status;     /* refreshing status */
   BOS_UINT32           reqHdl;     /* request handle */
   BOS_UINT32           refreshT;   /* refresh time in seconds */
   BOS_UINT32           refreshCnt; /* number of refresh performed */
   struct RFDATA        *pNext;     /* linked list next pointer */
} RFDATA;

typedef struct
{
   BOS_BOOL          used;          /* entry used or not used */
   BOS_UINT32        sid;         /* the socket id */
   STUN_TRANSPORT    destAddr;      /* the request destination
                                       i.e the server */
   STUN_TRANSPORT    remoteAddr;    /* the remote active Destination */
   BOS_UINT32        reqHdl;        /* request handle */
   int               state;         /* state of the request */
} ACTIVE_DEST;

/* ---- Private Variables ------------------------------------ */
static BOS_TASK_ID rfTaskId = (BOS_TASK_ID)NULL;
/* Refresh data queue */
static RFDATA *pRfDataQ = NULL;
/* Refresh queue mutex */
static BOS_MUTEX rfqMtx;

static ACTIVE_DEST activeDestList[TURNC_MAX_ACTIVE_DEST];
static BOS_MUTEX activeDestListMtx;

/* ---- Private Function Prototypes -------------------------- */
static void refreshThread( void );
static BOS_UINT32 setRefreshStatus( BOS_UINT32 reqHdl, BOS_UINT8 status, BOS_UINT32 rTime );
static BOS_BOOL refreshAllocate( RFDATA *tmp, BOS_UINT32 *pLifetime );

/* ---- Functions -------------------------------------------- */
/*****************************************************************************
*  FUNCTION:   turntx_Init
*
*  PURPOSE:    Initialize TURN request refresh module.
*
*  PARAMETERS:
*              none.
*
*  RETURNS:    BOS_TRUE if successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL turntx_Init( void )
{
   /* Start refresh thread */
   if ( BOS_STATUS_OK != bosTaskCreate( "TRSH",
                                        TRSH_TASK_STACK,
                                        TRSH_TASK_PRIORITY,
                                        (BOS_TASK_ENTRY)refreshThread,
                                        (BOS_TASK_ARG *)NULL,
                                        &rfTaskId ))
   {
      LOG(( "turntx_Init: failed to start refresh thread" ));
      return( BOS_FALSE );
   }
 
   /* Create mutex for refresh queue */
   if ( BOS_STATUS_OK != bosMutexCreate( "TRQM", &rfqMtx ) )
   {
      LOG(( "turntx_Init: failed to create refresh queue mutex" ));
      return( BOS_FALSE );
   }

   /* Create mutex for accessing active destination list */
   if ( BOS_STATUS_OK != bosMutexCreate( "TADM", &activeDestListMtx ) )
   {
      LOG(( "turncStartup: failed to create active Destination list mutex" ));
      return( BOS_FALSE );
   }

   memset( activeDestList, 0, TURNC_MAX_ACTIVE_DEST * sizeof(ACTIVE_DEST));

   return( BOS_TRUE );
}

/*****************************************************************************
*
*  FUNCTION:   refreshThread
*
*  PURPOSE:    Refresh Allocate Requests.
*
*  PARAMETERS:
*              none
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static void refreshThread( void )
{
   RFDATA *tmp;
   RFDATA *pNext;
   RFDATA *pPrev;

   for ( ;; )
   {
      (void)bosSleep( TRSH_SLEEP );

      RFQ_MUTEX_ACQ;

      pPrev = NULL;
      for ( tmp = pRfDataQ; tmp != NULL; tmp = pNext )
      {
         pNext = tmp->pNext;

         if ( tmp->status == REFRESH_PENDING )
         {
            if ( tmp->refreshT-- == 0 )
            {
               /*
                * No response from server, remove pending request
                */
               tmp->status = REFRESH_PURGE;
            }
         }

         if ( tmp->status == REFRESH_INPROGRESS )
         {
            if ( tmp->refreshT-- == 0 )
            {
               /* Refresh the ALLOCATE request */
               if (refreshAllocate( tmp, NULL ))
               {
                  tmp->refreshCnt++;

                  LOG(("Allocate request hdl [%d] refreshed", (int)tmp->reqHdl));
               }
               else
               {
                  LOG(("Failed to send the refreshed allocate request hdl [%d]", (int)tmp->reqHdl));
               }
            }
         }

         if ( tmp->status == REFRESH_TERMINATE )
         {
            BOS_UINT32 lifetime = 0;
            /* Explicitely send ALLOCATE with lifetime = 0 */
            refreshAllocate( tmp, &lifetime );
            tmp->status = REFRESH_PURGE;
         }

         if ( tmp->status == REFRESH_PURGE )
         {
            DLOG(("Allocate request hdl [%d] Removed", tmp->reqHdl));
            /* Remove itself from refresh Q */
            if (pPrev)
            {
               pPrev->pNext = tmp->pNext;
            }
            else
            {
               pRfDataQ = tmp->pNext;
            }

            /* Free resources */
            free(tmp->pMsg);
            free(tmp);
            tmp = NULL;
         }

         pPrev = tmp;
      }

      RFQ_MUTEX_REL;
   }
}

/*****************************************************************************
*  FUNCTION:   turntx_Deinit
*
*  PURPOSE:    De-initialize TRUN request refresh module.
*
*  PARAMETERS: none
*
*  RETURNS:    BOS_TRUE if de-initialization OK. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL turntx_Deinit( void )
{
   BOS_STATUS status;
   RFDATA *tmp;

   /* Acquire mutex to take control over the tx linked list */
   RFQ_MUTEX_ACQ;
   /* Clean up linked list and free memory */
   while ( NULL != pRfDataQ )
   {
      tmp = pRfDataQ;
      pRfDataQ = tmp->pNext;
      free(tmp->pMsg);
      free(tmp);
   }
   RFQ_MUTEX_REL;

   /* Delete refresh thread */
   status = bosTaskDestroy( &rfTaskId );
   if ( status != BOS_STATUS_OK )
   {
      DLOG(( "turntx: Failed to destroy TRSH thread" ));
      return ( BOS_FALSE );
   }

   /* Destroy mutex */
   status = bosMutexDestroy( &rfqMtx );
   if ( status != BOS_STATUS_OK )
   {
      DLOG(( "stuntx: Failed to destroy Mutex TRQM" ));
      return ( BOS_FALSE );
   }

   /* Destroy mutex */
   if ( BOS_STATUS_OK != bosMutexDestroy( &activeDestListMtx ) )
   {
      DLOG(( "turncShutdown: Failed to destroy Mutex TADM" ));
      return ( BOS_FALSE );
   }

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   turntx_Pending
*
*  PURPOSE:    Insert a pending entry to the refresh queue
*
*  PARAMETERS:
*              reqHdl [IN] - request handle
*              pMsg [IN] - request handle
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void turntx_Pending( const BOS_UINT32 sid, const STUN_TRANSPORT  *pDest, BOS_UINT32 reqHdl, BOS_UINT8 *pMsg, STUN_HMAC_PWD *pPwd )
{
   RFDATA *tmp;
   BOS_ASSERT( NULL != pDest &&
               NULL != pMsg &&
               NULL != pPwd );

   tmp = (RFDATA *)malloc( sizeof(RFDATA) );
   memset( tmp, 0, sizeof(RFDATA) );
   tmp->pMsg = (BOS_UINT8 *)malloc( STUN_MSG_BUFFER_SIZE );
   memcpy( tmp->pMsg, pMsg, STUN_MSG_BUFFER_SIZE );
   tmp->sid = sid;
   tmp->dest = *pDest;
   tmp->password = *pPwd;
   tmp->status = REFRESH_PENDING;
   tmp->reqHdl = reqHdl;
   tmp->refreshT = REFRESH_TIMEOUT_DEFAULT;

   /* insert into refresh queue */
   RFQ_MUTEX_ACQ;
   tmp->pNext = pRfDataQ;
   pRfDataQ = tmp;
   RFQ_MUTEX_REL;
}

/*****************************************************************************
*  FUNCTION:   turntx_Refresh
*
*  PURPOSE:    Start refreshing of a request.
*
*  PARAMETERS: reqHdl [IN] - request handle
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
BOS_UINT32 turntx_Refresh( BOS_UINT32 reqHdl, BOS_UINT32 rTime )
{
   return setRefreshStatus( reqHdl, REFRESH_INPROGRESS, rTime );
}

/*****************************************************************************
*  FUNCTION:   turntx_Terminate
*
*  PURPOSE:    Send allocate with 0 lifetime and
*              remove request from the refresh Q.
*
*  PARAMETERS: reqHdl [IN] - request handle
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void turntx_Terminate( BOS_UINT32 reqHdl )
{
   (void)setRefreshStatus( reqHdl, REFRESH_TERMINATE, 0 );
}

/*****************************************************************************
*  FUNCTION:   turntx_Purge
*
*  PURPOSE:    Remove a request from the refresh Q.
*
*  PARAMETERS: reqHdl [IN] - request handle
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void turntx_Purge( BOS_UINT32 reqHdl )
{
   (void)setRefreshStatus( reqHdl, REFRESH_PURGE, 0 );
}

/*****************************************************************************
*  FUNCTION:   setRefreshStatus
*
*  PURPOSE:    Start refreshing of a request.
*
*  PARAMETERS:
*              reqHdl [IN] - request handle
*              status [IN] - refresh status
*              rTime [IN] - refresh time in seconds
*
*  RETURNS:    number of refreshes had performed.
*
*  NOTES:
*
*****************************************************************************/
static BOS_UINT32 setRefreshStatus( BOS_UINT32 reqHdl, BOS_UINT8 status, BOS_UINT32 rTime )
{
   RFDATA *tmp;
   BOS_UINT32 cnt = 0;

   RFQ_MUTEX_ACQ;

   for ( tmp = pRfDataQ; tmp != NULL; tmp = tmp->pNext )
   {
      if ( tmp->reqHdl == reqHdl )
      {
         /*
          * match found, set the refresh state
          */
         tmp->status = status;
         tmp->refreshT = rTime;
         cnt = tmp->refreshCnt;
         break;
      }
   }

   RFQ_MUTEX_REL;
   return( cnt );
}

/*****************************************************************************
*  FUNCTION:   refreshAllocate
*
*  PURPOSE:    refresh the queued ALLOCATE request
*
*  PARAMETERS:
*              tmp [IN] - the queued rfData
*              pLifetime [IN] - lifetime
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL refreshAllocate( RFDATA *tmp, BOS_UINT32 *pLifetime )
{
   BOS_UINT16 len;
   BOS_UINT8 msgBuf[STUN_MSG_BUFFER_SIZE];
   STUN_RETRANS reTx;

   /* get new transaction id */
   stunSetTransactionId( tmp->pMsg, NULL );
 
   /* re-encode msg. Since transaction id changes, HMAC will
    * also change
    */
   if ( NULL != pLifetime )
   {
      stunSetAttribute( tmp->pMsg, STUN_ATTR_LIFETIME, (void *)pLifetime );
   }

   /* Make a copy of the refresh msg buffer for encoding and sending */
   memcpy( msgBuf, tmp->pMsg, STUN_MSG_BUFFER_SIZE );
 
   /* encode message to a network packet */
   if ( BOS_TRUE != stunEncode( msgBuf, &len, &tmp->password, 0 ) )
   {
      return( BOS_FALSE );
   }

   memset ( &reTx,
            0,
            sizeof ( STUN_RETRANS ) );

   /* queue the packet for sending into network */
   if ( BOS_TRUE != stunSendRequest( tmp->sid,
                                     &tmp->dest,
                                     msgBuf,
                                     len,
                                     &tmp->reqHdl,
                                     turnrx_ResponseHandler,
                                     &reTx,
                                     &tmp->password,
                                     0,
                                     (void *)&turncEvtCB) )
   {
      return ( BOS_FALSE );
   }

   return ( BOS_TRUE );
}

/*****************************************************************************
*
*  FUNCTION:   turntxAddActiveDest
*
*  PURPOSE:    Add to the list of Active destinations. Return BOS_FALSE if
*              the list is already full.
*
*  PARAMETERS:
*           sid           [IN] - socket id for sending the request
*           pDest         [IN] - destination where the request is sent to
*           pRemoteAddr   [IN] - Remote IP port (network byte ordered)
*           reqHdl        [IN] - Request Handle
*
*  RETURNS:    BOS_TRUE if remote addr has already been set active,
*              BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL turntxAddActiveDest( const BOS_UINT32 sid, const STUN_TRANSPORT  *pDest, const STUN_TRANSPORT  *pRemoteAddr, const BOS_UINT32 reqHdl )
{
   int i;
   BOS_BOOL found = BOS_FALSE;

   BOS_ASSERT( NULL != pRemoteAddr );

   /* Check if the sid pDest pair already exists,
    * update the remoteAddr and reqHdl, and if it's a new
    * remoteAddr, set the state to SD_TRANSITIONING.
    *
    * If not exist, add new entry and set state to NOTSET
    */
   ADL_MUTEX_ACQ;
   for ( i = 0; i < TURNC_MAX_ACTIVE_DEST; i++ )
   {
      if ( activeDestList[i].used &&
           sid == activeDestList[i].sid &&
           !memcmp(pDest, &activeDestList[i].destAddr, sizeof(STUN_TRANSPORT )) )
      {
         activeDestList[i].reqHdl = reqHdl;
         if ( memcmp(pRemoteAddr, &activeDestList[i].remoteAddr, sizeof(STUN_TRANSPORT )))
         {
            activeDestList[i].remoteAddr = *pRemoteAddr;
            activeDestList[i].state = AD_TRANSITIONING;
         }
         found = BOS_TRUE;
         break;
      }
   }

   if ( !found )
   {
      /* look for a new empty slot */
      for ( i = 0; i < TURNC_MAX_ACTIVE_DEST; i++ )
      {
         if ( BOS_FALSE == activeDestList[i].used )
         {
            activeDestList[i].used = BOS_TRUE;
            activeDestList[i].sid = sid;
            activeDestList[i].destAddr = *pDest;
            activeDestList[i].remoteAddr = *pRemoteAddr;
            activeDestList[i].reqHdl = reqHdl;
            activeDestList[i].state = AD_NONESET;
            found = BOS_TRUE;
            break;
         }
      }
   }
   ADL_MUTEX_REL;
 
   if ( !found )
   {
      return( BOS_FALSE );
   }
   else
   {
      return( BOS_TRUE );
   }
}

/*****************************************************************************
*
*  FUNCTION:   turntxSetActiveDest
*
*  PURPOSE:    Activate a pending Active destinations request.
*              Return BOS_FALSE if request does not exist.
*
*  PARAMETERS:
*           sid           [IN] - socket id for sending the request
*           pDest         [IN] - destination where the request is sent to
*           reqHdl        [IN] - Remote IP port (network byte ordered)
*
*  RETURNS:    BOS_TRUE if remote addr has already been set active,
*              BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL turntxSetActiveDest( const BOS_UINT32 sid, const STUN_TRANSPORT  *pDest, const BOS_UINT32 reqHdl )
{
   int i;
   BOS_BOOL rc = BOS_FALSE;

   ADL_MUTEX_ACQ;
   for ( i = 0; i < TURNC_MAX_ACTIVE_DEST; i++ )
   {
      if ( (activeDestList[i].used) &&
           (activeDestList[i].sid == sid) &&
           !memcmp(&activeDestList[i].destAddr,
                           pDest, sizeof (STUN_TRANSPORT )) &&
           (activeDestList[i].reqHdl == reqHdl))
      {
         activeDestList[i].state = AD_SET;
         rc = BOS_TRUE;
         break;
      }
   }
   ADL_MUTEX_REL;

   return( rc );

}
/*****************************************************************************
*
*  FUNCTION:   turntxRemoveActiveDest
*
*  PURPOSE:    Remove from the list of Active destinations.
*
*  PARAMETERS:
*           sid           [IN] - socket id for sending the request
*           pDest         [IN] - destination where the request is sent to
*           pRemoteAddr   [IN] - Remote IP port (network byte ordered)
*
*  RETURNS:    BOS_TRUE if remote addr has already been set active,
*              BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
void turntxRemoveActiveDest( const BOS_UINT32 sid, const STUN_TRANSPORT  *pDest )
{
   int i;
   ADL_MUTEX_ACQ;
   for ( i = 0; i < TURNC_MAX_ACTIVE_DEST; i++ )
   {
      if ( activeDestList[i].used )
      {
         if ( (activeDestList[i].sid == sid) &&
              (!memcmp(&activeDestList[i].destAddr, pDest, sizeof(STUN_TRANSPORT ))) )
         {
            memset( &activeDestList[i], 0, sizeof(ACTIVE_DEST) );
            break;
         }
      }
   }
   ADL_MUTEX_REL;
}

/*****************************************************************************
*
*  FUNCTION:   turntxIsActiveDest
*
*  PURPOSE:    return TRUE if the remote destination has already been
*              set to active.
*              A remote destination is set to active after the Relay
*              Server accepts the SetActiveDestination Request of the
*              remote destination.
*
*  PARAMETERS:
*           sid           [IN] - socket id for sending the request
*           pDest         [IN] - destination where the request is sent to
*           pRemoteAddr   [IN] - Remote IP port (network byte ordered)
*
*  RETURNS:    BOS_TRUE if remote addr has already been set active,
*              BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL turntxIsActiveDest( const BOS_UINT32 sid, const STUN_TRANSPORT  *pDest, const STUN_TRANSPORT  *pRemoteAddr )
{
   int i;
   BOS_BOOL rc = BOS_FALSE;
   ADL_MUTEX_ACQ;
   for ( i = 0; i < TURNC_MAX_ACTIVE_DEST; i++ )
   {
      if ( activeDestList[i].used )
      {
         if ( (activeDestList[i].sid == sid) &&
              (!memcmp(&activeDestList[i].destAddr, pDest, sizeof(STUN_TRANSPORT ))) &&
              (!memcmp(&activeDestList[i].remoteAddr, pRemoteAddr, sizeof(STUN_TRANSPORT ))) )
         {
            if ( activeDestList[i].state == AD_SET )
            {
               rc = BOS_TRUE;
            }
            break;
         }
      }
   }
   ADL_MUTEX_REL;

   return( rc );
}
