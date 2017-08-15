/****************************************************************************
*
* Copyright © 2006-2008 Broadcom Corporation
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
*      ICE Agent.
*      
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <bosMutex.h>
#include <bosSleep.h>
#include <bosTask.h>
#include <stdio.h>
#include <ice.h>
#include <stunc.h>
#include <stun.h>
#include "iceagnt.h"
#include "icectty.h"
#include "icei.h"
#include "icecndt.h"
#include "icesdp.h"
#include "icedbg.h"
/* ---- Public Variables ------------------------------------- */

/* ---- Private Constants and Types -------------------------- */
#define AGNT_MUTEX_ACQ  if ( BOS_STATUS_OK != bosMutexAcquire( &agntMtx ) ) { BOS_ASSERT( BOS_FALSE  ); }
#define AGNT_MUTEX_REL  if ( BOS_STATUS_OK != bosMutexRelease( &agntMtx ) ) { BOS_ASSERT( BOS_FALSE  ); }

/* Time between sending STUN request in ms */ 
#define ICEAGNT_TA_DEFAULT          (BOS_TIME_MS)20

/* Candidate list servicing task parameters */
#define CDTS_SLEEP                  (BOS_TIME_MS)10
#define CDTS_TASK_STACK             (4 *1024)
#define CDTS_TASK_PRIORITY          BOS_TASK_CLASS_MED

/* Maximum username and password size */
#define ICEAGNT_USERNAME_LEN_MAX    128
#define ICEAGNT_PASSWORD_LEN_MAX    128

/* candidate priority defaults */
#define ICEAGNT_PRI_HOST_DEFAULT    126
#define ICEAGNT_PRI_SRFLX_DEFAULT   100
#define ICEAGNT_PRI_PRFLX_DEFAULT   110
#define ICEAGNT_PRI_RELAYED_DEFAULT 10

#define SERVER_RES_TIMEOUT          1
#define SERVER_RES_UNKNOWN          2

/* ---- Private Variables ------------------------------------ */
static BOS_MUTEX agntMtx;
static ICEAGNT *pAgntLstHead = NULL;
static BOS_UINT16 agntCount = 0;
static BOS_TASK_ID cndtLstSrvTaskId = (BOS_TASK_ID)0;
static BOS_TIME_MS cndtLstTa = ICEAGNT_TA_DEFAULT;

/* ---- Private Function Prototypes -------------------------- */
static void agntSrvThread( void );
static BOS_UINT32 newAgntId( void );
static BOS_BOOL isWaitingServerResponse( const ICEAGNT *pAngt, int *rc );
static void setAgentRole( ICEAGNT *pAgnt, const ICE_SDP_AGENT sdpAgent );
static void processCheckStatus( const BOS_UINT32 reqHdl, const STUN_TRANSPORT *pMappedAddr, BOS_UINT32 statusCode );
static BOS_BOOL isAllCompsNominated( const ICEAGNT *pAgnt );
static BOS_BOOL isAllChksAckd( const ICEAGNT *pAgnt, BOS_BOOL *pbFailed );
static BOS_BOOL nominateValidatedChk( ICEAGNT *pAgnt );
static BOS_BOOL selectChkPair( ICEAGNT *pAgnt );
static void fillMediaTransport( ICEAGNT *pAgnt, ICE_MEDIA_TRANSPORT *pMediaTransport );
static void ackPeerBindingRequest( const STUN_REQ_DATA *pData );
static ICE_STUN_SERVER *getStunServerFromId( const ICE_STUN_SERVER_LIST *pList,
                                             const BOS_UINT16 stunServerId );
/* ---- Functions -------------------------------------------- */

/*****************************************************************************
*  FUNCTION:   iceagnt_Init
*
*  PURPOSE:    Initialize ICE Agent module.
*
*  PARAMETERS:
*              none
*
*  RETURNS:    BOS_TRUE if configuration successful. BOS_FALSE
*              otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL iceagnt_Init( void )
{
   /* Create mutex for Agent linked list content manipulations */
   if ( BOS_STATUS_OK != bosMutexCreate( "AGNT", &agntMtx ) )
   {
      LOG(( "iceagnt_Init: failed to create Agent mutex" ));
      return( BOS_FALSE );
   }

   /* Create candidate list servicing task */
   if ( BOS_STATUS_OK != bosTaskCreate( "AGTS",
                                        CDTS_TASK_STACK,
                                        CDTS_TASK_PRIORITY,
                                        (BOS_TASK_ENTRY)agntSrvThread,
                                        (BOS_TASK_ARG *)NULL,
                                        &cndtLstSrvTaskId ))
   {
      LOG(( "iceagnt_Init: failed to create candidate list servicing thread" ));
      return( BOS_FALSE );
   }
   
   return( BOS_TRUE );
}

/*****************************************************************************
*
*  FUNCTION:   iceagnt_Deinit
*
*  PURPOSE:    Deinitialize the ICE Agent module.
*
*  PARAMETERS:
*              none
*
*  RETURNS:    BOS_TRUE if successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL iceagnt_Deinit( void )
{
   BOS_STATUS status;

   /* delete all agents lists */
   AGNT_MUTEX_ACQ;
   while ( NULL != pAgntLstHead )
   {
      (void)iceagnt_Delete( pAgntLstHead->agentId );
   }
   AGNT_MUTEX_REL;

   /* Delete candidate list servicing thread */
   status = bosTaskDestroy( &cndtLstSrvTaskId );
   if ( status != BOS_STATUS_OK )
   {
      DLOG(( "iceagnt_Deinit: Failed to destroy AGTS thread" ));
      return ( BOS_FALSE );
   }

   /* Destroy Agent list mutex */
   if ( BOS_STATUS_OK != bosMutexDestroy( &agntMtx ) )
   {
      DLOG(( "iceagnt_Deinit: Failed to destroy Mutex AGNT" ));
      return ( BOS_FALSE );
   }
   LOG(( "iceagnt_Deinit: Done" ));

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   iceagnt_Create
*
*  PURPOSE:    Create a new agent.
*
*  PARAMETERS:
*              pServerList    [IN]  - STUN Server List
*              pOptions      [IN] - Agent options
*              pAgentId      [MOD]- Agent Id
*
*  RETURNS:    BOS_TRUE if successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL iceagnt_Create( const ICE_STUN_SERVER_LIST   *pServerList,
                         const ICE_AGENT_OPTIONS      *pOptions,
                         BOS_UINT32                   *pAgentId )
{
   int i;
   ICEAGNT *pAgnt;
   BOS_BOOL rc = BOS_TRUE;

   LOG(("iceagnt_Create..."));
   DLOG(("  mode [%s]", icedbgTxtMapById( iceModeMap, pOptions->mode) ));
   DLOG(("  nom [%s]", icedbgTxtMapById( iceNomMap, pOptions->nom) ));
   BOS_ASSERT( NULL != pServerList );
   BOS_ASSERT( NULL != pOptions );
   AGNT_MUTEX_ACQ;

   if ( agntCount > ICE_AGENT_NUM_MAX )
   {
      rc = BOS_FALSE;
   }

   if ( rc && (NULL != (pAgnt = (ICEAGNT *)malloc( sizeof(ICEAGNT)))))
   {
      memset( pAgnt, 0, sizeof(ICEAGNT));

      pAgnt->agentId = newAgntId();       /* Agent Identifier */
      memcpy( &pAgnt->serverList, pServerList, sizeof(ICE_STUN_SERVER_LIST) );
      pAgnt->mode = pOptions->mode;       /* Agent mode */
      pAgnt->rmtMode = ICE_UNKNOWN;       /* To be determinated later */
      pAgnt->nom = pOptions->nom;         /* Nomation setting */
      pAgnt->role = ICE_UNKNOWN;          /* To be determined later */
      pAgnt->state = ICEAGNT_OFFLINE;

      if ( NULL != pOptions->pPrefOverride )
      {
         pAgnt->priTypePref = pOptions->pPrefOverride->type;
         pAgnt->priLocalPref = pOptions->pPrefOverride->local;
      }
      else
      {
         pAgnt->priTypePref.host = ICE_TYPEPREF_HOST_DEFAULT;
         pAgnt->priTypePref.srflx = ICE_TYPEPREF_SERVER_REFLEXIVE_DEFAULT;
         pAgnt->priTypePref.prflx = ICE_TYPEPREF_PEER_REFLEXIVE_DEFAULT;
         pAgnt->priTypePref.relayed = ICE_TYPEPREF_RELAYED_DEFAULT;
         pAgnt->priLocalPref = ICE_LOCALPREF_DEFAULT;
      }

      for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
      {
         pAgnt->pLclCndtLst[i] = NULL;
         pAgnt->pRmtCndtLst[i] = NULL;
      }

      pAgnt->maxWait = 0;
      pAgnt->eTime = 0;

      pAgnt->pPrev = NULL;        /* Pointer to previous agent */
      pAgnt->pNext = pAgntLstHead;        /* Pointer to next agent */
      if ( NULL != pAgntLstHead )
      {
         pAgntLstHead->pPrev = pAgnt;
      }
      pAgntLstHead = pAgnt;

      *pAgentId = pAgnt->agentId;

      agntCount++;
      LOG((" Agent [%d] created", (int)*pAgentId));
   }
   else
   {
      rc = BOS_FALSE;
      *pAgentId = ICE_UNKNOWN;
      LOG(("  Failed to create Agent"));
   }

   AGNT_MUTEX_REL;

   return( rc );
}


