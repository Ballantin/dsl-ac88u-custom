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
*      ICE Connectivity Check module.
*      
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <stdio.h>
#include <stun.h>
#include "icei.h"
#include "icectty.h"
#include "iceagnt.h"
#include "icedbg.h"

/* ---- Public Variables ------------------------------------- */
/* ---- Private Constants and Types -------------------------- */
#undef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#undef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))

/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
static void sortCheckList( ICECTTY_CHKLST *pChkLst );
static void pruneCheckList( ICECTTY_CHKLST *pChkLst );
static ICECTTY_CHECK *getFirstCheckByState( const ICECTTY_CHKLST *pChkLst,
                                            const ICECTTY_CHECK_STATE state );
static void computePairPriority( const BOS_UINT32 offererPriority, const BOS_UINT32 answererPriority, ICECTTY_UINT64 *pPriority );
static BOS_BOOL isGreaterUINT64( const ICECTTY_UINT64 *pFirst, const ICECTTY_UINT64 *pSecond );
static BOS_BOOL icectty_StunEvtHdlr( const BOS_UINT32         reqHdl,
                                     const STUN_RES_DATA      *pResData );
static BOS_BOOL addCheckPair( ICECNDT_LST             *pRmtCndtLst[],
                              ICECTTY_CHKLST          *pChkLst,
                              const BOS_UINT32        sid,
                              const ICE_COMP_ID       compId,
                              const ICECNDT           *pLclCndt,
                              const STUN_TRANSPORT    *pRmtTransport, 
                              const ICE_TYPEPREF      *pPriTypePref,
                              const BOS_UINT16        *pPriLocalPref,
                              const BOS_UINT32        rmtPriority,
                              const ICECTTY_ROLE      role,
                              ICECTTY_CHECK           **ppChk );
static ICECTTY_CHECK *newChk( void );
static void deleteChk( ICECTTY_CHECK *pChk );
static void addChkToList( ICECTTY_CHECK *pChk, ICECTTY_CHKLST *pChkLst );
static BOS_BOOL isRedundantPair( const ICECTTY_CHECK *pChk1, const ICECTTY_CHECK *pChk2 );

