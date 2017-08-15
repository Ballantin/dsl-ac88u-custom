/****************************************************************************
*
* Copyright (c) 2002-2012 Broadcom Corporation
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
*      STUN Client transaction module.
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <bosTask.h>
#include <bosMutex.h>
#include <bosSleep.h>
#include <bosTime.h>
#include <bosSocket.h>
#include <ntcfg.h>
#include "./inc/stun.h"
#include "stuntx.h"
#include "stundbg.h"
#include "stuni.h"

/* ---- Public Variables ------------------------------------- */
/* ---- Private Constants and Types -------------------------- */
/* send responses locally for debugging purposes */

#define TXQ_MUTEX_ACQ  if ( BOS_STATUS_OK != bosMutexAcquire( &txqMtx ) ) { BOS_ASSERT( BOS_FALSE  ); }
#define TXQ_MUTEX_REL  if ( BOS_STATUS_OK != bosMutexRelease( &txqMtx ) ) { BOS_ASSERT( BOS_FALSE  ); }

#define STTX_TASK_STACK       NTCFG_STUN_TX_THREAD_STACK
#define STTX_TASK_PRIORITY    NTCFG_STUN_TX_THREAD_PRIORITY

#define STTX_INTVL_NUM    10    /* retransmission intervals */
#define STTX_SLEEP        10    /* 10 millisecond sleeping interval */

#define ERROR_REASON_401      "Unauthorized    " //error 401
#define ERROR_REASON_433      "Use TLS         " //error 433

/* 
** When using RFC 5389 retransmission algorithm, the retransmission of
** a STUN request will happen at intervals starting with RTO and doubling
** each time until Rc retransmissions (including the initial one) are reached.
** If no response has been received after the last retransmission plus Rm * RTO
** time, the request is considered timed out.
*/
#if NTCFG_STUN_RFC5389_RETRANS_SUPPORT
#   define STTX_INTVL_DEF_RTO   500  /* RTO for retransmission (msec) */
#   define STTX_INTVL_DEF_RC    7    /* Rc for retransmission (number of) */
#   define STTX_INTVL_DEF_RM    16   /* Rm for last retransmission (factor) */
#   define STTX_INTVL_MULT      2    /* Mult factor for calculation. */
#endif


/* transmission data block */
typedef struct TXDATA
{
   BOS_UINT8            txTimeIdx;  /* Index to retransmission time table */
   BOS_TIME_MS          etime;      /* Elasped time */
   BOS_TIME_MS          maxWait;    /* Max wait time before timeout */
   STUN_TXID            txid;       /* Transaction id */
   BOS_UINT32           reqHdl;     /* Request handle */
   STUN_RESCB           resCB;      /* Response Processing CB */
   BOS_UINT32           sid;        /* Socket id */
   STUN_TRANSPORT       dest;       /* Destination */
   STUN_HMAC_PWD        password;   /* Password for integrity check */
   void                 *pData;     /* Data to be sent */
   BOS_UINT16           len;        /* Data length in number of bytes */
   BOS_UINT8            resCnt;     /* Response count */
   STUN_OPTS            options;    /* Send options */
   void                 *pOpaq;     /* Opaque data to pass back in resCB */
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
   BOS_SEM              *pSyncdResSem;  /* Synchrous response semaphore */
#endif
   struct TXDATA        *pNext;     /* Linked list next pointer */
} TXDATA;