/*****************************************************************************
*
*  FUNCTION:   iceagnt_Delete
*
*  PURPOSE:    Delete a previously created agent.
*
*  PARAMETERS:
*              agentId      [IN]- Agent Id
*
*  RETURNS:    BOS_TRUE if successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL iceagnt_Delete( const BOS_UINT32 agentId )
{
   int i;
   BOS_BOOL rc = BOS_FALSE;
   ICEAGNT *pAgnt;

   LOG(("iceagnt_Delete... agentid [%d]", (int)agentId));
   AGNT_MUTEX_ACQ;

   if ( NULL != (pAgnt = iceagnt_GetAgentFromId( agentId )) )
   {
      for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
      {
         /* Delete associated candidate lists */
         if ( NULL != pAgnt->pLclCndtLst[i] )
         {
            DLOG(("  deleting local candidate lists..."));
            icecndt_DeleteList( pAgnt->pLclCndtLst[i] );
         }
         if ( NULL != pAgnt->pRmtCndtLst[i] )
         {
            DLOG(("  deleting remote candidate lists..."));
            icecndt_DeleteList( pAgnt->pRmtCndtLst[i] );
         }
      }

      /* Delete Check List */ 
      if ( NULL != &pAgnt->chkLst )
      {
         DLOG(("  deleting Check List..."));
         icectty_DeleteChkLst( &pAgnt->chkLst );
      }

      /* Delete Agent */
      if ( NULL != pAgnt->pPrev )
      {
         pAgnt->pPrev->pNext = pAgnt->pNext;
      }
      else
      {
         pAgntLstHead = pAgnt->pNext;
      }
      if ( NULL != pAgnt->pNext )
      {
         pAgnt->pNext->pPrev = pAgnt->pPrev;
      }

      rc = BOS_TRUE;
      agntCount--;
      LOG((" .. Agent deleted"));
   }

   AGNT_MUTEX_REL;

   return( rc );
}

/*****************************************************************************
*
*  FUNCTION:   newAgentId
*
*  PURPOSE:    Generate a new Agent Id
*
*  PARAMETERS:
*              none.
*
*  RETURNS:    agent id
*
*  NOTES:
*
*****************************************************************************/
static BOS_UINT32 newAgntId( void )
{
   static BOS_UINT16 agentId = 0;

   return( (BOS_UINT32)agentId++ );
}

/*****************************************************************************
*
*  FUNCTION:   iceagnt_SetPortParms
*
*  PURPOSE:    
*
*  PARAMETERS:
*              agentId     [IN] - agent ID
*              pPortParms  [IN] - port parameters
*
*  RETURNS:    BOS_TRUE if successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL iceagnt_SetPortParms( const BOS_UINT32          agentId,
                               const ICE_PORT_PARMS      *pPortParms[] )
{
   BOS_BOOL rc = BOS_FALSE; 
   ICEAGNT *pAgnt;
   ICECNDT_LST *pCndtLst;
   ICE_LOCAL_IF *pLocalIf;
   int i = 0;
   
   LOG(("iceagnt_SetPortParms: agentId [%d]", (int)agentId));
   AGNT_MUTEX_ACQ;
   if ( NULL != (pAgnt = iceagnt_GetAgentFromId( agentId )))
   {
      /* Find empty candidate list slot, 
       * or reuse slot of same component id 
       */
      rc = (ICEAGNT_OFFLINE == pAgnt->state)?BOS_TRUE:BOS_FALSE;

      for ( i = 0; rc && i < ICE_COMP_NUM_MAX; i++ )
      {
         if ( NULL == pPortParms[i] )
         {
            break;
         }

         if ( NULL != pAgnt->pLclCndtLst[i] )
         {
            icecndt_DeleteList( pAgnt->pLclCndtLst[i] );
         }
         pCndtLst = pAgnt->pLclCndtLst[i] = icecndt_NewList();
         BOS_ASSERT( NULL != pCndtLst );

         pLocalIf = ice_GetLocalIfFromId( pPortParms[i]->localIfId );
         BOS_ASSERT( NULL != pLocalIf );

         pCndtLst->priTypePref = pAgnt->priTypePref;
         pCndtLst->priLocalPref = pAgnt->priLocalPref;
         pCndtLst->pLocalIf = pLocalIf;
         pCndtLst->port = pPortParms[i]->port;
         pCndtLst->compId = pPortParms[i]->component;
         pCndtLst->transType = pPortParms[i]->transType;

         pCndtLst->sid = pPortParms[i]->sid;

         BOS_ASSERT( pPortParms[i]->port != 0 );

         if ( ICE_FULL == pAgnt->mode )
         {
            pCndtLst->pStunServer = getStunServerFromId( &pAgnt->serverList, pPortParms[i]->stunServerId );

            BOS_ASSERT( NULL != pCndtLst->pStunServer );

            /* Gather Server reflexive and relayed candidates */
            if ( (NULL != pPortParms[i]->pSecret) &&
                 (pPortParms[i]->pSecret->username.size > 0) && 
                 (pPortParms[i]->pSecret->password.size > 0) && 
                 (pPortParms[i]->pSecret->username.size <= ICEAGNT_USERNAME_LEN_MAX) &&
                 (pPortParms[i]->pSecret->password.size <= ICEAGNT_PASSWORD_LEN_MAX) )
            {
               memcpy(&pCndtLst->secret, pPortParms[i]->pSecret, sizeof(STUNC_SHARED_SECRET));
            }
            else
            {
               pCndtLst->secret.username.size = 0;
               pCndtLst->secret.password.size = 0;
            }
            pCndtLst->reqHdl = 0;
         }
      }
   }

   AGNT_MUTEX_REL;

   return( rc );
}