/* ---- Functions -------------------------------------------- */
/*****************************************************************************
*  FUNCTION:   icectty_FormCheckList
*
*  PURPOSE:    Form connectivity check list:
*              1. Pairs up local and remote candidates in offer/answer 
*                 exchange to form a list 
*              2. Computes pair priority.
*              3. Orders the pairs based on priority
*              4. Prunes list by removing redundant pairs.
*              5. Sets the state of each check to Frozen.
*
*  PARAMETERS:
*              pLclCndtLst [IN] - Local candidate list
*              pRmtCndtLst [IN] - Remote candidate list
*              pChkLst     [OUT] - Generated Check List
*              role        [IN] - Agent role
*
*  RETURNS:    BOS_TRUE if successfull, BOS_FAIL otherwise.
*
*  NOTES:      The agent parameters are not to be modified when this
*              function is excuted. Typically, this is under the
*              protection of a mutex in the iceagnt module.
*
*****************************************************************************/
BOS_BOOL icectty_FormCheckList( ICECNDT_LST           *pLclCndtLst[],
                                ICECNDT_LST           *pRmtCndtLst[],
                                ICECTTY_CHKLST        *pChkLst,
                                const ICECTTY_ROLE    role )
{
   int i,j;
   ICECNDT  *pLclCndt;  /* Candidate from local list */
   ICECNDT  *pRmtCndt;  /* Candidate from remote list */
   BOS_BOOL bLocalIsOfferer;

   BOS_ASSERT( (NULL != pLclCndtLst) &&
               (NULL != pRmtCndtLst) &&
               (NULL != pChkLst) );
   
   if ( (NULL == pLclCndtLst[0]) ||
        (NULL == pRmtCndtLst[0]) )
   {
      return( BOS_FALSE );
   }

   bLocalIsOfferer = (role == ICECTTY_CONTROLLING)?BOS_TRUE:BOS_FALSE;

   LOG(("icectty_FormCheckList: role [%d]...", role));
   DLOG(("   role [%s]", TMAP( icecttyRoleMap, role )));
   DLOG(("   chkList state at entry [%s]", TMAP( icecttyChkLstStateMap, pChkLst->state)));
   
   /* Form candidate pairs 
    * A local candidate is paired with a remote candidate if and
    * only if the two candidates have the same component ID and have the
    * same IP address version.
    */
   for ( i = 0; 
         ((NULL != pLclCndtLst[i]) && (i<ICE_COMP_NUM_MAX)); 
         i++ )
   {
      for ( j = 0; 
            ((NULL != pRmtCndtLst[j]) && (j<ICE_COMP_NUM_MAX)); 
            j++ )
      {
         if ( pRmtCndtLst[j]->compId != pLclCndtLst[i]->compId ) 
         {
            continue;
         }

         for ( pLclCndt = pLclCndtLst[i]->pCndtHead;
               NULL != pLclCndt; 
               pLclCndt = pLclCndt->pNext )
         {
            for ( pRmtCndt = pRmtCndtLst[j]->pCndtHead; 
                  NULL != pRmtCndt;
                  pRmtCndt = pRmtCndt->pNext )
            {
               if ( (pLclCndt->transport.bValid && pRmtCndt->transport.bValid ) &&
                    ( bosIpAddrGetType(&pLclCndt->transport.socAddr.ipAddr) ==
                      bosIpAddrGetType(&pRmtCndt->transport.socAddr.ipAddr) ) )
               {
                  /* Form pair, compute priority and initialize state */
                  if ( pChkLst->num < ICECTTY_CNDT_PAIR_MAX )
                  {
                     ICECTTY_CHECK *pChk = newChk();
                     /* Preserve the local candidate type */
                     pChk->lclCndtType = pLclCndt->type;

                     pChk->transType = pLclCndtLst[i]->transType;

                     pChk->pLclCndt = pLclCndt;

                     pChk->pRmtCndt = pRmtCndt;

                     if ( bLocalIsOfferer )
                     {
                        computePairPriority( pLclCndt->priority, pRmtCndt->priority, &pChk->pairPri ) ;
                     }
                     else
                     {
                        computePairPriority( pRmtCndt->priority, pLclCndt->priority, &pChk->pairPri ) ;
                     }

                     /* Set pair state to Frozen */
                     pChk->state = ICECTTY_FROZEN;

                     pChk->sid = pLclCndtLst[i]->sid;

                     pChk->compId = pRmtCndtLst[j]->compId;

                     pChk->bRedundant = BOS_FALSE;

                     addChkToList( pChk, pChkLst );

                  } /* if */
               } /* if */
            } /* for */
         } /* for */
      } /* for */
   } /* for */
   
   /* Sort list based on pair priority value */
   DLOG(("   Sort Check list"));
   sortCheckList( pChkLst );

   /* Prune list
    * (draft-ietf-mmusic-ice-13.txt)
    * For each pair where the local candidate is server reflexive, 
    * the server reflexive candidate MUST be replaced by its base.  
    * Once this has been done, the agent MUST remove redundant pairs.  
    * A pair is redundant if its local and remote candidates are 
    * identical to the local and remote candidates of a pair 
    * higher up on the priority list.
    */
   DLOG(("   Prune Check list"));
   pruneCheckList( pChkLst );

   /* Check list ready connectivity check */ 
   pChkLst->state = ICECTTY_CL_RUNNING;
   pChkLst->bNewSelected = BOS_FALSE;
   pChkLst->bAllChecksSent = BOS_FALSE;
   DLOG(("   chkList state at exit [%s]", TMAP( icecttyChkLstStateMap, pChkLst->state)));

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   computePairPriority
*
*  PURPOSE:    Compute the pair priority as follows:
*
*              (draft-ietf-mmusic-ice-13.txt)
*              Let O-P be the priority for the candidate provided by 
*              the offerer.
*              Let A-P be the priority for the candidate provided by the 
*              answerer.
*              The priority for a pair is computed as:
*
*              pair priority = 
*                 2^32*MIN(O-P,A-P) + 2*MAX(O-P,A-P) + (O-P>A-P?1:0)
*
*              Where O-P>A-P?1:0 is an expression whose value is 1 if O-P 
*                 is greater than A-P, and 0 otherwise.
*
*  PARAMETERS: 
*              offererPriority   [IN] - offerer priority
*              answererPriority  [IN] - answerer priority
*              pPriority         [OUT] - the computed value
*              
*
*  RETURNS:   none 
*
*  NOTES:     Not the computed priority is a 64-bit value.
*
*****************************************************************************/
static void computePairPriority( const BOS_UINT32 offererPriority, 
                                 const BOS_UINT32 answererPriority, 
                                 ICECTTY_UINT64 *pPairPri )
{
   BOS_ASSERT( NULL != pPairPri );
   pPairPri->high = MIN(offererPriority,answererPriority);
   pPairPri->low = (2 * MAX(offererPriority, answererPriority)) + ((offererPriority>answererPriority)?1:0);
}

/*****************************************************************************
*  FUNCTION:   isGreaterUINT64
*
*  PURPOSE:    Compare 2 UINT64 values. Return BOS_TRUE if the first
*              value is greater than the second value.
*
*  PARAMETERS: 
*              pFirst [IN] - first value
*              pSecond [IN] - second value
*
*  RETURNS:   BOS_TRUE if first value is larger.
*
*  NOTES:     
*
*****************************************************************************/
static BOS_BOOL isGreaterUINT64( const ICECTTY_UINT64 *pFirst, 
                                 const ICECTTY_UINT64 *pSecond )
{
   BOS_ASSERT( NULL != pFirst && NULL != pSecond );

   if ( pFirst->high == pSecond->high )
   {
      if ( pFirst->low > pSecond->low )
      {
         return( BOS_TRUE );
      }
   }
   else if ( pFirst->high > pSecond->high )
   {
      return( BOS_TRUE );
   }

   return( BOS_FALSE );
}

/*****************************************************************************
*  FUNCTION:   sortCheckList
*
*  PURPOSE:    Order the check list based on priority.
*
*  PARAMETERS: 
*              pChkLst [MOD] - check list
*
*  RETURNS:    none.
*
*  NOTES:
*
*****************************************************************************/
static void sortCheckList( ICECTTY_CHKLST *pChkLst )
{
   ICECTTY_CHECK   *pChk;
   ICECTTY_CHECK   *pTmp;
   BOS_BOOL bChanged = BOS_TRUE;

   BOS_ASSERT( NULL != pChkLst );

   if ( pChkLst->num < 2  )
   {
      DLOG((" sortCheckList: no sorting, list has %d entry", pChkLst->num));
      return;
   }

   while ( bChanged )
   {
      bChanged = BOS_FALSE;
      BOS_ASSERT( NULL != pChkLst->pHead && NULL != pChkLst->pHead->pNext );
      for ( pChk = pChkLst->pHead; NULL != pChk->pNext; pChk = pChk->pNext )
      {

         /* Sort list by placing higher priority check pair first */
         if ( isGreaterUINT64(&pChk->pNext->pairPri, &pChk->pairPri ))
         {
            pTmp = pChk->pNext->pNext;
            if ( pChk->pNext->pNext )
            {
               pChk->pNext->pNext->pPrev = pChk;
            }

            if ( pChk->pPrev )
            {
               pChk->pPrev->pNext = pChk->pNext;
            }
            else
            {
               pChkLst->pHead = pChk->pNext;
            }

            pChk->pNext->pPrev = pChk->pPrev;
            pChk->pNext->pNext = pChk;

            pChk->pPrev = pChk->pNext;
            pChk->pNext = pTmp;

            bChanged = BOS_TRUE;
            break;
            
         }
      }
   }
}

/*****************************************************************************
*  FUNCTION:   pruneCheckList
*
*  PURPOSE:    Mark redundant pairs.  
*              A pair is redundant if its local and remote candidates are 
*              identical to the local and remote candidates of a pair 
*              higher up on the priority list.
*              When an entry is marked redundant, it will NOT be used in
*              the connectivity check.
*
*  PARAMETERS: 
*              pChkLst [MOD] - check list
*
*  RETURNS:    none.
*
*  NOTES:
*
*****************************************************************************/
static void pruneCheckList( ICECTTY_CHKLST *pChkLst )
{
   ICECTTY_CHECK *pChk;
   ICECTTY_CHECK *pChk2;
   ICECTTY_CHECK *pNext;

   BOS_ASSERT( NULL != pChkLst );

   DLOG(("pruneCheckList..."));
   if ( NULL == pChkLst->pHead )
   {
      DLOG(("   Check list Empty."));
   }

   if ( ICECNDT_SRVRFLX == pChkLst->pHead->pLclCndt->type )
   {
      /* For each pair where the local candidate is 
       * server reflexive, the server reflexive candidate 
       * MUST be replaced by its base. 
       */
      DLOG(("   First local cndt is server reflexive, replace it by base")); 
      pChkLst->pHead->pLclCndt = pChkLst->pHead->pLclCndt->pBaseCndt;
   }

   for ( pChk = pChkLst->pHead; NULL != pChk; pChk = pChk->pNext ) 
   {
      if ( pChk->bRedundant )
      {
         /* skip over redudant pair */
         continue;
      }

      for ( pChk2=pChk->pNext; NULL != pChk2; pChk2 = pChk2->pNext )
      {
         if ( pChk2->bRedundant )
         {
            /* Skip over already identified redundant pair */
            continue;
         }

         if ( ICECNDT_SRVRFLX == pChk2->pLclCndt->type )
         {
            /* For each pair where the local candidate is 
             * server reflexive, the server reflexive candidate 
             * MUST be replaced by its base. 
             */
            DLOG(("   local cndt is server reflexive, replace it by base")); 
            pChk2->pLclCndt = pChk2->pLclCndt->pBaseCndt;
         }

         if ( isRedundantPair( pChk, pChk2 ) )
         {
            /* Mark pair redundant */
            DLOG(("   **Marking entry redundant")); 
            pChk2->bRedundant = BOS_TRUE;
         }
         else
         {
            /* Initialize check state */
            DLOG(("   Initializing check state to Frozen")); 
            pChk2->state = ICECTTY_FROZEN;
         }
      }
   }

   /* Remove redundant pairs */
   for ( pChk = pChkLst->pHead->pNext; NULL != pChk; ) 
   {
      pNext = pChk->pNext;
      if ( pChk->bRedundant )
      {
         DLOG(("   removing Redundant entry")); 
         pChk->pPrev->pNext = pChk->pNext;
         if (pChk->pNext) 
         {
            pChk->pNext->pPrev = pChk->pPrev;
         }
         pChkLst->num--;
         deleteChk( pChk );
         DLOG(("       new entry count %d", pChkLst->num)); 
      }
      pChk = pNext;
   }
}

/*****************************************************************************
*  FUNCTION:   isRedundantPair
*
*  PURPOSE:    Check if <pChk1> and <pChk2> are redundant pairs.
*
*  PARAMETERS: 
*              pChk1 [IN] - first check pair
*              pChk2 [IN] - second check pair
*
*  RETURNS:    BOS_TRUE if redundant. BOS_FALSE otherwise.
*
*  NOTES:      
*
*****************************************************************************/
static BOS_BOOL isRedundantPair( const ICECTTY_CHECK *pChk1, const ICECTTY_CHECK *pChk2 )
{
   if ( (pChk1->pLclCndt == pChk2->pLclCndt) &&
        (pChk1->pRmtCndt == pChk2->pRmtCndt))
   {
      return( BOS_TRUE );
   }

   if ( pChk1->compId != pChk2->compId )
   {
      return( BOS_FALSE );
   }

   if ( strcmp(pChk1->pLclCndt->foundation, pChk2->pLclCndt->foundation) ||
        strcmp(pChk1->pRmtCndt->foundation, pChk2->pRmtCndt->foundation))
   {
      return( BOS_FALSE );
   }

   if ( memcmp(&pChk1->pLclCndt->transport, &pChk2->pLclCndt->transport, sizeof( ICECNDT_ADDR ) ) ||
        memcmp(&pChk1->pRmtCndt->transport, &pChk2->pRmtCndt->transport, sizeof( ICECNDT_ADDR ) ) )
   {
      return( BOS_FALSE );
   }

   return( BOS_TRUE );
}


/*****************************************************************************
*  FUNCTION:   icectty_PeriodicCheck
*
*  PURPOSE:    Go through checklist and sending STUN connectivity checks.
*              Only one check will be sent when this function is called.
*
*              The function updates the index of the checklist to the 
*              next entry.
*
*  PARAMETERS: 
*              pChkLst           [IN] - Check list
*              pLclCredential    [IN] - Local credential
*              pRmtCredential    [IN] - Remote credential
*              pPriTypePref      [IN] - Type Preference priority
*              pPriLocalPref     [IN] - Local Preference priority
*              nom               [IN] - Nomination type
*              role              [IN] - Agent role
*
*  RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
*
*  NOTES:      The integrity check list is assumed to have been 
*              protected by Mutex.
*
*****************************************************************************/
BOS_BOOL icectty_PeriodicCheck( ICECTTY_CHKLST           *pChkLst, 
                                const ICE_CREDENTIAL     *pLclCredential,
                                const ICE_CREDENTIAL     *pRmtCredential,
                                const ICE_TYPEPREF       *pPriTypePref,
                                const BOS_UINT16         *pPriLocalPref,
                                const ICE_NOM            nom,
                                const ICECTTY_ROLE       role )
{
   ICECTTY_CHECK *pChk;
   BOS_BOOL rc = BOS_TRUE;

   BOS_ASSERT( (NULL != pChkLst) && 
               (NULL != pLclCredential) &&
               (NULL != pRmtCredential) &&
               (NULL != pPriTypePref) &&
               (NULL != pPriLocalPref) );
   
   if ( ICECTTY_CL_RUNNING == pChkLst->state && !pChkLst->bAllChecksSent)
   {
      /* Look for 'check' in WAITING state, 
       *
       * If found, send connectivity check, and put it to IN_PROGRESS state. 
       *
       * If not found, find first entry in FROZEN state, 
       * and send connectivity check, and set it to IN_PROGRESS state. 
       * Also, set all entries of the same compId to the WAITING state
       */
      if ( NULL != (pChk = getFirstCheckByState( pChkLst, ICECTTY_WAITING ) ) )
      {
         DLOG(("icectty_PeriodicCheck: Valid pair found in [%s] state, send Connectivity check", TMAP(icecttyChkStateMap, pChk->state)));

         /* Valid check pair found, send peer STUN check */
         rc = icectty_SendConnectivityCheck( pLclCredential, 
                                             pRmtCredential,
                                             pPriTypePref,
                                             pPriLocalPref,
                                             pChk,
                                             (nom == ICE_AGGRESSIVE)?BOS_TRUE:BOS_FALSE,
                                             role,
                                             &pChk->reqHdl );
         if ( BOS_TRUE == rc )
         {
            pChk->state = ICECTTY_IN_PROGRESS;

            DLOG(("   --- --- setting check state to [%s]", TMAP( icecttyChkStateMap, pChk->state) ));

         }
      }
      else if ( NULL != (pChk = getFirstCheckByState( pChkLst, ICECTTY_FROZEN ) ) )
      {
         /* Valid check pair found, send peer STUN check */
         DLOG(("icectty_PeriodicCheck: Valid pair found in [%s] state, send Connectivity check", TMAP(icecttyChkStateMap, pChk->state)));

         rc = icectty_SendConnectivityCheck( pLclCredential, 
                                             pRmtCredential,
                                             pPriTypePref,
                                             pPriLocalPref,
                                             pChk,
                                             (nom == ICE_AGGRESSIVE)?BOS_TRUE:BOS_FALSE,
                                             role,
                                             &pChk->reqHdl );

         if ( BOS_TRUE == rc )
         {
            pChk->state = ICECTTY_IN_PROGRESS;
            DLOG(("   --- --- setting check state to [%s]", TMAP( icecttyChkStateMap, pChk->state) ));
         }
      }
      else
      {
         /* Set a flag to indicate all Connectivity checks have 
          * been sent
          */
         DLOG(("   --- setting bAllCheckSent flag"));

         pChkLst->bAllChecksSent = BOS_TRUE;
      }
   }

   return( rc ) ;  
}

/*****************************************************************************
*  FUNCTION:   icectty_TriggeredCheck
*
*  PURPOSE:    For the received STUN Binding Request, 
*                 1. If the sender is not already in the <pRmtCndtLst>,
*                    consider it a new Remote candidate, and add it to 
*                    the <pRmtCndtLst>. 
*                 2. If the Check pair does not already exist, 
*                    form a new Check Pair with the Remote Candidate 
*                    and the Local Candidate, where the Local candidate is 
*                    the destination of the request.
*                 3. Initiate Connectivity Check for this pair.
*
*  PARAMETERS: 
*              pLclTransport     [IN] - Local Transport address
*              pRmtTransport     [IN] - Remote Transport address
*              sid               [IN] - Socket id
*              rmtPriority       [IN] - Remote priority
*              pChkLst           [MOD] - Check List
*              pLclCredential    [IN] - Local credential
*              pRmtCredential    [IN] - Remote credential
*              pPriTypePref      [IN] - Type preference priority
*              pPriLocalPref     [IN] - Local preference priority
*              nom               [IN] - Nomination type
*              role              [IN] - Agent role
*              bUseCndt          [IN] - Include USE-CANDIDATE attribute
*              pLclCndtLst       [IN] - Local Candidate list
*              pRmtCndtLst       [MOD] - Remote Candidate list

*  RETURNS:    BOS_TRUE if a Triggered Check has been sent as a result of 
*              matching Check pair found or a new check pair being inserted.
* 
*              BOS_FALSE otherwise.
*
*  NOTES:      The integrity check list is assumed to have been protected 
*              by Mutex.
*
*****************************************************************************/
BOS_BOOL icectty_TriggeredCheck( const STUN_TRANSPORT    *pLclTransport,
                                 const STUN_TRANSPORT    *pRmtTransport, 
                                 const BOS_UINT32        sid,
                                 const BOS_UINT32        rmtPriority,
                                 ICECTTY_CHKLST          *pChkLst, 
                                 const ICE_CREDENTIAL    *pLclCredential,
                                 const ICE_CREDENTIAL    *pRmtCredential,
                                 const ICE_TYPEPREF      *pPriTypePref,
                                 const BOS_UINT16        *pPriLocalPref,
                                 const ICE_NOM           nom,
                                 const ICECTTY_ROLE      role,
                                 const BOS_BOOL          bUseCndt,
                                 ICECNDT_LST             *pLclCndtLst[],
                                 ICECNDT_LST             *pRmtCndtLst[] )
{
   int j;
   ICECNDT *pLclCndt = NULL;
   ICECTTY_CHECK *pChk = NULL;
   ICE_COMP_ID compId = ICE_UNKNOWN;
   BOS_BOOL rc;
   BOS_BOOL bFound = BOS_FALSE;
   BOS_BOOL bSendCheck = BOS_FALSE;

   LOG(("icectty_TriggeredCheck..."));
   BOS_ASSERT( NULL != pLclTransport );
   BOS_ASSERT( NULL != pRmtTransport );
   BOS_ASSERT( NULL != pChkLst );
   BOS_ASSERT( NULL != pLclCredential );
   BOS_ASSERT( NULL != pPriTypePref );
   BOS_ASSERT( NULL != pPriLocalPref );
   BOS_ASSERT( NULL != pRmtCndtLst );

   DLOG(("  Checklist in [%s] state", TMAP(icecttyChkLstStateMap, pChkLst->state)));

   if ( NULL == pRmtCredential )
   {
      DLOG(("  Remote credential UNKNOWN"));
   }

   /* Find matching pair in check list */
   for ( pChk = pChkLst->pHead; NULL != pChk; pChk = pChk->pNext )
   {
      if ( pChk->pLclCndt->transport.bValid && pChk->pRmtCndt->transport.bValid )
      {
         if ( (!memcmp(&pChk->pLclCndt->transport.socAddr, pLclTransport, sizeof(STUN_TRANSPORT ))) )
         {
            /* Mark the local candidate of which the transport matches 
             * the peer Binding Request. 
             * This is needed for forming a new check pair 
             * should the Binding Request does not match any of 
             * the entry in the Checklist.
             */
            pLclCndt = pChk->pLclCndt;

            if (!memcmp(&pChk->pRmtCndt->transport.socAddr, pRmtTransport, sizeof(STUN_TRANSPORT )))
            {
               /* Matching pair found */
               bFound = BOS_TRUE;
               DLOG(("icectty_TriggeredCheck: Matching Pair found"));
               if ( pChk->state <= ICECTTY_IN_PROGRESS )
               {
                  bSendCheck = BOS_TRUE;
               }
               else
               {
                  DLOG((" -- [%s] Agent: Pair in [%s] state, no trigger check sent", TMAP(icecttyRoleMap, role), TMAP( icecttyChkStateMap, pChk->state ) ));
               }

               if ( (ICECTTY_CONTROLLED == role) && bUseCndt )
               {
                  DLOG((" -- Controlled Agent: USE-CANDIDATE present" ));
                  /*
                   * If USE-CANDIDATE is present in the Binding Request,
                   * the controlled Agent will nominate success pair 
                   * immediately. And for check pair is in progress,
                   * it will be nominated when a response is received.
                   */
                  switch( pChk->state )
                  {
                     case ICECTTY_IN_PROGRESS:
                     {
                        /* draft-ietf-mmusic-ice-15 Section 7.2.1.5
                         * If the state of this pair is In-Progress, if its 
                         * check produces a successful result, the resulting 
                         * valid pair has its nominated flag set when the 
                         * response arrives.  This may end ICE processing for 
                         * this media stream when it arrives
                         */
                        DLOG((" -- Controlled Agent: Set UseCndtChk Flag"));
                        pChk->bUseCndtChk = BOS_TRUE;
                     }
                     break;
                     case ICECTTY_SUCCEEDED:
                     {
                        /* draft-ietf-mmusic-ice-15 Section 7.2.1.5
                         * If the state of this pair is succeeded, it means 
                         * that the check generated by this pair produced a 
                         * successful response.  
                         * This would have caused the agent to construct 
                         * a valid pair when that success response was 
                         * received (see Section 7.1.2.2.3).  
                         * The agent now sets the nominated flag in the 
                         * valid pair to true.  
                         * This may end ICE processing for this media stream.
                         */
                        DLOG((" -- Controlled Agent: Set bNominated Flag"));
                        pChk->bNominated = BOS_TRUE;
                     }
                     break;
                     default:
                     {
                        DLOG((" -- Controlled Agent: Pair in [%s] state, no trigger check sent", TMAP( icecttyChkStateMap, pChk->state ) ));
                     }
                  }
               }
               break;
            }
         }
      }
   }
   
   if ( !bFound && (NULL != pLclCndt))
   {
      /* Get Component ID from sid */
      for ( j = 0; j < ICE_COMP_NUM_MAX; j++ )
      {
         if ( pLclCndtLst[j]->sid == sid )
         {
            compId = pLclCndtLst[j]->compId;
            break;
         }
      }

      if ( ICE_UNKNOWN == compId )
      {
         DLOG(("addCheckPair: Unknown CompId, no matching sid"));
         return( BOS_FALSE );
      }

      /* Create and insert check pair into check list */
      rc = addCheckPair( pRmtCndtLst,
                         pChkLst,
                         sid,
                         compId,
                         pLclCndt,
                         pRmtTransport,
                         pPriTypePref,
                         pPriLocalPref,
                         rmtPriority,
                         role,
                         &pChk );

      if ( BOS_TRUE == rc )
      {
         BOS_ASSERT( NULL != pChk );
         if ( NULL != pRmtCredential )
         {
            bSendCheck = BOS_TRUE;
         }
         else
         {
            DLOG(("  added new check pair, NO checks sent"));
         }
      }
      else
      {
         return( BOS_FALSE );
      }
   }

   if ( bSendCheck && (NULL != pChk) )
   {
      DLOG((" --- Sending Triggered Connectivity Check"));
      rc = icectty_SendConnectivityCheck( pLclCredential, 
                                          pRmtCredential,
                                          pPriTypePref,
                                          pPriLocalPref,
                                          pChk,
                                          nom,
                                          role,
                                          &pChk->reqHdl );
      if ( BOS_TRUE == rc )
      {
         pChk->state = ICECTTY_IN_PROGRESS;
         DLOG(("   --- SUCCESS --- setting check state to [%s]", TMAP( icecttyChkStateMap, pChk->state) ));
      }
      else
      {
         DLOG((" --- FAILED"));
      }
   }

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   icectty_LiteUpdateCheckList
*
*  PURPOSE:    This routine is used by the Lite Agent to add candidate to
*              its check list and set the nominated flag to complete the
*              check list.
*
*              (draft-ietf-mmusic-ice-15)
*              7.2.2.  Additional Procedures for Lite Implementations
*
*              If the check that was just received contained a USE-CANDIDATE
*              attribute, the agent constructs a candidate pair whose local
*              candidate is equal to the transport address on which the 
*              request was received, and whose remote candidate is equal 
*              to the source transport address of the request that was 
*              received.  This candidate pair is assigned an arbitrary 
*              priority, and placed into a list of valid
*              candidates pair for that component of that media stream, 
*              called the valid list.  The agent sets the nominated flag 
*              for that pair to true.  ICE processing is considered 
*              complete for a media stream if the valid
*              list contains a candidate pair for each component.
*
*  PARAMETERS: 
*              pLclTransport  [IN] - Local Candidate Transport
*              pRmtTransport  [IN] - Remote Candidate Transport
*              sid            [IN] - Socket Id
*              pLclCndtLst    [MOD]- Local Candidate List
*              pRmtCndtLst    [MOD]- Remote Candidate List
*              pChkLst        [MOD]- Check List
*
*  RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
*
*  NOTES:      The integrity check list is assumed to have been protected 
*              by Mutex.
*
*****************************************************************************/
BOS_BOOL icectty_LiteUpdateCheckList( const STUN_TRANSPORT  *pLclTransport,
                                      const STUN_TRANSPORT  *pRmtTransport, 
                                      const BOS_UINT32      sid,
                                      ICECNDT_LST           *pLclCndtLst[],
                                      ICECNDT_LST           *pRmtCndtLst[],
                                      ICECTTY_CHKLST        *pChkLst )
{
   BOS_BOOL rc = BOS_FALSE;
   ICECTTY_CHECK *pChk = NULL;
   ICECNDT_LST *pCndtLst = NULL;
   ICECNDT *pLclCndt = NULL;
   ICECNDT *pRmtCndt = NULL;
   int i;

   BOS_ASSERT( (NULL != pLclTransport) &&
               (NULL != pRmtTransport) && 
               (NULL != pLclCndtLst) &&
               (NULL != pChkLst));

   if ( pChkLst->num >= ICE_COMP_NUM_MAX )
   {
      DLOG(("icectty_LiteUpdateCheckList: Failed. Check List full"));
      return( BOS_FALSE );
   }
   
   /* Match component by transport address and socket id */
   for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
   {
      if ( NULL != (pCndtLst = pLclCndtLst[i]) )
      {
         if ( (pCndtLst->port == pLclTransport->port) &&
              (pCndtLst->sid == sid ))
         {
            pLclCndt = icecndt_NewCndt();
            pLclCndt->cndtId = pCndtLst->cndtCount;
            pLclCndt->transport.bValid = BOS_TRUE;
            pLclCndt->transport.socAddr = *pLclTransport;
            rc = icecndt_AddCndtToList( pLclCndt, pCndtLst );
            BOS_ASSERT( BOS_TRUE == rc );

            /* Use the first Remote Candidate list as a place holder */
            pRmtCndt = icecndt_NewCndt();
            if ( NULL == pRmtCndtLst[0] )
            {
               pRmtCndtLst[0] = icecndt_NewList();
            }
            pRmtCndt->cndtId = pRmtCndtLst[0]->cndtCount;
            pRmtCndt->transport.bValid = BOS_TRUE;
            pRmtCndt->transport.socAddr = *pRmtTransport;
            rc = icecndt_AddCndtToList( pRmtCndt, pRmtCndtLst[0] );
            BOS_ASSERT( BOS_TRUE == rc );

            break;
         }
      }
   }

   if ( rc && (NULL != pCndtLst))
   {
      BOS_ASSERT( NULL != pLclCndt && NULL != pRmtCndt );

      /* Insert check to the end of Check List */
      pChk = newChk();
      addChkToList( pChk, pChkLst );

      pChk->lclCndtType = ICECNDT_PEERRFLX;
      pChk->pLclCndt = pLclCndt;
      pChk->pRmtCndt = pRmtCndt;
      pChk->state = ICECTTY_FROZEN;
      pChk->sid = sid;
      pChk->compId = pCndtLst->compId;
      pChk->transType = pCndtLst->transType;
      pChk->bRedundant = BOS_FALSE;
      pChk->bValidated = BOS_TRUE;
      pChk->bNominated = BOS_TRUE;
      pChk->bUseCndtChk = BOS_FALSE;
      pChk->bSelected = BOS_FALSE;
   }

   return( rc );
}

/*****************************************************************************
*  FUNCTION:   addCheckPair
*
*  PURPOSE:    Add a Check Pair to the Checklist <pChkLst>. Local candidate
*              and Remote Candidate are added to the pLclCndtLst[] and 
*              pRmtCndtLst[] respectively. Candidate priority is computed
*              using the values in <pPriTypePref> and <pPriLocalPref>.
*
*  PARAMETERS: 
*              pRmtCndtLst[] [MOD] - Remote Candidate list
*              pChkList      [MOD] - Check List
*              sid           [IN]  - Socket Id
*              compId        [IN]  - Component Id
*              pLclCndt      [IN]  - Local Candidate 
*              pRmtTransport [IN]  - Remote transport
*              pPriTypePref  [IN]  - Type Priority Preference
*              pPriLocalPref [IN]  - Local Priority Preference
*              rmtPriority   [IN]  - Remote Priority
*              role          [IN]  - Agent role
*              ppChk         [OUT] - Check Pair
*              
*  RETURNS:    BOS_TRUE if check pair added, BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL addCheckPair( ICECNDT_LST             *pRmtCndtLst[],
                              ICECTTY_CHKLST          *pChkLst,
                              const BOS_UINT32        sid,
                              const ICE_COMP_ID       compId,
                              const ICECNDT           *pLclCndt,
                              const STUN_TRANSPORT    *pRmtTransport, 
                              const ICE_TYPEPREF      *pPriTypePref,
                              const BOS_UINT16        *pPriLocalPref,
                              const BOS_UINT32        rmtPriority,
                              const ICECTTY_ROLE      role,
                              ICECTTY_CHECK           **ppChk )
{
   ICECNDT *pCndt;
   ICECNDT_LST *pCndtLst = NULL;
   int i;
   BOS_BOOL bLocalIsOfferer;
   BOS_BOOL rc;
   
   BOS_ASSERT( (NULL != pRmtCndtLst) &&
               (NULL != pChkLst) &&
               (NULL != pLclCndt) &&
               (NULL != pRmtTransport) &&
               (NULL != pPriTypePref) &&
               (NULL != pPriLocalPref) &&
               (NULL != ppChk) );
   
   (void) pRmtTransport;
   (void) pPriTypePref;
   (void) pPriLocalPref;

   if ( pChkLst->num >= ICE_COMP_NUM_MAX )
   {
      BOS_ASSERT( BOS_FALSE );
      DLOG(("addCheckPair: Failed. Check List full"));
      return( BOS_FALSE );
   }

   pCndt = icecndt_NewCndt();
   BOS_ASSERT( NULL != pCndt );

   for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
   {
      if ( compId == pRmtCndtLst[i]->compId )
      {
         pCndtLst = pRmtCndtLst[i];
         break;
      }
   }

   if ( NULL == pCndtLst )
   {
      icecndt_DeleteCndt( pCndt );
      return( BOS_FALSE );
   }

   pCndt->cndtId = pCndtLst->cndtCount;
   pCndt->type = ICECNDT_PEERRFLX;
   pCndt->priority = rmtPriority;
   pCndt->state = ICECNDT_INUSE;
   pCndt->bInUse = BOS_TRUE;
   pCndt->pBaseCndt = NULL;
   pCndt->pRelated = NULL;
   strncpy( pCndt->foundation, "", ICECNDT_FOUNDATION_LEN_MAX );

   rc = icecndt_AddCndtToList( pCndt, pCndtLst );
   if ( BOS_TRUE != rc )
   {
      DLOG(("addCheckPair: failed to add Cndt to List"));
      icecndt_DeleteCndt( pCndt );
      return( BOS_FALSE );
   }

   *ppChk = newChk();
   addChkToList( *ppChk, pChkLst );

   (*ppChk)->lclCndtType = pLclCndt->type;
   (*ppChk)->pLclCndt = (ICECNDT *)pLclCndt;
   (*ppChk)->pRmtCndt = pCndt;
   (*ppChk)->state = ICECTTY_FROZEN;

   bLocalIsOfferer = (role == ICECTTY_CONTROLLING)?BOS_TRUE:BOS_FALSE;
   if ( bLocalIsOfferer )
   {
      computePairPriority( pLclCndt->priority, pCndt->priority, &((*ppChk)->pairPri) ) ;
   }
   else
   {
      computePairPriority( pCndt->priority, pLclCndt->priority, &((*ppChk)->pairPri) ) ;
   }

   (*ppChk)->sid = sid;
   (*ppChk)->compId = compId;
   (*ppChk)->transType = pCndtLst->transType;
   (*ppChk)->bRedundant = BOS_FALSE;
   
   /* Resort the check list */
   DLOG(("addCheckPair: Resort Check list"));
   sortCheckList( pChkLst );

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   getFirstCheckByState
*
*  PURPOSE:    Get the first check in the check list matching the specified
*              check state.
*
*  PARAMETERS: 
*              pChkLst [IN] - check list
*              state   [IN] - state
*
*  RETURNS:    NULL if not found. 
*
*  NOTES:
*
*****************************************************************************/
static ICECTTY_CHECK *getFirstCheckByState( const ICECTTY_CHKLST *pChkLst,
                                            const ICECTTY_CHECK_STATE state )
{
   ICECTTY_CHECK *pChk;
   BOS_ASSERT( NULL != pChkLst );

   for ( pChk = pChkLst->pHead; NULL != pChk; pChk = pChk->pNext )
   {
      if ( !pChk->bRedundant && (pChk->state == state))
      {
         return( pChk );
      }
   }

   return( NULL );

}

/*****************************************************************************
*  FUNCTION:   icectty_UnfreezeCheckList
*
*  PURPOSE:    Unfreeze checks for local candidate of the specified 
*              [foundation] in the Check list by setting the check state 
*              from FROZEN to WAITING.
*
*              Candidate Check pair foundation is formed by combining
*              the 'Foundation' of the local candidate with the 'Foundation'
*              of the remote candidate.
*
*              The comparison for check pair having the same foundation is 
*              achieved by comparing the Foundation of the local and remote 
*              candidate concurrently.
*
*  PARAMETERS: 
*              pChkLst         [IN] - check list
*              pLclFoundation  [IN] - Local foundation
*              pRmtFoundation  [IN] - Roment foundation
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void icectty_UnfreezeCndtPairs( ICECTTY_CHKLST *pChkLst, 
                                const char *pLclFoundation, 
                                const char *pRmtFoundation )
{
   ICECTTY_CHECK *pChk;

   BOS_ASSERT( NULL != pChkLst );

   for ( pChk = pChkLst->pHead; NULL != pChk; pChk = pChk->pNext )
   {
      if ( (ICECTTY_FROZEN == pChk->state ) && 
           !strcmp( pChk->pLclCndt->foundation, pLclFoundation ) &&
           !strcmp( pChk->pRmtCndt->foundation, pRmtFoundation ) )
      {
         pChk->state = ICECTTY_WAITING;
      }
   }
}

/*****************************************************************************
*  FUNCTION:   icectty_SendConnectivityCheck
*
*  PURPOSE:    Compose and send STUN request for connectivity check.
*
*  PARAMETERS: 
*              pLclCredential [IN] - local credential
*              pRmtCredential [IN] - remote credential
*              pPriTypePref   [IN] - Type preference 
*              pLocalPref     [IN] - Local preference 
*              pChk           [IN] - check pair 
*              bUseCndt       [IN] - flag to include USE-CANDIDATE attribute
*              role           [IN] - agent role
*              pReqHdl        [OUT] - request handle
*
*  RETURNS:    BOS_TRUE if check is sent successfully, BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL icectty_SendConnectivityCheck( const ICE_CREDENTIAL *pLclCredential,
                                        const ICE_CREDENTIAL *pRmtCredential,
                                        const ICE_TYPEPREF   *pPriTypePref,
                                        const BOS_UINT16     *pLocalPref,
                                        const ICECTTY_CHECK  *pChk,
                                        const BOS_BOOL       bUseCndt,
                                        const ICECTTY_ROLE   role,
                                        BOS_UINT32           *pReqHdl )
{
   BOS_UINT8 msgBuf[STUN_MSG_BUFFER_SIZE];
   STUN_HMAC_PWD password;
   STUN_HMAC_PWD *pPwd = &password;
   BOS_UINT16 len;
   char username[ICE_SDP_UFRAG_LEN_MAX*2+2]={0};
   STUN_TRANSPORT  dest;
   STUN_DATA stunData;
   BOS_UINT32 priority;
   STUN_RETRANS reTx;

   (void)role;

   BOS_ASSERT( (NULL != pLclCredential) &&
               (NULL != pRmtCredential) &&
               (NULL != pPriTypePref) &&
               (NULL != pLocalPref) &&
               (NULL != pChk ) &&
               (NULL != pReqHdl ) &&
               (NULL != pChk->pLclCndt) &&
               (NULL != pChk->pRmtCndt));

   BOS_ASSERT( ('\0' != pLclCredential->ufrag[0]) && 
               ('\0' != pLclCredential->pwd[0]) && 
               ('\0' != pRmtCredential->ufrag[0]) && 
               ('\0' != pRmtCredential->pwd[0]) );

   memset( msgBuf, 0, sizeof( msgBuf ) );

   stunSetMsgType( msgBuf, STUN_MSG_BINDING_REQUEST );

   /* Message Integrity needed */
   sprintf( username, 
            "%s:%s", 
            pRmtCredential->ufrag, 
            pLclCredential->ufrag );
   strncpy( (char *)pPwd->val, pRmtCredential->pwd, STUN_MAX_DATA_LEN );
   pPwd->len = (BOS_UINT16)strlen( pRmtCredential->pwd );

   stunData.pData = (BOS_UINT8 *)username;
   stunData.len = (BOS_UINT16)strlen( (char *)username );
   stunSetAttribute( msgBuf, STUN_ATTR_USERNAME, &stunData );

   /* An agent MUST include the PRIORITY attribute in its Binding Request.
    * The attribute MUST be set equal to the priority that would be
    * assigned, based on the algorithm in Section 4.1.2, to a peer
    * reflexive candidate learned from this check.  Such a peer reflexive
    * candidate has a stream ID, component ID and local preference that are
    * equal to the host candidate from which the check is being sent, but a
    * type preference equal to the value associated with peer reflexive
    * candidates.
    */
   priority = icecndt_ComputePriority( pPriTypePref->prflx, 
                                       *pLocalPref,
                                       pChk->compId );
   stunSetAttribute( msgBuf, STUN_ATTR_PRIORITY, &priority );
   if ( bUseCndt )
   {
      /* Include the USE-CANDIDATE attribute */
      stunSetAttribute( msgBuf, STUN_ATTR_USECANDIDATE, NULL );
   }

   stunSetTransactionId( msgBuf, NULL );

   /* encode message to a network packet */
   if ( BOS_TRUE != stunEncode( msgBuf,&len, pPwd, 0) )
   {
      return( BOS_FALSE );
   }

   /* Destination is the transport address of the remote candidate */
   dest = pChk->pRmtCndt->transport.socAddr;

   memset ( &reTx,
            0,
            sizeof ( STUN_RETRANS ) );

   /* queue the packet for sending into network */
   if ( BOS_TRUE != stunSendRequest( pChk->sid, 
                                     &dest, 
                                     msgBuf, 
                                     len, 
                                     pReqHdl, 
                                     icectty_StunEvtHdlr,
                                     &reTx,
                                     pPwd, 
                                     0,
                                     NULL))
   {
      return( BOS_FALSE );
   }

   return( BOS_TRUE );
}

/*****************************************************************************
*
*  FUNCTION:   icectty_StunEvtHdlr
*
*  PURPOSE:    Handler for STUN events generated from Connectivity checks. 
*
*  PARAMETERS:
*          reqHdl    [IN] - The request handle
*          pResData  [IN] - Response Data
*
*  RETURNS:    BOS_BOOL if successful or BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL icectty_StunEvtHdlr( const BOS_UINT32         reqHdl,
                                     const STUN_RES_DATA      *pResData )
{
   BOS_UINT16 msgType;
   BOS_BOOL bNotifySuccess = BOS_FALSE;
   STUN_TRANSPORT  mappedAddr;

   memset( (void*)&mappedAddr, 0, sizeof(STUN_TRANSPORT) );
   BOS_ASSERT( NULL != pResData );
   DLOG(("icectty_StunEvtHdlr: reqHdl [%d]...", (int)reqHdl));

   switch ( pResData->evt )
   {
      case STUN_TIMEOUT:
      case STUN_SYSTEM_ERR:
      {
         /* Notify failure */
         DLOG(("   ------ STUN_TIMEOUT/STUN_SYSTEM_ERR"));
         iceagnt_Notify( ICEAGNT_CTTYCHECK_STATUS, 
                         (BOS_UINT32)reqHdl,
                         (BOS_UINT32)ICEAGNT_CHECK_FAILURE,
                         (void *)NULL );
      }
      break;
      case STUN_RESPONSE:
      {
         DLOG(("   ------ STUN_RESPONSE"));
         msgType = stunGetMsgType( pResData->pMsg );

         switch( msgType )
         {
            case STUN_MSG_BINDING_RESPONSE:
            {
               LOG(("icectty_StunEvtHdlr: Binding Response [%d]", (int)reqHdl));
               /* The agent MUST check that the source IP address and port 
                * of the response equals the destination IP address and port 
                * that the Binding Request was sent to, and that the 
                * destination IP address and port of the response match the 
                * source IP address and port that the Binding Request was 
                * sent from.  If matching fails, notify agent of check 
                * failure check to Failed.
                */
               if ( (pResData->reqSid == pResData->sid) && 
                    !memcmp(pResData->pSrcAddr, pResData->pReqDest, sizeof(STUN_TRANSPORT ) ))
               {
                  DLOG(("      --- src dest socket address matched"));
                  bNotifySuccess = BOS_TRUE;
               }
            }
            break;
            case STUN_MSG_BINDING_ERROR_RESPONSE:
            {
               LOG(("icectty_StunEvtHdlr: Binding Error Response [%d]", (int)reqHdl ));
            }
            break;
            default:
            {
               LOG(("icectty_StunEvtHdlr: unexpected Response [%d]", msgType));
            }
         }

         if ( bNotifySuccess && 
              stunGetAttribute( pResData->pMsg, STUN_ATTR_MAPPED_ADDRESS, (void *)&mappedAddr ))
         {
            LOG(("icectty_StunEvtHdlr: Notify mappedAddr reqHdl [%d]", (int)reqHdl ));
            /* Notify success */
            iceagnt_Notify( ICEAGNT_CTTYCHECK_STATUS, 
                            (BOS_UINT32)reqHdl,
                            (BOS_UINT32)ICEAGNT_CHECK_SUCCESS,
                            (void *)&mappedAddr );
         }
         else
         {
            LOG(("icectty_StunEvtHdlr: Notify Error reqHdl [%d]", (int)reqHdl ));
            /* Notify failure */
            iceagnt_Notify( ICEAGNT_CTTYCHECK_STATUS, 
                            (BOS_UINT32)reqHdl,
                            (BOS_UINT32)ICEAGNT_CHECK_FAILURE,
                            (void *)NULL );
         }
      }
      break;
      default:
      {
         BOS_ASSERT( BOS_FALSE );
      }
   }
   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   icectty_DeleteChkLst
