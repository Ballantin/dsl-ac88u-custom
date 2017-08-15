/**********************************************************************************
** Copyright © 2007 - 2009 Broadcom Corporation
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
**      This file implements the resource base class.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <ceResBase.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */


/***********************************************************************************
**  FUNCTION:   CeResBase::CeResBase
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
CeResBase::CeResBase( CERESTYPE eType ) :
   meResourceGender ( eType ),
   muParentGuid     ( guCeInvalidGuid ),
   muGuid           ( guCeInvalidGuid )
{
   resourceInit();
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceBasicCtlEqual
**
**  PURPOSE:    Checks whether a resource is actually associated with a given
**              basic extension controller.
**
**  PARAMETERS:
**              pBasicExtCtl - the basic extension controller to lookup.
**
**
**  RETURNS:    BOS_FALSE always.  Derive class must implement this.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL CeResBase::resourceBasicCtlEqual(
                        IN MXD_GNS::ISceBasicExtensionControl *pBasicExtCtl )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pBasicExtCtl );

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceShow
**
**  PURPOSE:    Shows (displays via log) information about the current resource.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    None.
**
**  NOTES:
***********************************************************************************/
void CeResBase::resourceShow( void )
{
#if CCTKCFG_APPLICATION_STATUS
   BOS_UINT32 uIx, uJx;

   CCTK_TRACEI4( "ResBase (%p): Type [0x%lX], GUID [0x%lX], Parent [0x%lX]",
                 this,
                 (long unsigned int)meResourceGender,
                 muGuid,
                 muParentGuid );

   for ( uIx = 0 ; uIx < CCTKCFG_RES_FSM_DEPTH ; uIx++ )
   {
      CCTK_TRACEI3( "FsmStack (%d): %s (0x%lX)",
                    (int)uIx,
                    gpcCeFsmName[ meResourceFsm[ uIx ] ],
                    (long unsigned int)meResourceFsm[ uIx ] );
   }

   for ( uIx = 0 ; uIx < CCTKCFG_RES_STATE_DEPTH ; uIx++ )
   {
      CCTK_TRACEI3( "State (%d): %s (0x%lX)",
                    (int)uIx,
                    gpcCeFsmState[ meResourceState[ uIx ] ],
                    (long unsigned int)meResourceState[ uIx ] );

      for ( uJx = 0 ; uJx < CCTKCFG_RES_EVENT_DEPTH ; uJx++ )
      {
         CCTK_TRACEI3( "      Evt (%d): %s (0x%lX)",
                       (int)uJx,
                       gpcCeFsmEvent[
                          static_cast<BOS_UINT32>(
                                meResourceEvent[ uIx ][ uJx ] ) - eCEEVT_INV ],
                             (long unsigned int)meResourceEvent[ uIx ][ uJx ] );
      }
   }
#endif /* CCTKCFG_APPLICATION_STATUS */
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceAddress
**
**  PURPOSE:    Retrieves the address of the derived object that this CeResBase
**              object belongs to.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    mpResource - address of the derived object.
**
**  NOTES:
***********************************************************************************/
void *CeResBase::resourceAddress( void )
{
   return mpResource;
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceSetAddress
**
**  PURPOSE:    Sets the mpResource member.
**
**  PARAMETERS:
**              pAddr - address of the derived object
**
**  RETURNS:    None.
**
**  NOTES:
***********************************************************************************/
void CeResBase::resourceSetAddress( void* pAddr )
{
   mpResource = pAddr;
}