/*****************************************************************************
*
*  FUNCTION:   iceagnt_GatherCndts
*
*  PURPOSE:    
*
*  PARAMETERS:
*              agentId     [IN] - agent ID
*              maxWait     [IN] - time budget for candidate gathering
*
*  RETURNS:    BOS_TRUE if successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL iceagnt_GatherCndts( const BOS_UINT32            agentId,
                              const BOS_UINT16            maxWait )
{
   BOS_BOOL rc = BOS_FALSE; 
   ICEAGNT *pAgnt;
   ICECNDT_LST *pCndtLst;
   int i = 0;
   
   LOG(("iceagnt_GatherCndts: agentId [%d]", (int)agentId));
   AGNT_MUTEX_ACQ;
   if ( NULL != (pAgnt = iceagnt_GetAgentFromId( agentId )))
   {
      /* Find empty candidate list slot, 
       * or reuse slot of same component id 
       */
      rc = (ICEAGNT_OFFLINE == pAgnt->state)?BOS_TRUE:BOS_FALSE;

      for ( i = 0; rc && i < ICE_COMP_NUM_MAX; i++ )
      {
         if ( NULL == (pCndtLst = pAgnt->pLclCndtLst[i]) )
         {
            continue;
         }

         /* Set time budget for candidate gathering from server */
         pCndtLst->maxWait = maxWait;

         /* Gather Host candidates */
         rc = icecndt_GatherHostCndt( pCndtLst,
                                      pCndtLst->pLocalIf,
                                      pCndtLst->port,
                                      pCndtLst->compId );
         
         BOS_ASSERT( BOS_TRUE == rc );
         DLOG(("  Host Cndts gathered"));

         /* Candidate list ready for servicing */
         pCndtLst->state = CNDTLST_PENDING;
         DLOG(("  putting agentId [%d] Cndtlist to state [%s]", (int)agentId,icedbgTxtMapById( icecndtLstStateMap, pCndtLst->state )));
      }

      if ( BOS_TRUE == rc )
      {
         pAgnt->state = ICEAGNT_CNDT_GATHERING;
         if ( ICE_FULL == pAgnt->mode )
         {
            pAgnt->maxWait = (BOS_TIME_MS)maxWait;
         }
      }
      else
      {
         pAgnt->state = ICEAGNT_OFFLINE;
      }

      DLOG(("  putting agentId [%d] to state [%s]", (int)agentId, icedbgTxtMapById( iceagntStateMap, pAgnt->state )));
   }

   AGNT_MUTEX_REL;

   return( rc );
}


/*****************************************************************************
*
*  FUNCTION:   agntSrvThread
*
*  PURPOSE:    Agent list servicing thread.
*
*              For requests sent for the same candidate list, the 
*              request is paced at Ta intervals.
*
*              If time budget of a candidate list for getting candidate 
*              has been reached, the candidate list will be removed 
*              from the servicing queue, and a status event will 
*              be reported.
*
*  PARAMETERS:
*              none
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static void agntSrvThread( void )
{
   int i;
   ICEAGNT *pTmpAgnt;
   ICECNDT_LST *pCndtLst;
   BOS_BOOL bSendReqOK = BOS_TRUE;  /* OK to send STUN request */
   BOS_UINT32 serviceT = 0;
   int rc = 0;
   ICEAGNT_STATE nextState;
#if ICEDBG_LOG
   static int tmp = 0;
#endif

   for ( ;; )
   {
      (void)bosSleep( CDTS_SLEEP );

      if ( (serviceT += CDTS_SLEEP)>= cndtLstTa )
      {
         /* Paced sending of STUN request every Ta seconds */
         bSendReqOK = BOS_TRUE;
         serviceT = 0;
      }

#if ICEDBG_LOG
      tmp++;
#endif
      AGNT_MUTEX_ACQ;
      for ( pTmpAgnt = pAgntLstHead; NULL != pTmpAgnt; pTmpAgnt = pTmpAgnt->pNext )
      {
#if ICEDBG_LOG
         if ( tmp == 0xff )
         {
            DLOG(("agntSrvThread: agent [%d] current state [%s]",
                     (int)pTmpAgnt->agentId, icedbgTxtMapById( iceagntStateMap, pTmpAgnt->state )));
         }
#endif
         nextState = pTmpAgnt->state;
         switch( pTmpAgnt->state )
         {
            case ICEAGNT_OFFLINE:
            {
               /* Do nothing */
            }
            break;
            case ICEAGNT_CNDT_GATHERING:
            {
               if ( ICE_FULL == pTmpAgnt->mode )
               {
                  if ( ((pTmpAgnt->eTime += CDTS_SLEEP) > pTmpAgnt->maxWait) ||
                       ( BOS_FALSE == isWaitingServerResponse( pTmpAgnt, &rc ) ))
                  {
                     ICE_EVT evt;
                     DLOG(("agntSrvThread: agent [%d] current state [%s]",
                              (int)pTmpAgnt->agentId, icedbgTxtMapById( iceagntStateMap, pTmpAgnt->state )));
                     if ( pTmpAgnt->eTime > pTmpAgnt->maxWait )
                     {
                        evt = ICE_EVT_CANDIDATES_GATHER_TIMEOUT;
                     }
                     else 
                     {
                        if ( rc == SERVER_RES_TIMEOUT )
                        {
                           evt = ICE_EVT_CANDIDATES_GATHER_TIMEOUT;
                        }
                        else
                        {
                           evt = ICE_EVT_CANDIDATES_GATHER_COMPLETE;
                        }
                     }

                     for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
                     {
                        if (NULL != (pCndtLst = pTmpAgnt->pLclCndtLst[i]))
                        {
                           icecndt_FinalizeCndtLst( pCndtLst );
                        }
                     }

                     (*iceEvtCB)( evt, NULL, pTmpAgnt->agentId );
                     nextState = ICEAGNT_CTTY_CHECK_READY;
                  }
                  else
                  {
                     /* Continue to gather candidates */
                     for ( i = 0; (BOS_TRUE == bSendReqOK) && (i < ICE_COMP_NUM_MAX); i++ )
                     {
                        if ( (NULL != (pCndtLst = pTmpAgnt->pLclCndtLst[i]))
                          && (CNDTLST_PENDING == pCndtLst->state))
                        {
                           icecndt_SendRequest( pCndtLst );
                        } 
                     } 
                  }
               }
               else
               {
                  nextState = ICEAGNT_CTTY_CHECK_READY;
                  (*iceEvtCB)( ICE_EVT_CANDIDATES_GATHER_COMPLETE, 
                               NULL, 
                               pTmpAgnt->agentId );
               }
            }
            break;
            case ICEAGNT_CTTY_CHECK_READY:
            {
               /* Start connectivity check if it's a FULL mode
                * agent and when SDP parameters are valid
                */
               if ( (ICE_FULL == pTmpAgnt->mode) && 
                    pTmpAgnt->bLclSdpValid && 
                    pTmpAgnt->bRmtSdpValid )
               {
                  rc = icectty_FormCheckList( (pTmpAgnt->pLclCndtLst),
                                              (pTmpAgnt->pRmtCndtLst),
                                              &pTmpAgnt->chkLst,
                                              pTmpAgnt->role );
                  if ( BOS_TRUE == rc )
                  {
                     nextState = ICEAGNT_CTTY_CHECK_IN_PROGRESS;
                  }
               }
               else if ( (ICE_LITE == pTmpAgnt->mode) &&
                         isAllCompsNominated( pTmpAgnt) )
               {
                  ICE_MEDIA_TRANSPORT mediaTransport;
                  fillMediaTransport( pTmpAgnt, &mediaTransport );
                  /* Notify client Media transport */
                  DLOG((" --- ICE LITE CONCLUDED: Notify client media transport"));
                  (*iceEvtCB)( ICE_EVT_CONCLUDED_SUCCESS, 
                               &mediaTransport, 
                               pTmpAgnt->agentId );
                  nextState = ICEAGNT_CONCLUDED;
               }
               else
               {
                  /*
                   * Agent is ready for Receiving connectivity checks
                   */
               }
            }
            break;
            case ICEAGNT_CTTY_CHECK_IN_PROGRESS:
            {
               /* Must use Regular nomination if remote is ice-lite or if 
                * it's a Controlled role (Controlled role cannot nominate) 
                */
               ICE_NOM nom = ( ICECTTY_CONTROLLED == pTmpAgnt->role )?ICE_REGULAR:pTmpAgnt->nom;

               /* Only Full mode agent can reach this state */
               BOS_ASSERT( ICE_FULL == pTmpAgnt->mode ); 

               /* Start connectivity checks */
               if ( BOS_TRUE != icectty_PeriodicCheck( 
                                    &pTmpAgnt->chkLst,
                                    &pTmpAgnt->lclCredential,
                                    &pTmpAgnt->rmtCredential,
                                    &pTmpAgnt->priTypePref,
                                    &pTmpAgnt->priLocalPref,
                                    nom, 
                                    pTmpAgnt->role ))
               {
                  LOG(( "agntSrvThread: Periodic check error, agent id[%d]",
                           (int)pTmpAgnt->agentId ));
               }

               switch( pTmpAgnt->chkLst.state )
               {
                  case ICECTTY_CL_RUNNING:
                  {
                     if (ICECTTY_CONTROLLING == pTmpAgnt->role)
                     {
                        /* For Controlling Regular nominating agent, 
                         * when all checks have be sent, if no pair has 
                         * been nominated, nominate the highest priority 
                         * check pair and send check including the 
                         * Use-Candidate attribute.  
                         */
                        if ( ICE_REGULAR == pTmpAgnt->nom )
                        {
                           BOS_BOOL bFailed = BOS_FALSE;
                           if ( isAllChksAckd( pTmpAgnt, &bFailed ) )
                           {
                              if ( !bFailed )
                              {
                                 /* Nominate pair by sending check including the
                                  * USE-CANDIDATE attribute.
                                  */
                                 nominateValidatedChk( pTmpAgnt );
                              }
                           }
                        }
                     }

                     if ( isAllCompsNominated( pTmpAgnt ) )
                     {
                        pTmpAgnt->chkLst.state = ICECTTY_CL_COMPLETED;
                        pTmpAgnt->chkLst.bNewSelected = selectChkPair( pTmpAgnt );
                     }
                  }
                  break;
                  case ICECTTY_CL_COMPLETED:
                  {
                     ICE_MEDIA_TRANSPORT mediaTransport;
                     fillMediaTransport( pTmpAgnt, &mediaTransport );
                     /* Notify client selected pair */
                     DLOG((" --- ICE CONCLUDED: Notify client of selected pair"));
                     (*iceEvtCB)( ICE_EVT_CONCLUDED_SUCCESS, 
                                  &mediaTransport, 
                                  pTmpAgnt->agentId );
                     pTmpAgnt->chkLst.bNewSelected = BOS_FALSE;
                     nextState = ICEAGNT_CONCLUDED;
                  }
                  break;
                  case ICECTTY_CL_FAILED:
                  {
                     /* Notify client of Failure */
                     DLOG((" --- ICE CONCLUDED: Notify client of failure "));
                     (*iceEvtCB)( ICE_EVT_CONCLUDED_FAILURE, 
                                  NULL, 
                                  pTmpAgnt->agentId );
                     nextState = ICEAGNT_CONCLUDED;
                  }
                  break;
                  default:
                  {
                     /* Check list in INACTIVE state, no action */
                  }
               }
            }
            break;
            case ICEAGNT_CONCLUDED:
            {
               /* Do nothing, awaits restarting of ICE */
               /* Notify client of newly selected check pair,
                * this should only happen in Aggressive Nominating agent 
                */
               if ( pTmpAgnt->chkLst.bNewSelected )
               {
                  ICE_MEDIA_TRANSPORT mediaTransport;

                  DLOG(("agntSrvThread: state[%s], nom[%s] new selected check pair",
                     icedbgTxtMapById( iceagntStateMap, pTmpAgnt->state ),
                     icedbgTxtMapById( iceNomMap, pTmpAgnt->nom )));

                  fillMediaTransport( pTmpAgnt, &mediaTransport );
                  /* Notify client newly selected pair */
                  DLOG((" --- ICE already CONCLUDED: Update client of newly selected pair"));
                  (*iceEvtCB)( ICE_EVT_CONCLUDED_SUCCESS, 
                               &mediaTransport, 
                               pTmpAgnt->agentId );
                  pTmpAgnt->chkLst.bNewSelected = BOS_FALSE;
               }
            }
            break;
            default:
            {
               BOS_ASSERT( BOS_FALSE );
            }
         } /* switch */

         pTmpAgnt->state = nextState;
#if ICEDBG_LOG
         if ( tmp == 0xff )
         {
            DLOG(("agntSrvThread: agent [%d] next state [%s]",
                     (int)pTmpAgnt->agentId, icedbgTxtMapById( iceagntStateMap, pTmpAgnt->state )));
         }
#endif
      } /* for */
      AGNT_MUTEX_REL;
#if ICEDBG_LOG
      if ( tmp == 0xff )
      {
         tmp = 0;
      }
#endif
   }
}