/* ---- Private Variables ------------------------------------ */
static BOS_TASK_ID txTaskId = (BOS_TASK_ID)NULL;
/* transmission queue mutex */
static BOS_MUTEX txqMtx;
/* retransmission time intervals in ms */
#if NTCFG_STUN_RFC5389_RETRANS_SUPPORT
/* This table is populated during initialization of the module.
*/
BOS_TIME_MS txTime[STTX_INTVL_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
BOS_UINT8 txTimeMaxRetIdx = 0;
#else
/* this table uses fixed values per RFC 3489.
*/
BOS_TIME_MS txTime[STTX_INTVL_NUM] = {0, 100, 200, 400, 800, 1600,
                                    1600, 1600, 1600, 1600};
#endif

/* transmission queue */
static TXDATA *pTxDataQ = NULL;

static BOS_BOOL bRandSeeded = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------- */
static void stuntxThread( void );
static void qTxData( TXDATA *pData );
static void deqTxData( TXDATA *pData );
static void rmTxData( TXDATA *pTxData );
static void txidSeed( void );
static BOS_UINT32 txidRand( void );
#if NTCFG_STUN_RFC5389_RETRANS_SUPPORT
static void txRetransInit( const STUN_RETRANS *pRetData );
#endif


/* ---- Functions -------------------------------------------- */

/*****************************************************************************
*
*  FUNCTION:   stuntx_Init
*
*  PURPOSE:    Initialize STUN transaction module.
*
*  PARAMETERS: none
*
*  RETURNS:    BOS_TRUE if initialization OK. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stuntx_Init( void )
{
   /* create transmission thread */
#if BOS_CFG_RESET_SUPPORT
   if ( BOS_STATUS_OK != bosTaskCreateEx( "STTX",
                                          STTX_TASK_STACK,
                                          STTX_TASK_PRIORITY,
                                          NULL,
                                          (BOS_TASK_ENTRY)stuntxThread,
                                          NULL,
                                          (BOS_TASK_ARG *)NULL,
                                          &txTaskId ))
#else
   if ( BOS_STATUS_OK != bosTaskCreate  ( "STTX",
                                          STTX_TASK_STACK,
                                          STTX_TASK_PRIORITY,
                                          (BOS_TASK_ENTRY)stuntxThread,
                                          (BOS_TASK_ARG *)NULL,
                                          &txTaskId ))
#endif
   {
      DLOG(( "stuntx: transmision task creation failed" ));
      return( BOS_FALSE );
   }

   /* create mutex for transmission queuing */
   if (BOS_STATUS_OK != bosMutexCreate( "STQM", &txqMtx ) )
   {
      DLOG(( "stuntx: mutex creation failed" ));
      bosTaskDestroy( &txTaskId );
      return( BOS_FALSE );
   }
#if BOS_CFG_RESET_SUPPORT
   bosTaskSetNoReset( &txTaskId );
#endif
   txidSeed();

   return( BOS_TRUE );
}

/*****************************************************************************
*
*  FUNCTION:   stuntx_Deinit
*
*  PURPOSE:    De-initialize STUN transaction module.
*
*  PARAMETERS: none
*
*  RETURNS:    BOS_TRUE if de-initialization OK. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stuntx_Deinit( void )
{
   BOS_STATUS status;
   /* acquire mutex to take control over the tx linked list */
   TXQ_MUTEX_ACQ;
   /* clean up linked list and free memory*/
   while ( NULL != pTxDataQ )
   {
      rmTxData( pTxDataQ );
   }
   TXQ_MUTEX_REL;

   status = bosTaskDestroy( &txTaskId );
   if ( status != BOS_STATUS_OK )
   {
      DLOG(( "stuntx: Failed to destroy STTX thread" ));
      return ( BOS_FALSE );
   }

   /* Destroy mutex */
   status = bosMutexDestroy( &txqMtx );
   if ( status != BOS_STATUS_OK )
   {
      DLOG(( "stuntx: Failed to destroy Mutex STQM" ));
      return ( BOS_FALSE );
   }

   return( BOS_TRUE );
}

/*****************************************************************************
*
*  FUNCTION:   stuntx_GenenerateId
*
*  PURPOSE:    Generate new transaction id
*
*  PARAMETERS:
*           pMsg  [MOD] - STUN message pointer
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void stuntx_GenerateId( STUN_TXID *pTxid )
{
   int i;
   BOS_UINT32 randVal;
   BOS_UINT32 *ptr;

   BOS_ASSERT( NULL != pTxid );
   ptr = (BOS_UINT32 *)&pTxid->octet[0];
   /* construct the 128-bit (16 octets) transaction id */
#if NTCFG_STUN_MAGIC_COOKIE_SUPPORT
   *ptr++ = bosSocketNtoHL(STUN_MAGIC_COOKIE);
   for ( i = 1; i < 4; i++ )
#else
   for ( i = 0; i < 4; i++ )
