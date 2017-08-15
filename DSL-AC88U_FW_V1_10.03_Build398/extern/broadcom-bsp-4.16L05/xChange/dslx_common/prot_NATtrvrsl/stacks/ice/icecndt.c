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
*      ICE Candidate gathering and prioritizing Module.
*      
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <bosTypes.h>
#include <bosSocket.h>
#include <stunc.h>
#include <stun.h>
#include "icei.h"
#include "icecndt.h"
#include "iceagnt.h"
#include "icedbg.h"

/* ---- Public Variables ------------------------------------- */
/* ---- Private Constants and Types -------------------------- */
/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
static BOS_BOOL removeCndtFromList( ICECNDT *pCndt, ICECNDT_LST *pCndtLst );
static void setCndtAddr( ICECNDT_ADDR *pCndtAddr, const STUN_TRANSPORT *pAddr );

#if ICEDBG_LOG
void dumpCndtLstAll( BOS_UINT32 agentId );
void dumpCndtLst( ICECNDT_LST *pCndtLst, BOS_BOOL bDumpCndt );
void dumpCndt( ICECNDT *pCndt );
#endif
/* ---- Functions -------------------------------------------- */

/*****************************************************************************
*
*  FUNCTION:   icecndt_NewList
*
*  PURPOSE:    New Candidate list
*
*  PARAMETERS:
*              none
*
*  RETURNS:    New Candidate list if successful. NULL otherwise.
*
*  NOTES:
*
*****************************************************************************/
ICECNDT_LST *icecndt_NewList( void )
{
   ICECNDT_LST *pCndtLst;

   pCndtLst = (ICECNDT_LST *)malloc( sizeof(ICECNDT_LST ));

   if ( NULL != pCndtLst )
   {
      memset( pCndtLst, 0, sizeof(ICECNDT_LST) );

      pCndtLst->state = CNDTLST_OFFLINE;
      pCndtLst->maxWait = 0;
      pCndtLst->pStunServer = NULL;
      pCndtLst->compId = (ICE_COMP_ID)ICE_UNKNOWN;
      pCndtLst->pCndtHead = NULL;
      pCndtLst->cndtCount = 0;
      pCndtLst->reqHdl = ICE_UNKNOWN;
      memset(&pCndtLst->secret, 0, sizeof(STUNC_SHARED_SECRET));
      pCndtLst->sid = 0;
   }
   else
   {
      BOS_ASSERT( BOS_FALSE );
   }

   return ( pCndtLst );
}

/*****************************************************************************
*
*  FUNCTION:   icecndt_DeleteList
*
*  PURPOSE:    Delete all candidates on the list and free resources.
*
*  PARAMETERS:
*              pCndtLst [IN] - Candidate list to be deleted.
*
*  RETURNS:    BOS_TRUE if successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
void icecndt_DeleteList( ICECNDT_LST *pCndtLst )
{
   ICECNDT *pCndt;

   BOS_ASSERT( NULL != pCndtLst );

   /* remove candidates from the list */
   while ( NULL != (pCndt = pCndtLst->pCndtHead) )
   {
      pCndtLst->pCndtHead = pCndt->pNext;
      icecndt_DeleteCndt( pCndt );
   }
}

/*****************************************************************************
*  FUNCTION:   icecndt_NewCndt
*
*  PURPOSE:    Allocate resources and initialize a new candidate.
*
*  PARAMETERS:
*              none
*
*  RETURNS:    New Candidate, NULL if failed.
*
*  NOTES:
*
*****************************************************************************/
ICECNDT *icecndt_NewCndt( void )
{
   ICECNDT *ptr = malloc(sizeof(ICECNDT));
   if ( ptr )
   {
      memset( ptr, 0, sizeof(ICECNDT) );
   }
   else
   {
      BOS_ASSERT( BOS_FALSE );
   }

   return( ptr );
}