/*****************************************************************************
*  FUNCTION:   iceagnt_Notify
*
*  PURPOSE:    Routine to notify the ICE Agent state machine about 
*              Connectivity Check status and Requests.
*
*  PARAMETERS:
*              evt      [IN] - Ice agent event
*              op1      [IN] - event dependent data
*              op2      [IN] - event dependent data
*              pData    [IN] - event dependent data
*
*  RETURNS:    BOS_TRUE if OK. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL iceagnt_Notify( const ICEAGNT_EVT evt, const BOS_UINT32 op1, const BOS_UINT32 op2, const void *pData )
{
   BOS_BOOL rc = BOS_TRUE;

   switch( evt )
   {
      case ICEAGNT_CTTYCHECK_STATUS:
      {
         BOS_UINT32 reqHdl = op1;
         BOS_UINT32 statusCode = op2;
         STUN_TRANSPORT *pMappedAddr = (STUN_TRANSPORT *)pData;
         processCheckStatus( reqHdl, pMappedAddr, statusCode );
      }
      break;
      case ICEAGNT_CTTYCHECK_REQUEST:
      {
         ICEAGNT *pTmpAgnt;
         BOS_BOOL bUseCndt;
         BOS_BOOL bHasRmtPriority = BOS_FALSE;
         BOS_UINT32 rmtPriority;
         BOS_ASSERT( NULL != pData );

         /* Respond to Peer Connectivity Check */
         ackPeerBindingRequest( (STUN_REQ_DATA *)pData );

         bUseCndt = stunGetAttribute( ((STUN_REQ_DATA *)pData)->pMsg, STUN_ATTR_USECANDIDATE, NULL );
         bHasRmtPriority = stunGetAttribute( ((STUN_REQ_DATA *)pData)->pMsg, STUN_ATTR_PRIORITY, &rmtPriority );

         AGNT_MUTEX_ACQ;
         for ( pTmpAgnt = pAgntLstHead; NULL != pTmpAgnt; pTmpAgnt = pTmpAgnt->pNext )
         {
            /* For FULL mode agent, begin Triggered check */
            if ( (ICE_FULL == pTmpAgnt->mode) && 
                 (pTmpAgnt->state >= ICEAGNT_CTTY_CHECK_READY) &&
                 (pTmpAgnt->chkLst.state <ICECTTY_CL_COMPLETED ) )
            {
               ICE_NOM nom = ( ICECTTY_CONTROLLED == pTmpAgnt->role )?ICE_REGULAR:pTmpAgnt->nom;
               /* Get Remote Priority */
               if ( bHasRmtPriority )
               {
                  if ( BOS_TRUE != icectty_TriggeredCheck( 
                                       ((STUN_REQ_DATA *)pData)->pLclAddr,
                                       ((STUN_REQ_DATA *)pData)->pSrcAddr,
                                       ((STUN_REQ_DATA *)pData)->sid,
                                       rmtPriority,
                                       &pTmpAgnt->chkLst,
                                       &pTmpAgnt->lclCredential,
                                       &pTmpAgnt->rmtCredential,
                                       &pTmpAgnt->priTypePref,
                                       &pTmpAgnt->priLocalPref,
                                       nom,
                                       pTmpAgnt->role,
                                       bUseCndt,
                                       pTmpAgnt->pLclCndtLst,
                                       pTmpAgnt->pRmtCndtLst))
                  {
                     DLOG(("iceagnt_Notify: Trigger Check no match"));
                  }
                  else
                  {
                     LOG(("iceagnt_Notify: triggered Checks found matching pair or inserted new pair."));
                     break;
                  }
               }
               else
               {
                  LOG(("iceagnt_Notify: Priority Attribute missing, triggered Checks Aborted"));
               }
            }
            /* For Lite mode agent, if the Binding Request contains the
             * USE-CANDIDATE attribute, update its Check List 
             */
            else if ( (ICE_LITE == pTmpAgnt->mode) && bUseCndt )
            {
               if ( BOS_TRUE != icectty_LiteUpdateCheckList( 
                                             ((STUN_REQ_DATA *)pData)->pLclAddr,
                                             ((STUN_REQ_DATA *)pData)->pSrcAddr,
                                             ((STUN_REQ_DATA *)pData)->sid, 
                                             pTmpAgnt->pLclCndtLst,
                                             pTmpAgnt->pRmtCndtLst,
                                             &pTmpAgnt->chkLst ) )
               {
                  DLOG(("iceagnt_Notify: Lite no match"));
               }
               else
               {
                  LOG(("iceagnt_Notify: Lite found matching component"));
                  break;
               }
            }
         }
         AGNT_MUTEX_REL;
      }
      break;
      default:
      {
         BOS_ASSERT( BOS_FALSE );
      }
   }

   return( rc );
}