#endif
   {
      if ( NULL != stunRand32CB )
      {
         (*stunRand32CB)( &randVal );
      }
      else
      {
         randVal = txidRand();   /* 32-bit random number */
      }

      *ptr++ = randVal;
   }

   DLOG(( "stuntx_GenerateId: transaction id %lx %lx %lx %lx",
          *(BOS_UINT32*)&pTxid->octet[0],
          *(BOS_UINT32*)&pTxid->octet[4],
          *(BOS_UINT32*)&pTxid->octet[8],
          *(BOS_UINT32*)&pTxid->octet[12] ));
}

/*****************************************************************************
*
*  FUNCTION:   stuntx_Send
*
*  PURPOSE:    Send STUN message.
*              For UDP, request is queued for retransmission handling.
*
*  PARAMETERS:
*           type        [IN] - Transaction type
*           sid         [IN] - Socket id
*           pDest       [IN] - Destination address
*           pBuf        [IN] - Buffer containing data
*           len         [IN] - Buffer length
*           pTxid       [IN] - Transaction id
*           reqHdl      [IN] - Request handle
*           resCB       [IN] - Response processing callback
*           pSyncdResData [MOD] - Synchrous response data buffer
*           pRetxData   [IN] - Retransmission configuration
*           pPassword   [IN] - Password (not applicable for shared secret)
*           options     [IN] - Send options
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stuntx_Send( const BOS_UINT32             sid,
                      const STUN_TRANSPORT         *pDest,
                      const BOS_UINT8              *pBuf,
                      const BOS_UINT16             len,
                      const STUN_TXID              *pTxid,
                      const BOS_UINT32             reqHdl,
                      const STUN_RESCB             resCB,
                      const STUN_RETRANS           *pRetxData,
                      const STUN_HMAC_PWD          *pPassword,
                      const STUN_OPTS              options,
                      const void                   *pOpaq )
{
   TXDATA *pTxData;
   char err[] = "System memory low. Request not sent";
   STUN_RES_DATA resData;
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
   BOS_SEM  resSem;
   BOS_SEM  *pSyncdResSem = NULL;
   BOS_STATUS status;
   memset( &resSem, 0, sizeof(BOS_SEM) );
#endif

   memset( &resData, 0, sizeof(STUN_RES_DATA) );
   resData.options = options;
   resData.pOpaq = (void *)pOpaq;

#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
   if ( (options & STUN_OPT_SYNCD_RES))
   {
      /* create a semaphore for waiting for response */
      if ( BOS_STATUS_OK != bosSemCreate( "SESC", 0, 1, &resSem ) )
      {
         return( BOS_FALSE );
      }
      else
      {
         pSyncdResSem = &resSem;
      }
   }
#endif

   pTxData = (TXDATA *)malloc( sizeof( TXDATA ) );
   if ( NULL == pTxData )
   {
      LOG(("stuntx_Send: %s", err));
      resData.evt = STUN_SYSTEM_ERR;
      (*resCB)( reqHdl, &resData );
      return (BOS_FALSE);
   }
   memset( pTxData, 0, sizeof( TXDATA ) );

   pTxData->pData = malloc( len );
   if ( NULL == pTxData->pData )
   {
      LOG(("stuntx_Send: %s", err));
      resData.evt = STUN_SYSTEM_ERR;
      (*resCB)( reqHdl, &resData );
      free( pTxData );
      return (BOS_FALSE);
   }
   memset( pTxData->pData, 0, len );

   pTxData->txTimeIdx = 0;
   pTxData->etime = 0;
   pTxData->maxWait = (pRetxData->uMaxWaitTime!=0)?
                        pRetxData->uMaxWaitTime:BOS_WAIT_FOREVER;
   pTxData->resCnt = 0;

#if NTCFG_STUN_RFC5389_RETRANS_SUPPORT
   /* Init retransmission parameters per configuration */
   txRetransInit( pRetxData );
