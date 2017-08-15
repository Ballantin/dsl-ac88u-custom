/**********************************************************************************
** Copyright © 2008-2010 Broadcom Corporation
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
**      This file implements the Application Programming Interface for the 
**      Network Address Translation module.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <nsNatKa.h>
#include <nsNatStun.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
NsNatKa   *gpNsNatKa = NULL;
NsNatStun *gpNsNatStun = NULL;


/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   nsNatInit
**
**  PURPOSE:    Module initializer.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void nsNatInit( void )
{
   /* Create keep alive support.
   **
   ** Keep Alive service is also used to hook up the Broadcom Transport
   ** Service layer on top of the stack layer, this servbice is required
   ** for keep alive processing as well as for packet marking (DSCP), so
   ** it needs to run all the time even when the keep alive is not enabled
   ** as a CCTK feature.
   */
   if ( gpNsNatKa == NULL )
   {
      gpNsNatKa = BRCM_NEW( NsNatKa );
      CCTK_TRACEI1( "nsNatInit - Ka: %p", gpNsNatKa );
   }

#if CCTKCFG_KEEP_ALIVE
   /* Create STUN support.
   */
   if ( gpNsNatStun == NULL )
   {
      gpNsNatStun = BRCM_NEW( NsNatStun )( gpNsNatKa );
      CCTK_TRACEI1( "nsNatInit - Stun: %p", gpNsNatStun );
   }
#endif /* CCTKCFG_KEEP_ALIVE */
}


/***********************************************************************************
**  FUNCTION:   nsNatDeInit
**
**  PURPOSE:    Module de-initializer.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void nsNatDeInit( void )
{
   if ( gpNsNatKa != NULL )
   {
      CCTK_TRACEI1( "nsNatDeInit - Ka: %p", gpNsNatKa );

      /* The custom transport service should have been released during stack
      ** shutdown. This is executed only if the stack has never been started up.
      ** NOTE: We access the CSipTransportMgr synchronously as we assume the 
      **       transport thread has not been started yet (i.e. stack not started
      **       yet or it has already been shut down). Therefore, this destructor
      **       is expected to be running in the context of the application's thread.
      */
      if ( MXD_GNS::CSipTransportMgr::GetBrcmTransportSvc() != NULL )
      {
         gpNsNatKa->ReleaseService();
      }

      gpNsNatKa = NULL;
   }

#if CCTKCFG_KEEP_ALIVE
   if ( gpNsNatStun != NULL )
   {
      CCTK_TRACEI1( "nsNatDeInit - Stun: %p", gpNsNatStun );

      BRCM_DELETE( gpNsNatStun );
      gpNsNatStun = NULL;
   }
#endif /* CCTKCFG_KEEP_ALIVE */
}


/***********************************************************************************
**  FUNCTION:   nsNatStartKeepAlive
**
**  PURPOSE:    Start a keep alive for a given resource (user).
**
**  PARAMETERS:
**              uResGuid  - the GUID of the resource to apply this action on
**              pPeerAddr - peer address that the keep alive should be sent to
**
**  RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
**
**  NOTES:      Will return BOS_TRUE if there is no keep alive to be started and
**              the action is still successful.
***********************************************************************************/
BOS_BOOL nsNatStartKeepAlive( BOS_UINT32 uResGuid, MXD_GNS::CSocketAddr *pPeerAddr )
{
   BOS_BOOL bRet = BOS_FALSE;

#if CCTKCFG_KEEP_ALIVE
   if ( gpNsNatKa != NULL )
   {
      bRet = gpNsNatKa->nsNatKaStartA( uResGuid, pPeerAddr );
   }
#else
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pPeerAddr );

   /* Assume success to not prevent application from working
   ** even if we do not start anything.
   */
   bRet = BOS_TRUE;
#endif /* CCTKCFG_KEEP_ALIVE */

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   nsNatStopKeepAlive
**
**  PURPOSE:    Stops a keep alive for a given resource (user).
**
**  PARAMETERS:
**              uResGuid - the GUID of the resource to apply this action on
**
**
**  RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
**
**  NOTES:      Will return BOS_TRUE if there is no keep alive to be stopped and
**              the action is still successful.
***********************************************************************************/
BOS_BOOL nsNatStopKeepAlive( BOS_UINT32 uResGuid )
{
   BOS_BOOL bRet = BOS_FALSE;

#if CCTKCFG_KEEP_ALIVE
   if ( gpNsNatKa != NULL )
   {
      bRet = gpNsNatKa->nsNatKaStopA( uResGuid );
   }
#else
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );

   /* Assume success to not prevent application from working
   ** even if we do not stop anything.
   */
   bRet = BOS_TRUE;
#endif /* CCTKCFG_KEEP_ALIVE */

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   nsNatEndTimer
**
**  PURPOSE:    Terminates all timer under control of the NAT module that may be
**              running at the time of the call.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    CCTKSTATUS based on result of action taken.
**
**  NOTES:      .
***********************************************************************************/
CCTKSTATUS nsNatEndTimer( void )
{
   CCTKSTATUS eStatus = eCCTKSTATUS_NOOP;

#if CCTKCFG_KEEP_ALIVE
   if ( gpNsNatKa != NULL )
   {
      eStatus = gpNsNatKa->nsNatKaEndTimer();
   }
#endif /* CCTKCFG_KEEP_ALIVE */

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   nsNatKaBackOff
**
**  PURPOSE:    Gets the value of the backoff timer for the keep alive recovery
**              process.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**
**  RETURNS:    The timer.  Can be 0 if configured by external means
**
**  NOTES:
***********************************************************************************/
BOS_UINT32 nsNatKaBackOff( BOS_UINT32 uResGuid )
{
#if !CCTKCFG_KEEP_ALIVE
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_KEEP_ALIVE */

#if CCTKCFG_KEEP_ALIVE
   if ( gpNsNatKa != NULL )
   {
      return gpNsNatKa->nsNatKaBackOff ( uResGuid );
   }
   else
#endif /* CCTKCFG_KEEP_ALIVE */
   {
      /* Return 0 by default, should be interpreted as 'do not apply'.
      */
      return 0;
   }
}