/*****************************************************************************
*  FUNCTION:   processCheckStatus
*
*  PURPOSE:    Process Connectivity Check responses
*
*  PARAMETERS:
*              reqHdl      [IN] - Request handle
*              pMappedAddr [IN] - Mapped Address included in peer Binding
*                                 response
*              statusCode  [IN] - status code
*
*  RETURNS:    none
*
*  NOTES:      Agent list Mutex is assumed to have been taken.
*
*****************************************************************************/
static void processCheckStatus( const BOS_UINT32 reqHdl, const STUN_TRANSPORT *pMappedAddr, BOS_UINT32 statusCode )
{
   ICEAGNT *pTmpAgnt;
   ICECTTY_CHECK *pChk;
   int j;
   ICECNDT *pCndt;
   BOS_BOOL rc;
   BOS_BOOL bFound = BOS_FALSE;
   BOS_BOOL bFailed;

   AGNT_MUTEX_ACQ;
   /* Find agent */
   for ( pTmpAgnt = pAgntLstHead; (NULL != pTmpAgnt) && !bFound; pTmpAgnt = pTmpAgnt->pNext )
   {
      if ( (ICEAGNT_CTTY_CHECK_IN_PROGRESS > pTmpAgnt->state) ||
           (ICECTTY_CL_RUNNING > pTmpAgnt->chkLst.state ) )
      {
         DLOG(("  Agent: NO Match state [%s], chklist state [%s]", icedbgTxtMapById( iceagntStateMap, pTmpAgnt->state ), icedbgTxtMapById( icecttyChkLstStateMap, pTmpAgnt->chkLst.state) ));
         continue;
      }

      LOG(("processCheckStatus: agentId[%d] reqHdl[%d] statusCode[%d]", (int)pTmpAgnt->agentId, (int)reqHdl, (int)statusCode ));
      for ( pChk = pTmpAgnt->chkLst.pHead; 
            !bFound && (NULL != pChk); 
            pChk = pChk->pNext )
      {
         if ( pChk->state < ICECTTY_IN_PROGRESS )
         {
            DLOG(("  Check State: NO Match" ));
            continue;
         }

         if ( pChk->reqHdl != reqHdl )
         {
            DLOG(("  handle: NO Match" ));
            continue;
         }

         LOG(("   reqHdl[%d] matched",(int)reqHdl));
         bFound = BOS_TRUE;

         switch( statusCode )
         {
            case ICEAGNT_CHECK_SUCCESS:
            {
               pChk->state = ICECTTY_SUCCEEDED;
               if ( !pChk->bValidated )
               {
                  pChk->bValidated = BOS_TRUE;

                  /* Changes the states for all other Frozen pairs for 
                   * the same media stream and same foundation to Waiting.
                   */
                  icectty_UnfreezeCndtPairs( &pTmpAgnt->chkLst, 
                                             pChk->pLclCndt->foundation,
                                             pChk->pRmtCndt->foundation );

                  /* Discover new candidate from response */
                  if ( NULL != pMappedAddr )
                  {
                     BOS_BOOL bCndtMatched = BOS_FALSE;
                     ICECNDT_LST *pCndtLst = NULL;
                     /* If the transport address does not match 
                      * any of the local candidates that the
                      * agent knows about, the mapped address 
                      * represents a new candidate - a peer reflexive 
                      * candidate for that component.
                      */
                     for( j = 0; 
                           (NULL != pTmpAgnt->pLclCndtLst[j]) && 
                           !bCndtMatched && j<ICE_COMP_NUM_MAX; 
                          j++ )
                     {
                        if ( pTmpAgnt->pLclCndtLst[j]->compId != pChk->compId )
                        {
                           continue;
                        }

                        pCndtLst = pTmpAgnt->pLclCndtLst[j];
                        for ( pCndt = pCndtLst->pCndtHead;
                              NULL != pCndt && !bCndtMatched;
                              pCndt = pCndt->pNext )
                        {
                           if ( pCndt->transport.bValid )
                           {
                              if ( !memcmp(pMappedAddr, 
                                          &pCndt->transport.socAddr,
                                          sizeof(STUN_TRANSPORT) ))
                              {
                                 /* Candidate already exists */
                                 bCndtMatched = BOS_TRUE;
                                 break;
                              }/* if */
                           }/* if */
                        }/* for */
                     }/* for */

                     if ( !bCndtMatched )
                     {
                        /* Add to local candidate list */
                        pCndt = icecndt_NewCndt();
                        BOS_ASSERT( NULL != pCndt );
                        
                        /* pCndtLst should be valid as pchk->compId should have matched
                         * to one of the cndtlist 
                         */
                        BOS_ASSERT( NULL != pCndtLst );

                        pCndt->cndtId = pCndtLst->cndtCount;
                        pCndt->state = ICECNDT_FROZEN;
                        pCndt->type = ICECNDT_PEERRFLX;
                        pCndt->pBaseCndt = pChk->pLclCndt; 

                        pCndt->priority = icecndt_ComputePriority( 
                                          pCndtLst->priTypePref.prflx, 
                                          pCndtLst->priLocalPref, 
                                          pCndtLst->compId );

                        pCndt->bInUse = BOS_FALSE;
                        pCndt->transport.bValid = BOS_TRUE;
                        pCndt->transport.socAddr = *pMappedAddr;
                        pCndt->pRelated = NULL;
                        sprintf( pCndt->foundation,
                                 "%d",
                                 (pCndtLst->pLocalIf->id<<4) |
                                 0 <<2 |
                                 ICECNDT_FOUNDATION_PRFLX );
                             
                        DLOG(("processCheckStatus: Adding new candidate compId [%s] learned from check response", TMAP(iceCompIdMap, pChk->compId )));

                        rc = icecndt_AddCndtToList( pCndt, pCndtLst);
                        BOS_ASSERT( BOS_TRUE == rc );

                        break;
                     } /* if */
                  } /* if */
                  else
                  {
                     LOG(("processCheckStatus: Mapped Address attribute missing"));
                  }

                  /* For Aggressive Nominating agent, the check pair can be
                   * promoted to nominated 
                   */
                  if ( ICE_AGGRESSIVE == pTmpAgnt->nom )
                  {
                     LOG(("   --- Aggressive nominating agent, promote pair to Nominated."));
                     pChk->bNominated = BOS_TRUE;
                  }
               }

               if ( pChk->bUseCndtChk )
               {
                  /* this check status is a result of a check for promoting 
                   * to nominated pair. 
                   */
                  LOG(("processCheckStatus: Check success for nominated pair."));
                  pChk->bNominated = BOS_TRUE;
               }

               /* Update check list state:
                *    If each component has a nominated pair, move check 
                *    list to the Completed state.
                *    If any component fails all validation checks, 
                *    move check list to the Failed state.
                */
               if ( pTmpAgnt->chkLst.state >= ICECTTY_CL_RUNNING )
               {
                  if ( isAllCompsNominated( pTmpAgnt ) )
                  {
                     LOG(("  ---Check list completes with nominated pairs found"));
                     pTmpAgnt->chkLst.state = ICECTTY_CL_COMPLETED;
                     /* Select check pair, set new selected flag if 
                      * a higher priority pair has been selected 
                      */
                     pTmpAgnt->chkLst.bNewSelected = selectChkPair( pTmpAgnt );
                  }
               }
            }
            break;
            case ICEAGNT_CHECK_FAILURE:
            {
               pChk->state = ICECTTY_FAILED;
            }
            break;
            default:
            {
               BOS_ASSERT( BOS_FALSE );
            }
         }/* switch */

         /* If all Checks has been acknowledged, 
          * update Check List state for this agent */
         if ( isAllChksAckd( pTmpAgnt, &bFailed ) )
         {
            if ( bFailed )
            {
               LOG(("  ---Check list ends with some failure"));
               pTmpAgnt->chkLst.state = ICECTTY_CL_FAILED;
            }
         }
      }/* for */
   } /* for */

   AGNT_MUTEX_REL;
}