/*****************************************************************************
*  FUNCTION:   icecndt_DeleteCndt
*
*  PURPOSE:    Free previously allocated candidate 
*
*  PARAMETERS:
*              pCndt [IN] - candidate to be free
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void icecndt_DeleteCndt( ICECNDT *pCndt )
{
   if ( NULL != pCndt )
   {
      pCndt->pPrev = NULL;
      pCndt->pNext = NULL;
      free( pCndt );
   }
}

/*****************************************************************************
*
*  FUNCTION:   icecndt_GatherHostCndt
*
*  PURPOSE:    Gather host candidate for the specified interface, 
*              and add it to an existing candidate list.
*
*  PARAMETERS:
*              pCndtLst    [MOD] - Candidate list
*              pLocalIf    [IN] - Local interface
*              port        [IN] - Port
*              compId      [IN] - Component ID
*
*  RETURNS:    BOS_TRUE if successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL icecndt_GatherHostCndt( ICECNDT_LST           *pCndtLst,
                                 const ICE_LOCAL_IF    *pLocalIf, 
                                 const BOS_UINT16      port, 
                                 const ICE_COMP_ID     compId )
{
   BOS_BOOL rc = BOS_TRUE;
   STUN_TRANSPORT socAddr;
   ICECNDT *pCndt;

   BOS_ASSERT( NULL != pCndtLst );

   memset( &socAddr, 0, sizeof( STUN_TRANSPORT ) );

   if ( (NULL != pCndtLst) && (NULL != (pCndt = icecndt_NewCndt())) )
   {
      pCndt->cndtId = pCndtLst->cndtCount;
      pCndt->state = ICECNDT_FROZEN;
      pCndt->type = ICECNDT_HOST;
      pCndt->pBaseCndt = pCndt;  /* Base is host candidate itself */

      pCndt->priority = icecndt_ComputePriority( pCndtLst->priTypePref.host, 
                                                 pCndtLst->priLocalPref, 
                                                 compId );
      pCndt->bInUse = BOS_FALSE;

      socAddr.ipAddr = pLocalIf->address;
      socAddr.port = (BOS_SOCKET_IP_PORT)port;

      setCndtAddr( &pCndt->transport, &socAddr );
      pCndt->pRelated = NULL;
      sprintf( pCndt->foundation, 
               "%d", 
               (pLocalIf->id)<<4|ICECNDT_FOUNDATION_HOST);
      
      rc = icecndt_AddCndtToList( pCndt, pCndtLst );
   }
   else
   {
      rc = BOS_FALSE;
   }

   return( rc );
}

/*****************************************************************************
*  FUNCTION:   icecndt_AddCndtToList
*
*  PURPOSE:    Insert a candidate into the end of a candidate list and
*              increment total number of candidate count.
*
*  PARAMETERS:
*              pCndt    [MOD] - candidate
*              pCndtLst [MOD] - candidate list
*
*  RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL icecndt_AddCndtToList( ICECNDT *pCndt, ICECNDT_LST *pCndtLst )
{
   ICECNDT *pTmp;

   BOS_ASSERT( NULL != pCndt && NULL != pCndtLst );
   
   if ( NULL != pCndt && NULL != pCndtLst )
   {
      if ( NULL != pCndtLst->pCndtHead )
      {
         for ( pTmp = pCndtLst->pCndtHead; 
               NULL != pTmp->pNext; 
               pTmp = pTmp->pNext )
         {
            ;;
         }
         pTmp->pNext = pCndt;
         pCndt->pPrev = pTmp;
         pCndt->pNext = NULL;
      }
      else
      {
         pCndt->pPrev = NULL;
         pCndt->pNext = NULL;
         pCndtLst->pCndtHead = pCndt;
      }

      pCndtLst->cndtCount++;
      return( BOS_TRUE );
   }
   else
   {
      return( BOS_FALSE );
   }
}

/*****************************************************************************
*  FUNCTION:   removeCndtFromList
*
*  PURPOSE:    Remove the candidate from the candidate list it belongs.
*
*  PARAMETERS:
*              pCndt       [MOD] - candidate
*              pCndtLst    [MOD] - candidate list
*
*  RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL removeCndtFromList( ICECNDT *pCndt, ICECNDT_LST *pCndtLst )
{
   BOS_ASSERT( NULL != pCndt && NULL != pCndtLst );
   
   if ( NULL != pCndt && NULL != pCndtLst )
   {
      if ( NULL != pCndt->pNext )
      {
         pCndt->pNext->pPrev = pCndt->pPrev;
      }

      if ( NULL != pCndt->pPrev )
      {
         pCndt->pPrev->pNext = pCndt->pNext;
      }
      else
      {
         /* First in the list */
         pCndtLst->pCndtHead = pCndt->pNext;
      }
      pCndtLst->cndtCount--;
      return( BOS_TRUE );
   }
   else
   {
      return( BOS_FALSE );
   }
}