#endif

   memcpy( &pTxData->txid, pTxid, sizeof( STUN_TXID ) );

   if ( (NULL != pPassword) && (pPassword->len > 0))
   {
      memcpy( &pTxData->password, pPassword, sizeof( STUN_HMAC_PWD ) );
   }
   else
   {
      pTxData->password.len = 0;
   }

   pTxData->sid = sid;
   memcpy( &pTxData->dest, pDest, sizeof( STUN_TRANSPORT  ) );
   memcpy( pTxData->pData, pBuf, len );
   pTxData->len = len;
   pTxData->options = options;
   pTxData->reqHdl = reqHdl;
   pTxData->resCB = resCB;
   pTxData->pOpaq = (void *)pOpaq;

#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
   pTxData->pSyncdResSem = pSyncdResSem;
#endif

   /* queue data for sending */
   qTxData( pTxData);

#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
   if ( (options & STUN_OPT_SYNCD_RES))
   {
      status = bosSemTake( pSyncdResSem );

      if ( (status == BOS_STATUS_OK) )
      {
         bosSemDestroy( pSyncdResSem );
      }    
      else
      {
         /* reset or err */
         /* still try to destroy the semaphore - do not assert */
         bosSemDestroy( pSyncdResSem );
         return( BOS_FALSE );
      }
   }
#endif

   return ( BOS_TRUE );
}

/*****************************************************************************
*
*  FUNCTION:   stuntx_Authenticate
*
*  PURPOSE:    First check whether the transaction id in the
*              response/err matches any transmitted requests.
*              Then check for message Integrity if it was used.
*              Stop retransmission if checks passed.
*
*  PARAMETERS:
*           pData    [IN] - STUN message
*           pMsg     [IN] - parsed stun message
*           hmLen    [IN] - hmac msg length
*           pResCnt  [OUT] - Re-send count
*           pReqHdl  [OUT] - Request handle
*           pEvtCB   [OUT] - Event CB
*           pReqDest [OUT] - Request destination
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stuntx_Authenticate( const void        *pData,
                              const BOS_UINT8   *pHash,
                              const BOS_UINT16  hmLen,
                              BOS_UINT32        *pReqHdl,
                              BOS_UINT8         *pResCnt,
                              BOS_UINT32        *pReqSid,
                              STUN_TRANSPORT    **ppReqDest,
                              void              **ppOpaq,
                              STUN_RESCB        *pResCB,
                              STUN_OPTS         *pOptions
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
                              ,
                              BOS_SEM           **ppSyncdResSem
#endif
                              )
{
   BOS_BOOL rc = BOS_FALSE;
   TXDATA *pTmp;

   BOS_ASSERT( NULL != pData );
   BOS_ASSERT( NULL != pResCnt );
   BOS_ASSERT( NULL != pReqHdl );
   BOS_ASSERT( NULL != pResCB );
   BOS_ASSERT( NULL != pReqSid );
   BOS_ASSERT( NULL != ppReqDest );

   if ( NULL != pData )
   {
      TXQ_MUTEX_ACQ;
      for ( pTmp = pTxDataQ; pTmp != NULL; pTmp = pTmp->pNext )
      {
         if ( memcmp( &pTmp->txid, &(((STUN_MSG *)pData)->msgHdr.id), sizeof(STUN_TXID) ) == 0 )
         {
            DLOG(("stuntx_Authenticated: transaction id Matched"));
            if ( pTmp->password.len > 0 )
            {
               if ( 0 != hmLen )
               {
                  if ( !stunIntegrityCheck( pData,
                                            pHash,
                                            hmLen,
                                            &pTmp->password ) )
                  {
                     /* integrity check failed, discard response */
                     DLOG(("stuntx_Authenticated: Failed"));
                     break;
                  }
               }
               else
               {
                  /* per RFC3489, request had message-integrity,
                   * missing message integrity in response,
                   * discard response
                   */
                  DLOG(("stuntx_Authenticated: Message integrity Missing. Discard response"));
                  break;
               }
            }

            *pResCnt = pTmp->resCnt;
            *pReqHdl = pTmp->reqHdl;
            *pResCB = pTmp->resCB;
            *ppReqDest = &pTmp->dest;
            *pReqSid = pTmp->sid;
            *ppOpaq = pTmp->pOpaq;
            *pOptions = pTmp->options;
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
            *ppSyncdResSem = pTmp->pSyncdResSem;