*
*  PURPOSE:    Delete and free resources of a Check List.
*
*  PARAMETERS: 
*              pChkLst     [IN] - Check List
*
*  RETURNS:    none.
*
*  NOTES:
*
*****************************************************************************/
void icectty_DeleteChkLst( ICECTTY_CHKLST *pChkLst )
{
   ICECTTY_CHECK *pChk;

   BOS_ASSERT( NULL != pChkLst );

   /* remove check from the list */
   while ( NULL != (pChk = pChkLst->pHead) )
   {
      pChkLst->pHead = pChk->pNext;
      deleteChk( pChk );
   }
}


/*****************************************************************************
*  FUNCTION:   newChk
*
*  PURPOSE:    Allocate resources and initialize a new Check.
*
*  PARAMETERS:
*              none
*
*  RETURNS:    New Candidate, NULL if failed.
*
*  NOTES:
*
*****************************************************************************/
static ICECTTY_CHECK *newChk( void )
{
   ICECTTY_CHECK *ptr = malloc(sizeof(ICECTTY_CHECK));
   if ( ptr )
   {
      memset( ptr, 0, sizeof(ICECTTY_CHECK) );
   }
   else
   {
      BOS_ASSERT( BOS_FALSE );
   }

   return( ptr );
}

/*****************************************************************************
*  FUNCTION:   deleteChk
*
*  PURPOSE:    Free previously allocated Check.
*
*  PARAMETERS:
*              pChk [IN] - Check to be free
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static void deleteChk( ICECTTY_CHECK *pChk )
{
   if ( NULL != pChk )
   {
      pChk->pPrev = NULL;
      pChk->pNext = NULL;
      free( pChk );
   }
}

/*****************************************************************************
*  FUNCTION:   addChkToList
*
*  PURPOSE:    Add a Check to the Check List.
*
*  PARAMETERS:
*              pChk     [IN] - Check to be added
*              pChkLst  [IN] - Check list 
*
*  RETURNS:    BOS_FALSE if check list is full
*
*  NOTES:
*
*****************************************************************************/
static void addChkToList( ICECTTY_CHECK *pChk, ICECTTY_CHKLST *pChkLst )
{
   ICECTTY_CHECK *pTmp;
   BOS_ASSERT( NULL != pChk && NULL != pChkLst );

   BOS_ASSERT( pChkLst->num < ICECTTY_CNDT_PAIR_MAX );

   if ( NULL != pChkLst->pHead )
   {
      for ( pTmp = pChkLst->pHead; NULL != pTmp->pNext; pTmp = pTmp->pNext )
      {
         ;;
      }

      pTmp->pNext = pChk;
      pChk->pPrev = pTmp;
   }
   else
   {
      pChkLst->pHead = pChk;
      pChk->pPrev = NULL;
   }

   pChk->pNext = NULL;
   pChkLst->num++;
}