/*****************************************************************************
*  FUNCTION:   icecndt_ComputePriority
*
*  PURPOSE:    Calculate priority value based on the formula:
*              priority = (2^24)*(type preference) + 
*                         (2^8)*(local preference) +
*                         (2^0)*(256 - component ID)
*
*  PARAMETERS:
*              typePref    [IN] - type preference
*              localPref   [IN] - local preference
*              compId      [IN] - component ID
*
*  RETURNS:    priority
*
*  NOTES:
*
*****************************************************************************/
BOS_UINT32 icecndt_ComputePriority( const BOS_UINT8 typePref, const BOS_UINT16 localPref, const BOS_UINT8 compId)
{
   BOS_ASSERT( (typePref <= ICE_TYPEPREF_MAX) && (compId >= ICE_COMP_MIN));

   return( ((1<<24)*typePref)+(1<<8)*localPref+(256-compId) );
}

/*****************************************************************************
*  FUNCTION:   setCndtAddr
*
*  PURPOSE:    Set the ice candidate address.
*
*  PARAMETERS:
*              pCndtAddr  [MOD] - candidate address
*              pAddr      [IN] - socket address
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static void setCndtAddr( ICECNDT_ADDR *pCndtAddr, const STUN_TRANSPORT *pAddr )
{
   BOS_ASSERT( NULL != pCndtAddr && NULL != pAddr );
   
   pCndtAddr->bValid = BOS_TRUE;
   pCndtAddr->socAddr = *pAddr;
}

/*****************************************************************************
*  FUNCTION:   icecndt_SendRequest
*
*  PURPOSE:    Send STUN requests to STUN/TURN server.
*
*  PARAMETERS:
*              pCndtLst    [IN] - Candidate list
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void icecndt_SendRequest( ICECNDT_LST *pCndtLst )
{
   STUNC_SHARED_SECRET *pSecret = NULL;
   STUN_RETRANS         reTx;

   
   BOS_ASSERT( NULL != pCndtLst );
   BOS_ASSERT( NULL != pCndtLst->pStunServer );

   if ( (pCndtLst->secret.username.size > 0) && (pCndtLst->secret.username.size > 0 ))
   {
      pSecret = &pCndtLst->secret;
   }

   BOS_ASSERT( 0 == pCndtLst->reqHdl );

   if ( ICE_SRV_STUN == pCndtLst->pStunServer->type )
   {
      memset ( &reTx,
               0,
               sizeof ( STUN_RETRANS ) );
      reTx.uMaxWaitTime = pCndtLst->maxWait;

      if ( STUNC_SUCCESS == stuncPublicAddr( 
                                 pCndtLst->sid,
                                 &pCndtLst->pStunServer->socAddr,
                                 pSecret,
                                 0,
                                 &pCndtLst->reqHdl,
                                 iceagnt_ProcessStuncRes,
                                 &reTx
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
                                 ,NULL
#endif
                                 ) )
      {
         pCndtLst->state = CNDTLST_GATHERING;
      }
      else
      {
         pCndtLst->state = CNDTLST_TIMEOUT;
      }
   }
#if NTCFG_TURN_SUPPORT
   else if ( ICE_SRV_TURN == pCndtLst->pStunServer->type )
   {
      if ( TURNC_SUCCESS == turncAllocate( pCndtLst->sid,
                                           &pCndtLst->pStunServer->socAddr,
                                           (TURNC_SHARED_SECRET *)pSecret,
                                           NULL,
                                           &pCndtLst->reqHdl) )
      {
         pCndtLst->state = CNDTLST_GATHERING;
      }
      else
      {
         pCndtLst->state = CNDTLST_TIMEOUT;
      }
   }
#endif
   else
   {
      BOS_ASSERT( BOS_FALSE );
   }
}


/*****************************************************************************
*  FUNCTION:   icecndt_GatherStunCndt
*
*  PURPOSE:    Process STUN responses and add Server Reflexive candidate 
*              to candidate list accordingly.
*
*  PARAMETERS:
*              pCndtLst    [MOD] - Candidate list
*              pEvtData    [IN] - STUNC response data
*
*  RETURNS:    BOS_TRUE if candidate has been added, BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL icecndt_GatherStunCndt( ICECNDT_LST          *pCndtLst,
                                 const STUNC_EVT_DATA *pEvtData )
{
   ICECNDT *pCndt;
   BOS_BOOL rc = BOS_FALSE;

   BOS_ASSERT( NULL != pCndtLst );
   BOS_ASSERT( NULL != pEvtData );
   switch( pEvtData->evt )
   {
      case STUNC_EVT_MAPPED_ADDRESS:
      {
         STUN_TRANSPORT *pSocAddr = (STUN_TRANSPORT *)&pEvtData->data.socAddr;
         BOS_ASSERT( NULL != pSocAddr );

         if ( NULL != (pCndt = icecndt_NewCndt()) )
         {
            pCndt->cndtId = pCndtLst->cndtCount;
            pCndt->state = ICECNDT_FROZEN;
            pCndt->type = ICECNDT_SRVRFLX;
            pCndt->pBaseCndt = pCndtLst->pCndtHead; 

            pCndt->priority = icecndt_ComputePriority( 
                                       pCndtLst->priTypePref.srflx, 
                                       pCndtLst->priLocalPref, 
                                       pCndtLst->compId );

            pCndt->bInUse = BOS_FALSE;
            setCndtAddr( &pCndt->transport, pSocAddr );
            pCndt->pRelated = &pCndtLst->pCndtHead->transport;
            sprintf( pCndt->foundation,
                     "%d",
                     (pCndtLst->pLocalIf->id<<4) |
                     (pCndtLst->pStunServer->id<<2) |
                     ICECNDT_FOUNDATION_SRFLX );
            
            rc = icecndt_AddCndtToList( pCndt, pCndtLst );
         }
         else
         {
            rc = BOS_FALSE;
         }
      }
      break;
      case STUNC_EVT_RES_ERROR:
      case STUNC_EVT_TX_TIMEOUT:
      case STUNC_EVT_SHARED_SECRET:
      case STUNC_EVT_SYSTEM_ERR:
      default:
      {
         rc = BOS_FALSE;
      }
   }

   if ( BOS_TRUE == rc )
   {
      pCndtLst->state = CNDTLST_COMPLETE;
   }
   else
   {
      pCndtLst->state = CNDTLST_TIMEOUT;
   }

   return ( rc );
}

#if NTCFG_TURN_SUPPORT
/*****************************************************************************
*  FUNCTION:   icecndt_GatherTurnCndts
*
*  PURPOSE:    Process TURN responses and add Server Reflexive and Server 
*              Relayed candidates to an existing candidate list accordingly.
*
*  PARAMETERS:
*              pCndtLst    [IN] - Candidate list
*              pData       [IN] - TURN response data
*
*  RETURNS:    BOS_TRUE if candidate has been added.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL icecndt_GatherTurnCndts( ICECNDT_LST           *pCndtLst,
                                  const TURNC_EVT_DATA  *pEvtData )
{
   ICECNDT *pCndt;
   BOS_BOOL rc = BOS_FALSE;

   BOS_ASSERT( NULL != pCndtLst );
   BOS_ASSERT( NULL != pEvtData );
   switch( pEvtData->evt )
   {
      case TURNC_EVT_ALLOCATE_SUCCESS:
      {
         TURNC_ALLOCATE_DATA *pAllocate = (TURNC_ALLOCATE_DATA *)&pEvtData->data.allocAddrs;
         if ( NULL != (pCndt = icecndt_NewCndt()) )
         {
            pCndt->cndtId = pCndtLst->cndtCount;
            pCndt->state = ICECNDT_FROZEN;
            pCndt->type = ICECNDT_SRVRFLX;
            pCndt->pBaseCndt = pCndtLst->pCndtHead; 

            pCndt->priority = icecndt_ComputePriority( 
                                       pCndtLst->priTypePref.srflx, 
                                       pCndtLst->priLocalPref, 
                                       pCndtLst->compId );
            pCndt->bInUse = BOS_FALSE;
            setCndtAddr( &pCndt->transport, &pAllocate->reflexiveAddr );
            pCndt->pRelated = &pCndtLst->pCndtHead->transport;
            sprintf( pCndt->foundation, "%d",
                     (pCndtLst->pLocalIf->id<<4) |
                     (pCndtLst->pStunServer->id<<2) |
                     ICECNDT_FOUNDATION_SRFLX );
            
            rc = icecndt_AddCndtToList( pCndt, pCndtLst );
         }
         else
         {
            rc = BOS_FALSE;
         }

         if ( rc && (NULL != (pCndt = icecndt_NewCndt())) )
         {
            pCndt->cndtId = pCndtLst->cndtCount;
            pCndt->state = ICECNDT_FROZEN;
            pCndt->type = ICECNDT_RELAYED;
            pCndt->pBaseCndt = pCndtLst->pCndtHead; 

            pCndt->priority = icecndt_ComputePriority( 
                                       pCndtLst->priTypePref.relayed, 
                                       pCndtLst->priLocalPref, 
                                       pCndtLst->compId );
            pCndt->bInUse = BOS_FALSE;
            setCndtAddr( &pCndt->transport, &pAllocate->relayAddr );
            pCndt->pRelated = &pCndtLst->pCndtHead->pNext->transport;
            sprintf( pCndt->foundation, "%d", 
                     (pCndtLst->pLocalIf->id<<4) |
                     (pCndtLst->pStunServer->id<<2) |
                     ICECNDT_FOUNDATION_RELAYED );
            
            rc = icecndt_AddCndtToList( pCndt, pCndtLst );
         }
         else
         {
            rc = BOS_FALSE;
         }
      }
      break;
      case TURNC_EVT_ALLOCATE_ERROR:
      case TURNC_EVT_ACTIVE_DEST_SET:
      case TURNC_EVT_ACTIVE_DEST_UNSET:
      case TURNC_EVT_ACTIVE_DEST_ERROR:
      case TURNC_EVT_CONNECT_SUCCESS:
      case TURNC_EVT_CONNECT_ERROR:
      case TURNC_EVT_CONNECT_STATUS:
      case TURNC_EVT_SYSTEM_ERR:
      default:
      {
         rc = BOS_FALSE;
      }
   }

   if ( BOS_TRUE == rc )
   {
      pCndtLst->state = CNDTLST_COMPLETE;
   }
   else
   {
      pCndtLst->state = CNDTLST_TIMEOUT;
   }

   return ( rc );
}
#endif

/*****************************************************************************
*  FUNCTION:   icecndt_FinalizeCndtLst
*
*  PURPOSE:    Eliminate redundant candidates from the list.
*
*  PARAMETERS:
*              pCndtLst    [MOD] - Candidate list
*
*  RETURNS:    none
*
*  NOTES:
*              Given the following conditions how the candidate list is 
*              formed,
*                 i)    using a single interface,
*                 ii)   using a single STUN server,
*                 iii)  and orignated from the same candidate base,
*              it is therefore sufficient to remove redundant candidate 
*              just by comparing the transport address.
*
*****************************************************************************/
void icecndt_FinalizeCndtLst( ICECNDT_LST  *pCndtLst )
{
   ICECNDT *pCndt1;
   ICECNDT *pCndt2;
   ICECNDT *pNext = NULL;
   BOS_ASSERT( NULL != pCndtLst );

   if  (NULL != pCndtLst->pCndtHead)
   {
      /* Remove redundant candidates */
      for ( pCndt1 = pCndtLst->pCndtHead->pNext; NULL != pCndt1; pCndt1 = pNext )
      {
         pNext = pCndt1->pNext;
         for ( pCndt2 = pCndtLst->pCndtHead; NULL != pCndt2; pCndt2 = pCndt2->pNext )
         {
            if ( pCndt2 != pCndt1 )
            {
               if ( !memcmp( &pCndt2->transport, &pCndt1->transport, sizeof( ICECNDT_ADDR ) ) )
               {
                  DLOG(("finalizeCndtLst: redundant candidate, remove from list"));
                  removeCndtFromList( pCndt1, pCndtLst );
                  icecndt_DeleteCndt( pCndt1 );
                  break;
               }
            }
         } /* for */
      }/* for */
   }
   else
   {
      DLOG(("finalizeCndtLst: Candidate list EMPTY"));
      return;
   }
}