#endif

            /* stop retransmission */
            if ( pTmp->resCnt == 0 )
            {
               /* set */
               pTmp->etime = 0;
            }
            /* increment response count */
            pTmp->resCnt++;

            DLOG(("stuntx_Authenticated: Passed"));
            rc = BOS_TRUE;
            break;
         }
      }
      TXQ_MUTEX_REL;
   }

   return ( rc );
}


/*****************************************************************************
*
*  FUNCTION:   stuntx_CancelRequest
*
*  PURPOSE:    Cancels an outstanding request being sent.  Triggers a
*              time out event for the request if it was still queued up
*              for retransmission.
*
*  PARAMETERS:
*              uReqHdl  [IN] - Request handle
*
*  RETURNS:    BOS_TRUE on success, BOS_FALSE on failure.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stuntx_CancelRequest( const BOS_UINT32 uReqHdl )
{
   TXDATA *pTmp;
   TXDATA *pNext;
   BOS_BOOL rc = BOS_FALSE;

   TXQ_MUTEX_ACQ;
   for ( pTmp = pTxDataQ; pTmp != NULL; pTmp = pNext )
   {
      pNext = pTmp->pNext;

      if ( ( pTmp->reqHdl == uReqHdl ) &&
           ( pTmp->resCnt == 0 ) )
      {
         STUN_RES_DATA resData;
         memset( &resData, 0, sizeof(STUN_RES_DATA) );
         resData.evt = STUN_TIMEOUT;
         resData.pOpaq = pTmp->pOpaq;
         resData.options = pTmp->options;

         (*pTmp->resCB)( pTmp->reqHdl, &resData );

         rmTxData( pTmp );
         rc = BOS_TRUE;
      }
   }
   TXQ_MUTEX_REL;

   return ( rc );
}


/*****************************************************************************
*
*  FUNCTION:   stuntxThread
*
*  PURPOSE:    This task transmits queued data into network.
*
*  PARAMETERS:
*              none
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static void stuntxThread( void )
{
   TXDATA *pTmp;
   TXDATA *pNext;

   for ( ;; )
   {
      (void)bosSleep( STTX_SLEEP );

      TXQ_MUTEX_ACQ;
      for ( pTmp = pTxDataQ; pTmp != NULL; pTmp = pNext )
      {
#if STUNDBG_LOG
         char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
#endif
         pNext = pTmp->pNext;
         if ( pTmp->resCnt == 0 )
         {
            /* No response receive yet */
            BOS_BOOL bTimeout = BOS_FALSE;

            pTmp->etime += STTX_SLEEP;
            if ( pTmp->etime > pTmp->maxWait )
            {
               bTimeout = BOS_TRUE;
            }
            else
            {
               if ( pTmp->etime >= txTime[pTmp->txTimeIdx] )
               {
#if NTCFG_STUN_RFC5389_RETRANS_SUPPORT
                  if ( pTmp->txTimeIdx < txTimeMaxRetIdx )
#else
                  if ( pTmp->txTimeIdx < (STTX_INTVL_NUM - 1) )
#endif
                  {
                     if ( !(pTmp->options & STUN_OPT_RETRANSMIT_DISABLE ) ||
                           (0 == pTmp->txTimeIdx ) )
                     {
#if STUNDBG_LOG
                        (void)bosIpAddrGetStr(&pTmp->dest.ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
                        DLOG(( "stuntxThread: send packet sid [%lx], dest[addr %s, port %d], len[%d]", pTmp->sid, ipAddrStr, pTmp->dest.port, pTmp->len ));
#endif

#if !STUNDBG_LOCAL_RESPONSE
                        (*stunSendPktCB)( pTmp->sid, &pTmp->dest, pTmp->pData, pTmp->len );
#else
                        TXQ_MUTEX_REL;
                        stundbg_GenerateLocalResponse( pTmp->sid, &pTmp->dest, pTmp->pData, pTmp->len );
                        TXQ_MUTEX_ACQ;
#endif
                     }

                     pTmp->etime = 0;
                     pTmp->txTimeIdx++;
                  }
                  else
                  {
                     /* at the last time out, remove from transmission
                      * queue and notify app
                      */
                     DLOG(("stuntxTread: no response received upon last retransmission"));
                     bTimeout = BOS_TRUE;
                  }
               }
            }

            if ( BOS_TRUE == bTimeout )
            {
               STUN_RES_DATA resData;
               memset( &resData, 0, sizeof(STUN_RES_DATA) );
               resData.evt = STUN_TIMEOUT;
               resData.pOpaq = pTmp->pOpaq;
               resData.options = pTmp->options;
#if STUNDBG_LOG
               (void)bosIpAddrGetStr(&pTmp->dest.ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
               DLOG(( "stuntxThread: TIMEOUT no response received. sid [%lx], dest [addr %s, port %d], txid[%s]",
                     pTmp->sid,
                     ipAddrStr,
                     pTmp->dest.port,
                     stuntx_IdStr( &pTmp->txid ) ));
#endif

               (*pTmp->resCB)( pTmp->reqHdl, &resData );

               rmTxData( pTmp );
            }
         }
         else
         {
            rmTxData( pTmp );
            continue;
         }
      }
      TXQ_MUTEX_REL;
   }
}