#if ICEDBG_LOG
extern void dumpCndt( ICECNDT *pCndt );

void dumpChkLst( BOS_UINT32 agentId )
{
   ICEAGNT *pAgnt;
   ICECTTY_CHKLST *pChkLst;
   ICECTTY_CHECK *pChk;
   int i = 0;
   
   if ( NULL == (pAgnt = iceagnt_GetAgentFromId( agentId )) )
   {
      DLOG(("Agent NOT FOUND"));
      return;
   }

   if ( NULL != (pChkLst = &pAgnt->chkLst) )
   {
      DLOG(("Check list state [%s]", TMAP( icecttyChkLstStateMap, pChkLst->state)));
      DLOG((" count [%d]", pChkLst->num ));
      for ( pChk = pChkLst->pHead; NULL != pChk; pChk = pChk->pNext )
      {
         ICECNDT  *pCndt;
         DLOG(("  --Check[%d]", i++));
         DLOG(("     ----type [%s]", TMAP( icecndtTypeMap, pChk->lclCndtType )));
         if ( NULL != (pCndt = pChk->pLclCndt))
         {
            dumpCndt( pCndt );
         }
         else
         {
            DLOG(("   Local Cndt empty "));
         }
         if ( NULL != (pCndt = pChk->pRmtCndt))
         {
            dumpCndt( pCndt );
         }
         else
         {
            DLOG(("   Remote Cndt empty "));
         }

         DLOG(("     ----state [%s]", TMAP( icecttyChkStateMap, pChk->state )));
         DLOG(("     ----pair priority high[%x] low[%x]", (unsigned int)pChk->pairPri.high, (unsigned int)pChk->pairPri.low));
         DLOG(("     ----sid [%lx]", pChk->sid));
         DLOG(("     ----reqHdl [%d]", (int)pChk->reqHdl));
         DLOG(("     ----compId [%s]", TMAP( iceCompIdMap, pChk->compId)));
         DLOG(("     ----bRedundant [%s]", pChk->bRedundant == 0?"BOS_FALSE":"BOS_TRUE"));
         DLOG(("     ----bValiddated [%s]", pChk->bValidated == 0?"BOS_FALSE":"BOS_TRUE"));
         DLOG(("     ----bNominated [%s]", pChk->bNominated == 0?"BOS_FALSE":"BOS_TRUE"));
      }
   }
   else
   {
      DLOG(("Check list is empty"));
   }
}
#endif