#if ICEDBG_LOG
void dumpCndtLstAll( BOS_UINT32 agentId )
{
   ICEAGNT *pAgnt;
   int i;
   
   if ( NULL == (pAgnt = iceagnt_GetAgentFromId( agentId )) )
   {
      DLOG(("Agent NOT FOUND"));
      return;
   }

   DLOG(("LOCAL Candidate List"));
   for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
   {
      if ( NULL != pAgnt->pLclCndtLst[i] )
      {
         dumpCndtLst( pAgnt->pLclCndtLst[i], BOS_TRUE );
      }
   }

   DLOG(("REMOTE Candidate List"));
   for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
   {
      if ( NULL != pAgnt->pRmtCndtLst[i] )
      {
         dumpCndtLst( pAgnt->pRmtCndtLst[i], BOS_TRUE );
      }
   }
}

void dumpCndtLst( ICECNDT_LST *pCndtLst, BOS_BOOL bDumpCndt )
{
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];

   ICECNDT *pCndt = pCndtLst->pCndtHead;
   
   DLOG(("Candidate List"));
   DLOG(("--->state      [%s]", TMAP(icecndtLstStateMap, pCndtLst->state)));
   DLOG(("--->hostpref   [%d]", pCndtLst->priTypePref.host));
   DLOG(("--->srflxpref  [%d]", pCndtLst->priTypePref.srflx));
   DLOG(("--->prflxpref  [%d]", pCndtLst->priTypePref.prflx));
   DLOG(("--->relaypref  [%d]", pCndtLst->priTypePref.relayed));
   DLOG(("--->localpref  [%d]", pCndtLst->priLocalPref));
   DLOG(("--->maxWait    [%d]", pCndtLst->maxWait));
   if ( NULL != pCndtLst->pStunServer )
   {
      DLOG(("--->stunserver Configured"));
      DLOG(("---+--->id    [%d]", pCndtLst->pStunServer->id));
      DLOG(("---+--->type  [%d]", pCndtLst->pStunServer->type));
      (void)bosIpAddrGetStr(&pCndtLst->pStunServer->socAddr.ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
      DLOG(("---+--->ip    [%s]", ipAddrStr));
      DLOG(("---+--->port  [%d]", pCndtLst->pStunServer->socAddr.port));
   }
   DLOG(("--->compId     [%s]", TMAP(iceCompIdMap, pCndtLst->compId)));
   DLOG(("--->transType [%s]", TMAP(iceTransportMap, pCndtLst->transType)));
   DLOG(("--->cndtCount  [%d]", pCndtLst->cndtCount));
   DLOG(("--->reqHdl     [%d]", (int)pCndtLst->reqHdl));
   DLOG(("--->secret.usr [%d] len", pCndtLst->secret.username.size));
   DLOG(("--->secret.pwd [%d] len", pCndtLst->secret.password.size));
   DLOG(("--->sid        [%lx]", pCndtLst->sid));
   if ( NULL != pCndtLst->pLocalIf )
   {
      DLOG(("--->pLocalIf  id [%d]", pCndtLst->pLocalIf->id));
      (void)bosIpAddrGetStr(&pCndtLst->pLocalIf->address, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
      DLOG(("--->pLocalIf  ip [%s]", ipAddrStr));
   }
   else
   {
      DLOG(("--->pLocalIf = NULL"));
   }
   DLOG(("--->port      [%d]", pCndtLst->port));

   if ( bDumpCndt )
   {
      while ( NULL != pCndt )
      {
         dumpCndt( pCndt );
         pCndt = pCndt->pNext;
      }
   }
}

void dumpCndt( ICECNDT *pCndt )
{
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
   
   DLOG(("Candidate     [%p]", pCndt));
   DLOG(("Candidate     [%d]", pCndt->cndtId));
   DLOG(("--->state     [%s]", TMAP(icecndtStateMap, pCndt->state)));
   DLOG(("--->type      [%s]", TMAP(icecndtTypeMap,pCndt->type)));
   if ( NULL != pCndt->pBaseCndt )
   {
      DLOG(("--->baseCndtId[%d]", pCndt->pBaseCndt->cndtId));
   }
   else
   {
      DLOG(("--->baseCndtId[UNDEFINED]"));
   }
   DLOG(("--->priority  [%d]", (int)pCndt->priority));
   DLOG(("--->bInUse    [%d]", pCndt->bInUse));
   if ( pCndt->transport.bValid )
   {
      DLOG(("--->transport Valid"));
      (void)bosIpAddrGetStr(&pCndt->transport.socAddr.ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
      DLOG(("---+--->ip    [%s]", ipAddrStr));
      DLOG(("---+--->port  [%d]", pCndt->transport.socAddr.port));
   }
   else
   {
      DLOG(("--->transport NOT valid"));
   }

   if ( pCndt->pRelated && pCndt->pRelated->bValid )
   {
      DLOG(("--->pRelated Valid"));
      (void)bosIpAddrGetStr(&pCndt->pRelated->socAddr.ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
      DLOG(("---+--->ip    [%s]", ipAddrStr));
      DLOG(("---+--->port  [%d]", pCndt->pRelated->socAddr.port));
   }
   else
   {
      DLOG(("--->pRelated NOT valid"));
   }

   DLOG(("--->foundation [%s]", pCndt->foundation));
}

#endif