/*****************************************************************************
*
*  FUNCTION:   isAllCompsNominated
*
*  PURPOSE:    Check checklist and return BOS_TRUE if each comp has been 
*              nominated.
*
*  PARAMETERS:
*              pAgnt      [IN] - Agent
*
*  RETURNS:    BOS_TRUE if found, BOS_FALSE otherwise.
*
*  NOTES:      Agent list Mutex is assumed to have been taken.
*
*****************************************************************************/
static BOS_BOOL isAllCompsNominated( const ICEAGNT *pAgnt )
{
   ICE_COMP_ID compId[]={ICE_COMP_RTP, ICE_COMP_RTCP};
   int i;
   BOS_BOOL found = BOS_FALSE;
   BOS_BOOL compIdExists = BOS_FALSE;
   ICECTTY_CHECK *pChk;
   
   BOS_ASSERT( NULL != pAgnt );

   /* Get first current CompId from the check list */
   if ( NULL == pAgnt->chkLst.pHead )
   {
      return( BOS_FALSE );
   }

   /* Traverse check list to check for any un-nominated components */
   for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
   {
      found = BOS_FALSE;
      compIdExists = BOS_FALSE;
      for ( pChk = pAgnt->chkLst.pHead; NULL != pChk; pChk = pChk->pNext )
      {
         if ( compId[i] == pChk->compId )
         {
            compIdExists = BOS_TRUE;
            if ( pChk->bNominated )
            {
               found = BOS_TRUE;
               break;
            }
         }
      }
      if ( compIdExists && !found )
      {
         return( BOS_FALSE );
      }
   }

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   isAllChksAckd
*
*  PURPOSE:    Check checklist and return BOS_TRUE if all connectivity
*              checks have been responded to or timed out.
*
*  PARAMETERS:
*              pAgnt    [IN]   - Agent
*              pbFailed [OUT]  - Check list ends with failure
*
*  RETURNS:    BOS_TRUE if found, BOS_FALSE otherwise.
*
*  NOTES:      Agent list Mutex is assumed to have been taken.
*
*****************************************************************************/
static BOS_BOOL isAllChksAckd( const ICEAGNT *pAgnt, BOS_BOOL *pbFailed )
{
   BOS_BOOL bAllAckd = BOS_TRUE;
   ICE_COMP_ID compId[ICE_COMP_NUM_MAX]={ICE_COMP_RTP, ICE_COMP_RTCP};
   BOS_BOOL compIdExists = BOS_FALSE;
   int i;
   ICECTTY_CHECK *pChk;
   
   BOS_ASSERT( NULL != pAgnt );
   BOS_ASSERT( NULL != pbFailed );

   *pbFailed = BOS_FALSE;

   /* Get first current CompId from the check list */
   if ( NULL == pAgnt->chkLst.pHead )
   {
      return( BOS_FALSE );
   }

   for ( i = 0; i < ICE_COMP_NUM_MAX && bAllAckd && !*pbFailed; i++ )
   {
      compIdExists = BOS_FALSE;
      for ( pChk = pAgnt->chkLst.pHead; NULL != pChk; pChk = pChk->pNext )
      {
         if ( pChk->state <= ICECTTY_IN_PROGRESS )
         {
            /* Some checks are still ongoing */
            bAllAckd = BOS_FALSE;
            break;
         }

         if ( compId[i] == pChk->compId )
         {
            if ( !pChk->bValidated )
            {
               *pbFailed |= BOS_TRUE;
               break;
            }
         }
      }
   }

   return( bAllAckd );
}

/*****************************************************************************
*  FUNCTION:   nominateValidatedChk
*
*  PURPOSE:    Nominate a pair from each component from a list of 
*              validated checks.
*
*  PARAMETERS:
*              pAgnt    [MOD]   - Agent
*
*  RETURNS:    BOS_TRUE if found, BOS_FALSE otherwise.
*
*  NOTES:      Agent list Mutex is assumed to have been taken.
*
*****************************************************************************/
static BOS_BOOL nominateValidatedChk( ICEAGNT *pAgnt )
{
   ICE_COMP_ID compId[ICE_COMP_NUM_MAX]={ICE_COMP_RTP, ICE_COMP_RTCP};
   int i;
   ICECTTY_CHECK *pChk;
   BOS_BOOL rc = BOS_FALSE;

   BOS_ASSERT( NULL != pAgnt );
   BOS_ASSERT( ICECTTY_CL_RUNNING == pAgnt->chkLst.state );
   /* Get first current CompId from the check list */
   BOS_ASSERT( NULL != pAgnt->chkLst.pHead );

   for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
   {
      for ( pChk = pAgnt->chkLst.pHead; NULL != pChk; pChk = pChk->pNext )
      {
         if ( compId[i] == pChk->compId )
         {
            if ( !pChk->bUseCndtChk )
            {
               /* This component has not been nominated, send check 
                * including the USE-CANDIDATE attribute 
                */
               rc = icectty_SendConnectivityCheck( &pAgnt->lclCredential,
                                                   &pAgnt->rmtCredential,
                                                   &pAgnt->priTypePref,
                                                   &pAgnt->priLocalPref,
                                                   pChk,
                                                   BOS_TRUE,
                                                   pAgnt->role,
                                                   &pChk->reqHdl );
               if ( BOS_TRUE == rc )
               {
                  pChk->state = ICECTTY_IN_PROGRESS;
                  pChk->bUseCndtChk = BOS_TRUE;
                  DLOG(("nominateValidatedChk: setting check state to [%s]", icedbgTxtMapById( icecttyChkStateMap, pChk->state) ));
               }
            }
            /* next compId */
            break;
         }
      }
   }

   return( rc );
}

/*****************************************************************************
*
*  FUNCTION:   iceagnt_ProcessStuncRes
*
*  PURPOSE:    Process STUNC responses. This function locates the ICE agent
*              by the response handle <reqHdl> and updates candidate list.
*
*  PARAMETERS:
*              reqHdl      [IN] - STUN request handle
*              pEvtData    [IN] - STUN event data
*
*  RETURNS:    BOS_TRUE if found, BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
void iceagnt_ProcessStuncRes( const BOS_UINT32 reqHdl, const STUNC_EVT_DATA *pEvtData )
{
   BOS_BOOL rc = BOS_FALSE;
   ICEAGNT *pTmpAgnt;
   ICECNDT_LST *pCndtLst;
   int i;
   BOS_BOOL bFound = BOS_FALSE;

   AGNT_MUTEX_ACQ;
   /*
    * Locate the agent which sent the Binding request
    */
   for ( pTmpAgnt = pAgntLstHead; 
         !bFound && (NULL != pTmpAgnt); 
         pTmpAgnt = pTmpAgnt->pNext )
   {
      for ( i = 0; !bFound && i < ICE_COMP_NUM_MAX; i++ )
      {
         if (NULL == (pCndtLst = pTmpAgnt->pLclCndtLst[i]))
         {
            continue;
         }

         if ( (CNDTLST_GATHERING == pCndtLst->state) && 
              (NULL != pCndtLst->pStunServer) &&
              (reqHdl == pCndtLst->reqHdl) )
         {
            bFound = BOS_TRUE;

            /* Add server reflexive and relayed candidates to candidate list */
            if ( ICE_SRV_STUN == pCndtLst->pStunServer->type )
            {
               rc = icecndt_GatherStunCndt( pCndtLst, pEvtData );
            }
            else
            {
               BOS_ASSERT( BOS_FALSE );
            }
         }
      }
   }

   AGNT_MUTEX_REL;
}