/*****************************************************************************
*
*  FUNCTION:   rmTxData
*
*  PURPOSE:    remove TxData from retransmission queue
*
*  PARAMETERS:
*           pTxData [IN] - Tx Data
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static void rmTxData( TXDATA *pTxData )
{
   STUN_RES_DATA resData;
   memset( &resData, 0, sizeof(STUN_RES_DATA) );
   resData.options = pTxData->options;
   resData.pOpaq = pTxData->pOpaq;
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
   /* Semaphore should have been released */
   if ( NULL != pTxData->pSyncdResSem )
   {
      resData.evt = STUN_SYSTEM_ERR;
      (*pTxData->resCB)( pTxData->reqHdl, &resData );
      if ( pTxData->resCnt == 0 )
      {
         bosSemGive( pTxData->pSyncdResSem );
      }
   }
#endif

   /* remove request from tx queue */
   deqTxData( pTxData );

   free( pTxData->pData );
   pTxData->pData = NULL;
   free( pTxData );
   pTxData = NULL;
}

/*****************************************************************************
*
*  FUNCTION:   qTxData
*
*  PURPOSE:    Queue retransmission data.
*
*  PARAMETERS:
*           pData [IN] - retransmission data
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static void qTxData( TXDATA *pData )
{
   BOS_ASSERT( NULL != pData );

   TXQ_MUTEX_ACQ;

   /* insert to the beginning of the queue */
   if ( NULL != pTxDataQ )
   {
      pData->pNext = pTxDataQ;
      pTxDataQ = pData;
   }
   else
   {
      /* nothing in queue */
      pTxDataQ = pData;
      pData->pNext = NULL;
   }

   TXQ_MUTEX_REL;
}

/*****************************************************************************
*
*  FUNCTION:   deqTxData
*
*  PURPOSE:    Dequeue retransmission data.
*
*  PARAMETERS:
*           data [IN] - retransmission data
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static void deqTxData( TXDATA *pData )
{
   TXDATA *pTmp;
   TXDATA *pPrev = NULL;
   BOS_ASSERT( NULL != pData );

   TXQ_MUTEX_ACQ;

   for ( pTmp = pTxDataQ; pTmp != NULL; pTmp = pTmp->pNext )
   {
      if ( pData == pTmp )
      {
         /* found, remove from queue */
         if ( pPrev == NULL )
         {
            if ( pData->pNext != NULL )
            {
               /* first in queue */
               pTxDataQ = (TXDATA *)pData->pNext;
            }
            else
            {
               /* only one in queue */
               pTxDataQ = NULL;
            }
         }
         else if ( pData->pNext == NULL )
         {
            /* last in queue */
            pPrev->pNext = NULL;
         }
         else
         {
            /* middle of the queue */
            pPrev->pNext = pData->pNext;
         }
         break;
      }
      pPrev = pTmp;
   }

   TXQ_MUTEX_REL;
}

