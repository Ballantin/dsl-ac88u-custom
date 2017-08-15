/**********************************************************************************
** Copyright (c) 2008-2013 Broadcom Corporation
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
**      This file implements the manager for the Call Data Record resources as part
**      of the Call Engine.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceCdr.h>
#include <ceCdrMgr.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
CeCdr      *CeCdrMgr::mpCdrPool[CCTKCFG_MAX_CDR];
BOS_BOOL    CeCdrMgr::bCdrInit;


/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeCdrMgr::CeCdrMgr
**
**  PURPOSE:    Class constructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
CeCdrMgr::CeCdrMgr( void )
{
   BOS_UINT32 uIx;

   bCdrInit = BOS_TRUE;

   /* Create our pool of CDR.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CDR ; uIx++ )
   {
      mpCdrPool[uIx] = BRCM_NEW( CeCdr );
   }
}


/***********************************************************************************
**  FUNCTION:   CeCdrMgr::~CeCdrMgr
**
**  PURPOSE:    Class desctructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
CeCdrMgr::~CeCdrMgr( void )
{
   BOS_UINT32 uIx;

   bCdrInit = BOS_FALSE;

   /* Delete all resources allocated.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CDR ; uIx++ )
   {
      BRCM_DELETE( mpCdrPool[uIx] );
      mpCdrPool[uIx] = NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   CeCdrMgr::allocateCdr
**
**  PURPOSE:    Allocate a CDR of a given type and associate it with the owner.
**
**  PARAMETERS:
**              uCdrOwner  - the GUID for the CDR owner.
**              eType      - the type of CDR to be created.
**
**
**  RETURNS:    Pointer to the CDR, or NULL if none can be allocated.
**
**  NOTES:      This function ensures that there is a free CDR to be allocated
**              for the user, if none is available, it will try to re-use the
**              'oldest' CDR instead.
***********************************************************************************/
CeCdr *CeCdrMgr::allocateCdr( BOS_UINT32 uCdrOwner, CeCdr::CECDRTYPE eType )
{
   CeCdr *pCdr = NULL;
   BOS_UINT32 uIx;
   BOS_UINT32 uCnt = 0;

   if ( bCdrInit )
   {
      /* First pass.
      **
      ** Count the number of CDR associated with the given owner, locate a
      ** possible free CDR candidate to associate.
      */
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_CDR ; uIx++ )
      {
         /* Count.
         */
         if ( mpCdrPool[uIx] &&
              ( mpCdrPool[uIx]->muCdrOwner == uCdrOwner ))
         {
            uCnt++;
         }
         /* Locate.
         */
         if ( mpCdrPool[uIx] &&
              ( mpCdrPool[uIx]->muCdrOwner == guCeInvalidGuid ) &&
              ( pCdr == NULL ))
         {
            pCdr = mpCdrPool[uIx];
         }
      }

      if ( uCnt >= guCeCdrMaxPerUser )
      {
         pCdr = NULL;

         /* Second pass.
         **
         ** Arriving here, it was determined that no CDR is available for this
         ** owner because it already exceeded its allocated quota.
         ** Let's see if we can free up one of the 'oldest'.
         */
         for ( uIx = 0 ; uIx < CCTKCFG_MAX_CDR ; uIx++ )
         {
            if ( mpCdrPool[uIx] &&
                 ( mpCdrPool[uIx]->muCdrOwner == uCdrOwner ))
            {
               /* Ensure the call associated with the CDR is terminated.
               */
               if ( mpCdrPool[uIx]->muCdrRes == guCeInvalidGuid )
               {
                  /* If one is already located, see if this new one is 'older'.
                  ** This is determined by the 'end-time' for this CDR.
                  */
                  if ( pCdr != NULL )
                  {
                     BOS_TIME_MS curTime;
                     BOS_TIME_MS delTime_1;
                     BOS_TIME_MS delTime_2;

                     bosTimeGetMs( &curTime );

                     bosTimeCalcDeltaMs(
                        reinterpret_cast<const BOS_TIME_MS *> (&curTime),
                        reinterpret_cast<const BOS_TIME_MS *> (
                                                     &pCdr->mCdrEndMs),
                        &delTime_1 );

                     bosTimeCalcDeltaMs(
                        reinterpret_cast<const BOS_TIME_MS *> (&curTime),
                        reinterpret_cast<const BOS_TIME_MS *> (
                                                     &mpCdrPool[uIx]->mCdrEndMs),
                        &delTime_2 );

                     if ( delTime_2 > delTime_1 )
                     {
                        pCdr = mpCdrPool[uIx];
                     }
                  }
                  else
                  {
                     pCdr = mpCdrPool[uIx];
                  }
               }
            }
         }
      }
   }


   CCTK_TRACEI3( "Allocate CDR (0x%p) - owner 0x%lX, type %d",
                 pCdr,
                 uCdrOwner,
                 eType );

   /* If a valid CDR was located, assign it for this owner.
   */
   if ( pCdr )
   {
      pCdr->muCdrOwner = uCdrOwner;
      pCdr->meCdrType = eType;
   }

   return pCdr;
}