/*****************************************************************************
*  FUNCTION:   iceagnt_GetLclCndtLsts
*
*  PURPOSE:    Return Local Candidate lists.
*
*  PARAMETERS:
*              agentId      [IN] - Agent Id
*
*  RETURNS:    Local candidate lists, NULL if not found.
*
*  NOTES:
*
*****************************************************************************/
ICEAGNT *iceagnt_GetAgentFromId( const BOS_UINT32 agentId )
{
   ICEAGNT *pTmpAgnt;

   AGNT_MUTEX_ACQ;
   for ( pTmpAgnt = pAgntLstHead; NULL != pTmpAgnt; pTmpAgnt = pTmpAgnt->pNext )
   {
      /* Match Agent ID */
      if ( agentId == pTmpAgnt->agentId )
      {
         break;
      }
   }
   AGNT_MUTEX_REL;

   return( pTmpAgnt );
}

/*****************************************************************************
*  FUNCTION:   setAgentRole
*
*  PURPOSE:    Set the role of the ICE-Agent.
*
*  PARAMETERS:
*              pAgnt       [MOD] - ICE Agent
*              sdpAgent    [IN] - SDP agent type
*
*  RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
*
*  NOTES:
*     (X = dont care)
*     Local Offerer/Answerer rmt-Mode  Role
*     ---------------------------------------
*     Full  Offerer          X         Controlling 
*     Full  X                Lite      Controlling 
*     X     Offerer          Lite      Controlling 
*
*****************************************************************************/
static void setAgentRole( ICEAGNT *pAgnt, const ICE_SDP_AGENT sdpAgent )
{
   /*
    * Controlling Role if any of the following is true:
    * - Local Agent Full mode and local is SDP offerer
    * - Local Agent Full mode and remote agent is Lite mode
    * - Local is offerer and remote Agent is Lite mode 
    */
   if ( ((ICE_FULL == pAgnt->mode) && (ICE_SDP_OFFERER == sdpAgent)) || 
        ((ICE_FULL == pAgnt->mode) && (ICE_LITE == pAgnt->rmtMode )) || 
        ((ICE_SDP_OFFERER == sdpAgent) && (ICE_LITE == pAgnt->rmtMode)) )
   {
      pAgnt->role = ICECTTY_CONTROLLING;
   }
   else
   {
      pAgnt->role = ICECTTY_CONTROLLED;
   }
}

/*****************************************************************************
*  FUNCTION:   isWaitingServerResponse
*
*  PURPOSE:    Check if the agent is still waiting for server responses for
*              the STUN/TURN requests sent for the candidate lists.
*
*  PARAMETERS:
*              pAgnt     [IN] - agent
*              rc        [OUT] - additional return code information
*
*  RETURNS:    
*              BOS_TRUE if it is still waiting.
*              BOS_FALSE otherwise. rc = 1 to indicate timeout condition.
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL isWaitingServerResponse( const ICEAGNT *pAgnt, int *rc )
{
   int i;
   ICECNDT_LST *pCndtLst;
   BOS_ASSERT( NULL != pAgnt );

   *rc = SERVER_RES_UNKNOWN;

   for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
   {
      if (NULL != (pCndtLst = pAgnt->pLclCndtLst[i]))
      {
         if ( (CNDTLST_PENDING == pCndtLst->state)
            ||(CNDTLST_GATHERING == pCndtLst->state) )
         {
            *rc = 0;
            return( BOS_TRUE );
         }

         if ( CNDTLST_TIMEOUT == pCndtLst->state )
         {
            *rc = SERVER_RES_TIMEOUT;
         }
      }
   }
   return( BOS_FALSE );
}

/*****************************************************************************
*  FUNCTION:   iceagnt_EncodeSdp
*
*  PURPOSE:    
*
*  PARAMETERS:
*              agentId     [IN] - agent Id
*              sdpAgent    [IN] - SDP offerer/answerer
*              pSdpParms   [OUT]- SDP data
*
*  RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL iceagnt_EncodeSdp( const BOS_UINT32 agentId, const ICE_SDP_AGENT sdpAgent, ICE_SDP_PARMS *pSdpParms )
{
   ICEAGNT *pAgnt;
   BOS_BOOL rc = BOS_TRUE;

   AGNT_MUTEX_ACQ;

   if ( NULL == (pAgnt = iceagnt_GetAgentFromId( agentId ) ))
   {
      AGNT_MUTEX_REL;
      return( BOS_FALSE );
   }

   if (BOS_TRUE != icesdp_Encode( pAgnt, pSdpParms ))
   {
      AGNT_MUTEX_REL;
      return( BOS_FALSE );
   }

   /* Set ICE-Agent role */
   setAgentRole( pAgnt, sdpAgent );

   /* Set local credential */
   pAgnt->lclCredential = pSdpParms->credential;

   /* Restarting SDP offer/answer*/
   if ( pAgnt->bLclSdpValid && pAgnt->bRmtSdpValid )
   {
      pAgnt->bRmtSdpValid = BOS_FALSE;
      if ( pAgnt->state >= ICEAGNT_CTTY_CHECK_READY )
      {
         pAgnt->state = ICEAGNT_CTTY_CHECK_READY;
      }
   }
   pAgnt->bLclSdpValid = BOS_TRUE;
   
   AGNT_MUTEX_REL;
   
   return( rc );
}

/*****************************************************************************
*  FUNCTION:   iceagnt_ParseSdp
*
*  PURPOSE:    
*
*  PARAMETERS:
*              agentId     [IN] - agent Id
*              sdpAgent    [IN] - SDP offerer/answerer
*              pSdpParms   [IN] - SDP data
*
*  RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL iceagnt_ParseSdp( const BOS_UINT32 agentId, const ICE_SDP_AGENT sdpAgent, const ICE_SDP_PARMS *pSdpParms )
{
   BOS_BOOL rc = BOS_TRUE;
   ICEAGNT *pAgnt;

   AGNT_MUTEX_ACQ;

   if ( NULL == (pAgnt = iceagnt_GetAgentFromId( agentId ) ))
   {
      AGNT_MUTEX_REL;
      return( BOS_FALSE );
   }
 
   if ( BOS_TRUE != icesdp_Parse( pAgnt, pSdpParms ) )
   {
      AGNT_MUTEX_REL;
      return( BOS_FALSE );
   }

   DLOG(("iceagnt_ParseSdp: SDP Parsed, remote agent mode [%s]", icedbgTxtMapById( iceModeMap, pAgnt->rmtMode) ));

   /* Set ICE-Agent role */
   setAgentRole( pAgnt, sdpAgent );

   pAgnt->rmtCredential = pSdpParms->credential;

   if ( pAgnt->bLclSdpValid && pAgnt->bRmtSdpValid )
   {
      pAgnt->bLclSdpValid = BOS_FALSE;
      if ( pAgnt->state >= ICEAGNT_CTTY_CHECK_READY )
      {
         pAgnt->state = ICEAGNT_CTTY_CHECK_READY;
      }
   }
   pAgnt->bRmtSdpValid = BOS_TRUE;

   AGNT_MUTEX_REL;
   
   return( rc );
}