/*****************************************************************************
*  FUNCTION:
*     txidSeed
*
*  PURPOSE:
*     Seeds the 32-bit randome generator
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     32-bit random value
*
*  NOTES:
*
*****************************************************************************/
static void txidSeed( void )
{
   BOS_TIME_MS tick;
   BOS_UINT32 seed;

   bosTimeGetMs( &tick );

   seed = (BOS_UINT32)tick;

   srand(seed);

   bRandSeeded = BOS_TRUE;
}

/*****************************************************************************
*  FUNCTION:
*     txidRand
*
*  PURPOSE:
*     32-bit randome generator
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     32-bit random value
*
*  NOTES:
*
*****************************************************************************/
static BOS_UINT32 txidRand( void )
{
   BOS_UINT16 upper;
   BOS_UINT16 lower;

   if ( BOS_TRUE != bRandSeeded )
   {
      txidSeed();  /* Just seed it if it is not seeded */
   }

   /* Depending on the random generator, the value may only have 16-bit
    * of randomness
    */
   upper = (BOS_UINT16)rand();
   lower = (BOS_UINT16)rand();

   return((BOS_UINT32)((upper<<16) | lower));
}

#if STUNDBG_LOG
/*****************************************************************************
*  FUNCTION:
*     stuntx_IdStr
*
*  PURPOSE:
*     return the string representation of the txid.
*
*  PARAMETERS:
*
*  RETURNS:
*     reqHdl
*
*  NOTES:
*
*****************************************************************************/
char *stuntx_IdStr( STUN_TXID *pTxid )
{
   static char tmp[40];
   BOS_ASSERT( NULL != pTxid );
   sprintf( tmp, "%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x",
            pTxid->octet[0],pTxid->octet[1],pTxid->octet[2],pTxid->octet[3],
            pTxid->octet[4],pTxid->octet[5],pTxid->octet[6],pTxid->octet[7],
            pTxid->octet[8],pTxid->octet[9],pTxid->octet[10],pTxid->octet[11],
            pTxid->octet[12],pTxid->octet[13],pTxid->octet[14],pTxid->octet[15]);
   return tmp;
}

#endif

#if NTCFG_STUN_RFC5389_RETRANS_SUPPORT
/*****************************************************************************
*  FUNCTION:
*     txRetransInit
*
*  PURPOSE:
*     Initializes the retransmission algorithm time table based on the module
*     configuration information.
*
*  PARAMETERS:
*     pRetData - the retransmission data configuration default to be used for
*                the algorithm setup.
*
*  RETURNS:
*     Nothing.
*
*  NOTES:
*     When a configuration value is deemed out of range of the capabilities
*     of the support offered, the algorithm will default to the one defined
*     in RFC 5389.
*
*****************************************************************************/
void txRetransInit( const STUN_RETRANS *pRetData )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uMult;

   BOS_UINT32 uRto = STTX_INTVL_DEF_RTO;
   BOS_UINT32 uRc = STTX_INTVL_DEF_RC;
   BOS_UINT32 uRm = STTX_INTVL_DEF_RM;

   BOS_ASSERT( STTX_INTVL_NUM >= 1 );

   if ( pRetData )
   {
      /* Validate range for RTO, enforce minimum value.
      */
      if ( pRetData->uRto && ( pRetData->uRto > STTX_INTVL_DEF_RTO ) )
      {
         uRto = pRetData->uRto;
      }

      /* Validate range for the number of retransmission count.
      */
      if ( pRetData->uRc && ( pRetData->uRc < STTX_INTVL_NUM - 1 ) )
      {
         uRc = pRetData->uRc;
      }

      /* Validate range for final time out.
      */
      if ( pRetData->uRm )
      {
         uRm = pRetData->uRm;
      }
   }

   /* Fill in the retransmission table with the correct values.
   */
   txTime[ 0 ] = 0;
   txTime[ 1 ] = uRto;
   uMult = 2;
   for ( uIx = 2 ; uIx < uRc ; uIx++ )
   {
      txTime[ uIx ] = txTime[ uIx - 1 ] + ( uMult * uRto ); 
      uMult *= STTX_INTVL_MULT;
   }
   txTime[ uRc ] = uRm * uRto;
   txTimeMaxRetIdx = (BOS_UINT8) uRc;
}
#endif