/***********************************************************************************
**  FUNCTION:   CeCdrMgr::locateCdr
**
**  PURPOSE:    Locates a CDR from the pool.
**
**  PARAMETERS:
**              uCdrOwner  - the GUID for the CDR owner.
**              uCdrRes    - the GUID for the CDR resource.
**
**
**  RETURNS:    Pointer to the CDR, or NULL if none can be matched.
**
**  NOTES:      .
***********************************************************************************/
CeCdr *CeCdrMgr::locateCdr( BOS_UINT32 uCdrOwner, BOS_UINT32 uCdrRes )
{
   CeCdr *pCdr = NULL;
   BOS_UINT32 uIx;

   if ( bCdrInit &&
        ( uCdrOwner != guCeInvalidGuid ) &&
        ( uCdrRes != guCeInvalidGuid ) )
   {
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_CDR ; uIx++ )
      {
         if ( mpCdrPool[uIx] &&
              ( mpCdrPool[uIx]->muCdrOwner == uCdrOwner ) &&
              ( mpCdrPool[uIx]->muCdrRes == uCdrRes ) )
         {
            pCdr = mpCdrPool[uIx];
         }
      }
   }

   CCTK_TRACEI3( "Locate CDR (0x%p) - owner 0x%lX, res 0x%lX",
                 pCdr,
                 uCdrOwner,
                 uCdrRes );

   return pCdr;
}


/***********************************************************************************
**  FUNCTION:   CeCdrMgr::locatePrevCdr
**
**  PURPOSE:    Locates the last CDR associated with a terminated resource.
**
**  PARAMETERS:
**              uCdrOwner  - the GUID for the CDR owner.
**              uCdrRes    - the GUID for the resource associated with the CDR.
**
**
**  RETURNS:    Pointer to the CDR, or NULL if none can be found.
**
**  NOTES:      This function MUST only be used to retrieve CDR information once
**              a call resource is no longer valid, but the CDR may still be
**              available.  This is the case when for example looking at call
**              statistics for a call that once existed.
***********************************************************************************/
CeCdr *CeCdrMgr::locatePrevCdr( BOS_UINT32 uCdrOwner, BOS_UINT32 uCdrRes )
{
   CeCdr *pCdr = NULL;
   BOS_UINT32 uIx;

   if ( bCdrInit &&
        ( uCdrOwner != guCeInvalidGuid ) &&
        ( uCdrRes != guCeInvalidGuid ) )
   {
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_CDR ; uIx++ )
      {
         if ( mpCdrPool[uIx] &&
              ( mpCdrPool[uIx]->muCdrOwner == uCdrOwner ) &&
              ( mpCdrPool[uIx]->muCdrPrevRes == uCdrRes ) )
         {
            pCdr = mpCdrPool[uIx];
         }
      }
   }

   CCTK_TRACEI3( "Locate Prev CDR (0x%p) - owner 0x%lX, res 0x%lX",
                 pCdr,
                 uCdrOwner,
                 uCdrRes );

   return pCdr;
}


/***********************************************************************************
**  FUNCTION:   CeCdrMgr::freeCdr
**
**  PURPOSE:    Free all the CDR from the pool associated with the identified owner.
**
**  PARAMETERS:
**              uCdrOwner  - the GUID for the CDR owner.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      Use of this method is extremely dangerous as it may render some
**              of the system feature useless.  It is however needed in some cases,
**              in particular when a user is re-allocated internally because of
**              some failover condition, under which case we need to clean up all
**              resources then associated with the previous user allocation.
***********************************************************************************/
void CeCdrMgr::freeCdr( BOS_UINT32 uCdrOwner )
{
   BOS_UINT32 uIx;

   if ( bCdrInit &&
        ( uCdrOwner != guCeInvalidGuid ) )
   {
      CCTK_TRACEI1( "Unlink all CDRs - owner 0x%lX",
                    uCdrOwner );

      for ( uIx = 0 ; uIx < CCTKCFG_MAX_CDR ; uIx++ )
      {
         if ( mpCdrPool[uIx] &&
              ( mpCdrPool[uIx]->muCdrOwner == uCdrOwner ) )
         {
            mpCdrPool[uIx]->muCdrOwner = guCeInvalidGuid;
            mpCdrPool[uIx]->muCdrRes = guCeInvalidGuid;
            mpCdrPool[uIx]->muCdrPrevRes = guCeInvalidGuid;
         }
      }
   }
}