/*****************************************************************************
*  FUNCTION:   selectChkPair
*
*  PURPOSE:    Select a nominated pair from validated list. 
*              If none has been selected, select
*              the highest priority in the validated list 
*              (i.e. first in the list).
*              If a pair has been selected previously, if the check pair is
*              of higher priority of the previously selected pair, select
*              the current pair and de-select the previous one, and 
*              return BOS_TRUE.
*
*  PARAMETERS:
*              pAgnt             [MOD] - Agent
*
*  RETURNS:    BOS_TRUE if a new check pair has been selected.
*
*  NOTES:      
*
*****************************************************************************/
static BOS_BOOL selectChkPair( ICEAGNT *pAgnt )
{
   BOS_BOOL rc = BOS_FALSE;
   BOS_BOOL bNewSelected;
   ICE_COMP_ID compId[ICE_COMP_NUM_MAX]={ICE_COMP_RTP, ICE_COMP_RTCP};
   int i;
   ICECTTY_CHECK *pChk;

   BOS_ASSERT( NULL != pAgnt );

   for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
   {
      bNewSelected = BOS_FALSE;
      for ( pChk = pAgnt->chkLst.pHead; NULL != pChk; pChk = pChk->pNext )
      {
         if ( compId[i] == pChk->compId )
         {
            if ( !bNewSelected && pChk->bNominated && !pChk->bSelected )
            {
               /* Promote the higher priority Nominated pair to be Selected */
               pChk->bSelected = BOS_TRUE;
               bNewSelected = BOS_TRUE;
               rc = BOS_TRUE;
               DLOG(("selectChkPair: New selected pair compId[%d], priority high[%x], low[%x]", pChk->compId, (unsigned int)pChk->pairPri.high, (unsigned int)pChk->pairPri.low ));
            }
            else
            {
               /* Clear the selected flag */
               pChk->bSelected = BOS_FALSE;
            }
         }
      }
   }

   DLOG(("   --- New selected pair [%s]", rc?"TRUE":"FALSE"));

   return ( rc );
}

/*****************************************************************************
*  FUNCTION:   fillMediaTransport
*
*  PURPOSE:    Fill the Media Transport struct.
*
*  PARAMETERS:
*              pAgnt             [MOD] - Agent
*              pMediaTransport   [OUT] - Media Transport information
*
*  RETURNS:    none
*
*  NOTES:      
*
*****************************************************************************/
static void fillMediaTransport( ICEAGNT *pAgnt, ICE_MEDIA_TRANSPORT *pMediaTransport )
{
   ICE_COMP_ID compId[ICE_COMP_NUM_MAX]={ICE_COMP_RTP, ICE_COMP_RTCP};
   int i;
   ICECTTY_CHECK *pChk;

   BOS_ASSERT( NULL != pAgnt );
   BOS_ASSERT( NULL != pMediaTransport );

   memset( pMediaTransport, 0, sizeof(ICE_MEDIA_TRANSPORT) );
   
   for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
   {
      for ( pChk = pAgnt->chkLst.pHead; NULL != pChk; pChk = pChk->pNext )
      {
         if ( compId[i] == pChk->compId )
         {
            if ( pChk->bSelected )
            {
               pMediaTransport->num++;
               pMediaTransport->cndtPair[i].compId = pChk->compId;
               pMediaTransport->cndtPair[i].sid = pChk->sid;
               pMediaTransport->cndtPair[i].transType = pChk->transType;
               pMediaTransport->cndtPair[i].local = pChk->pLclCndt->transport.socAddr;
               pMediaTransport->cndtPair[i].remote = pChk->pRmtCndt->transport.socAddr;

               /* Generate local candidate aline for this component */
               icesdp_GenerateAline( pMediaTransport->cndtPair[i].lCndtAline,
                                     pChk->pLclCndt,
                                     pChk->compId,
                                     pChk->transType );
               
               /* Generate remote candidate aline if it is a controlling agent */
               if ( ICECTTY_CONTROLLING == pAgnt->role)
               {
                  icesdp_GenerateAline( pMediaTransport->cndtPair[i].rCndtAline,
                                        pChk->pRmtCndt,
                                        pChk->compId,
                                        pChk->transType );
               }

               break;
            }
         }
      }
   }
}


/*****************************************************************************
*  FUNCTION:   ackPeerBindingRequest
*
*  PURPOSE:    Respond to Peer Binding Request:
*              - Authenticate and send Binding Response
*
*  PARAMETERS:
*              pData        [IN] - Parsed STUN request
*
*  RETURNS:    none
*
*  NOTES:      
*
*****************************************************************************/
static void ackPeerBindingRequest( const STUN_REQ_DATA *pData )
{
   ICEAGNT *pTmpAgnt;
   STUN_TRANSACTION_ID *pTxid;
   BOS_UINT8 msgBuf[STUN_MSG_BUFFER_SIZE];
   BOS_UINT16 size;
   STUN_HMAC_PWD pwd;
   STUN_HMAC_PWD *pPwd = NULL;
   STUN_DATA username;
   STUN_DATA integrity;
   BOS_UINT8 uBuf[STUN_MAX_DATA_LEN];
   BOS_UINT8 iBuf[STUN_MAX_DATA_LEN];
   BOS_BOOL bValid = BOS_TRUE;

   BOS_ASSERT( NULL != pData );
            
   memset( msgBuf, 0, sizeof( msgBuf ) );
   pwd.len = 0;

   username.pData = uBuf;
   integrity.pData = iBuf;

   /* Send response with Integrity if request contains
    * username and integrity 
    */
   if ( stunGetAttribute( pData->pMsg, STUN_ATTR_USERNAME, &username ) &&
        stunGetAttribute( pData->pMsg, STUN_ATTR_MESSAGE_INTEGRITY, &integrity ) )
   {
      /* Find agent that matches the username. If found, use password for 
       * Message Integrity 
       */
      AGNT_MUTEX_ACQ;
      /* Find agent */
      for ( pTmpAgnt = pAgntLstHead; NULL != pTmpAgnt; pTmpAgnt = pTmpAgnt->pNext )
      {
         if ( NULL != strstr((char*)username.pData, pTmpAgnt->lclCredential.ufrag ))
         {
            /* Authenticate request */
            if ( pData->hashDataLen > 0 )
            {
               pwd.len = (BOS_UINT16)strlen(pTmpAgnt->lclCredential.pwd);
               if ( pwd.len > 0 )
               {
                  memcpy( pwd.val, pTmpAgnt->lclCredential.pwd, pwd.len );

                  if ( !stunIntegrityCheck( pData->pHashData,
                                            pData->pHash,
                                            pData->hashDataLen,
                                            &pwd) )
                  {
                     LOG(("ackPeerBindingRequest: Failed integrity check"));
                     bValid = BOS_FALSE;
                  }
                  else
                  {
                     /* Prepare password for Message Integrity in Response */
                     pPwd = &pwd;
                  }
               }
               else
               {
                  DLOG(("ackPeerBindingRequest: local credential not available yet"));
               }
            }
            break;
         }
      }

      AGNT_MUTEX_REL;
   }

   if ( bValid )
   {
      pTxid = stunGetTransactionId( pData->pMsg );
      /* set transaction id */
      stunSetTransactionId( msgBuf, pTxid );

      stunSetMsgType( msgBuf, STUN_MSG_BINDING_RESPONSE );

      stunSetAttribute( msgBuf, STUN_ATTR_MAPPED_ADDRESS, (void *)pData->pSrcAddr );
      stunSetAttribute( msgBuf, STUN_ATTR_XORMAPPED_ADDRESS, (void *)pData->pSrcAddr );

      size = sizeof( msgBuf );
      if ( BOS_TRUE != stunEncode( msgBuf, &size, pPwd, 0 ) )
      {
         BOS_ASSERT( BOS_FALSE );
      }

      (void)stunSendResponse( pData->sid, pData->pSrcAddr, msgBuf, size );
   }
   else
   {
      LOG(("ackPeerBindingRequest: Request failed authentication, no response sent"));
   }
}

/*****************************************************************************
*
*  FUNCTION:   getStunServerFromId
*
*  PURPOSE:    Return STUN server from the STUN server id.
*
*  PARAMETERS:
*              pList [IN] - STUN server list
*              stunServerId [IN] - STUN server id
*
*  RETURNS:    pointer to STUN server structure. NULL if not found.
*
*  NOTES:
*
*****************************************************************************/
static ICE_STUN_SERVER *getStunServerFromId( const ICE_STUN_SERVER_LIST *pList,
                                             const BOS_UINT16 stunServerId )
{
   int i;
   ICE_STUN_SERVER *pServer = NULL;
   BOS_ASSERT( NULL != pList );
   for ( i = 0; i < ICE_STUN_SERVER_MAX && i < pList->num; i++ )
   {
      if ( stunServerId == pList->server[i].id )
      {
         pServer = (ICE_STUN_SERVER *)&pList->server[i];
         break;
      }
   }
   return( pServer );
}

