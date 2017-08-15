/**********************************************************************************
** Copyright (c) 2007-2013 Broadcom Corporation
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
**      This file implements the registrar finite state machine class.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <ceFsmReg.h>
#include <ceMain.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* Default backup and recovery delay timer, an offset added to the timer started
** by the SCE layer to ensure the CCTK layer will time out prior to this one.
*/
static const BOS_UINT32 guCeFsmRegBrDelayTmr = 5;

/* Minimum number of re-registration which need to be surpassed before
** failback procedures can be initiated in eCCTKBRBACKTOPRIMMODE_DEREG mode.
*/
static const BOS_UINT32 guCeFsmRegFbackReRegMin = 3;

/* Default failback to primary recovery timer (2 minutes in msecs).
*/
static const BOS_UINT32 guCeFsmRegBk2PrimTmrMs = 120000;

#define CE_FSM__CONTACT_USER_PHONE     "phone"
#define CE_FSM__CONTACT_LENGTH         256
#define CE_FSM__CONTACT_IP_ADDR_LENGTH 64
#define CE_FSM__NONCE_COUNT_INIT       1

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */
static BOS_UINT32 regGetRetryAfter( CeResUser *pResUser );

/* ---- State Machine Definition ------------------------------------------------ */
/* ------------------------------------------------------------------------------ */
/* ---- The specific state machine for this processing is defined below.  The --- */
/* ---- number of states and events processed by this FSM are defined and the --- */
/* ---- FSM is populated. ------------------------------------------------------- */
/* ------------------------------------------------------------------------------ */

static const BOS_UINT32 gFsmNumberOfStates =
      CeFsmBase::eCEFSMSTATE_REG_SET_FINISH -
         CeFsmBase::eCEFSMSTATE_REG_SET_BEGIN - 1;

static const BOS_UINT32 gFsmNumberOfEvents =
      eCEEVT_REG_SET_FINISH - eCEEVT_REG_SET_BEGIN - 1;

static CeFsmBase::CEFSMVOIDFUNCTIONPTR
   gFsmFnc[gFsmNumberOfStates][gFsmNumberOfEvents] =
{
   /* ---- eCEFSMSTATE_REG_IDLE -------------------------------------------- */
   {
        /* ---- eCEEVT_REG_NET_UNREG ------------ */ &CeFsmReg::fsmRegFail,
        /* ---- eCEEVT_REG_NET_TRYING ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_NET_OK --------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_NET_REFRESH ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_NET_FAIL ------------- */ &CeFsmReg::fsmRegFail,
        /* ---- eCEEVT_REG_NET_ENDED ------------ */ &CeFsmReg::fsmRegEnd,
        /* ---- eCEEVT_REG_NET_TIMEOUT ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_NET_CHALLENGE -------- */ &CeFsmReg::fsmAuthAck,
        /* ---- eCEEVT_REG_NET_SRV_FAIL --------- */ &CeFsmReg::fsmAuthSrvFail,
        /* ---- eCEEVT_REG_NET_NEXT_NONCE ------- */ &CeFsmReg::fsmAuthNextNonce,
        /* ---- eCEEVT_REG_NET_OPTIONS ---------- */ &CeFsmReg::fsmOptions,
        /* ---- eCEEVT_REG_NET_REFER ------------ */ &CeFsmReg::fsmRefRej,
        /* ---- eCEEVT_REG_NET_REFENDED --------- */ &CeFsmReg::fsmRefEnded,
        /* ---- eCEEVT_REG_NET_PAU_ID ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_NET_USR_ADD ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_NET_USR_REMOVE ------- */ &CeFsmReg::fsmNoOp,
        /* ---- eCEEVT_REG_USR_REG -------------- */ &CeFsmReg::fsmReg,
        /* ---- eCEEVT_REG_USR_UNREG ------------ */ &CeFsmReg::fsmNoSvc,
        /* ---- eCEEVT_REG_USR_ENDED ------------ */ &CeFsmReg::fsmRegEnd,
        /* ---- eCEEVT_REG_USR_KA_OK ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_USR_KA_FAIL ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_USR_KA_RECOVERY ------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_USR_PUBLISH ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_USR_ADD_OK ----------- */ &CeFsmReg::fsmUsrAdded,
        /* ---- eCEEVT_REG_USR_SVC_ENA ---------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_REG_USR_SVC_DIS ---------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_REG_USR_TARGET_FAIL ------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_USR_MSG -------------- */ &CeFsmReg::fsmMsg,
        /* ---- eCEEVT_REG_USR_INBOUND_TARGET --- */ &CeFsmBase::fsmNoOp,
   },

   /* ---- eCEFSMSTATE_REG_REGING ------------------------------------------ */
   {
        /* ---- eCEEVT_REG_NET_UNREG ------------ */ &CeFsmReg::fsmReg,
        /* ---- eCEEVT_REG_NET_TRYING ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_NET_OK --------------- */ &CeFsmReg::fsmReged,
        /* ---- eCEEVT_REG_NET_REFRESH ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_NET_FAIL ------------- */ &CeFsmReg::fsmRegFail,
        /* ---- eCEEVT_REG_NET_ENDED ------------ */ &CeFsmReg::fsmRegEnd,
        /* ---- eCEEVT_REG_NET_TIMEOUT ---------- */ &CeFsmReg::fsmRegTimeOut,
        /* ---- eCEEVT_REG_NET_CHALLENGE -------- */ &CeFsmReg::fsmAuthAck,
        /* ---- eCEEVT_REG_NET_SRV_FAIL --------- */ &CeFsmReg::fsmAuthSrvFail,
        /* ---- eCEEVT_REG_NET_NEXT_NONCE ------- */ &CeFsmReg::fsmAuthNextNonce,
        /* ---- eCEEVT_REG_NET_OPTIONS ---------- */ &CeFsmReg::fsmOptions,
        /* ---- eCEEVT_REG_NET_REFER ------------ */ &CeFsmReg::fsmRefRej,
        /* ---- eCEEVT_REG_NET_REFENDED --------- */ &CeFsmReg::fsmRefEnded,
        /* ---- eCEEVT_REG_NET_PAU_ID ----------- */ &CeFsmReg::fsmPauId,
        /* ---- eCEEVT_REG_NET_USR_ADD ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_NET_USR_REMOVE ------- */ &CeFsmReg::fsmNoOp,
        /* ---- eCEEVT_REG_USR_REG -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_USR_UNREG ------------ */ &CeFsmReg::fsmUnReg,
        /* ---- eCEEVT_REG_USR_ENDED ------------ */ &CeFsmReg::fsmUnReg,
        /* ---- eCEEVT_REG_USR_KA_OK ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_USR_KA_FAIL ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_USR_KA_RECOVERY ------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_USR_PUBLISH ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_USR_ADD_OK ----------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_REG_USR_SVC_ENA ---------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_REG_USR_SVC_DIS ---------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_REG_USR_TARGET_FAIL ------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_USR_MSG -------------- */ &CeFsmReg::fsmMsg,
        /* ---- eCEEVT_REG_USR_INBOUND_TARGET --- */ &CeFsmBase::fsmNoOp,
   },

   /* ---- eCEFSMSTATE_REG_REGED ------------------------------------------- */
   {
        /* ---- eCEEVT_REG_NET_UNREG ------------ */ &CeFsmReg::fsmReg,
        /* ---- eCEEVT_REG_NET_TRYING ----------- */ &CeFsmReg::fsmTrying,
        /* ---- eCEEVT_REG_NET_OK --------------- */ &CeFsmReg::fsmRereged,
        /* ---- eCEEVT_REG_NET_REFRESH ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_REG_NET_FAIL ------------- */ &CeFsmReg::fsmRegFail,
        /* ---- eCEEVT_REG_NET_ENDED ------------ */ &CeFsmReg::fsmRegEnd,
        /* ---- eCEEVT_REG_NET_TIMEOUT ---------- */ &CeFsmReg::fsmRegTimeOut,
        /* ---- eCEEVT_REG_NET_CHALLENGE -------- */ &CeFsmReg::fsmAuthAck,
        /* ---- eCEEVT_REG_NET_SRV_FAIL --------- */ &CeFsmReg::fsmAuthSrvFail,
        /* ---- eCEEVT_REG_NET_NEXT_NONCE ------- */ &CeFsmReg::fsmAuthNextNonce,
        /* ---- eCEEVT_REG_NET_OPTIONS ---------- */ &CeFsmReg::fsmOptions,
        /* ---- eCEEVT_REG_NET_REFER ------------ */ &CeFsmReg::fsmRefer,
        /* ---- eCEEVT_REG_NET_REFENDED --------- */ &CeFsmReg::fsmRefEnded,
        /* ---- eCEEVT_REG_NET_PAU_ID ----------- */ &CeFsmReg::fsmPauId,
        /* ---- eCEEVT_REG_NET_USR_ADD ---------- */ &CeFsmReg::fsmRemUsrAdd,
        /* ---- eCEEVT_REG_NET_USR_REMOVE ------- */ &CeFsmReg::fsmRemUsrRemove,
        /* ---- eCEEVT_REG_USR_REG -------------- */ &CeFsmReg::fsmReg,
        /* ---- eCEEVT_REG_USR_UNREG ------------ */ &CeFsmReg::fsmUnReg,
        /* ---- eCEEVT_REG_USR_ENDED ------------ */ &CeFsmReg::fsmUnReg,
        /* ---- eCEEVT_REG_USR_KA_OK ------------ */ &CeFsmReg::fsmKaSuccess,
        /* ---- eCEEVT_REG_USR_KA_FAIL ---------- */ &CeFsmReg::fsmKaFail,
        /* ---- eCEEVT_REG_USR_KA_RECOVERY ------ */ &CeFsmReg::fsmKaRecovery,
        /* ---- eCEEVT_REG_USR_PUBLISH ---------- */ &CeFsmReg::fsmPublish,
        /* ---- eCEEVT_REG_USR_ADD_OK ----------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_REG_USR_SVC_ENA ---------- */ &CeFsmReg::fsmSvcEna,
        /* ---- eCEEVT_REG_USR_SVC_DIS ---------- */ &CeFsmReg::fsmSvcDis,
        /* ---- eCEEVT_REG_USR_TARGET_FAIL ------ */ &CeFsmReg::fsmTargetFail,
        /* ---- eCEEVT_REG_USR_MSG -------------- */ &CeFsmReg::fsmMsg,
        /* ---- eCEEVT_REG_USR_INBOUND_TARGET --- */ &CeFsmReg::fsmRegInbound,
   },
};

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRun
**
**  PURPOSE:    Run the registrar state machine.
**
**  PARAMETERS:
**              rRes - reference to the resource this FSM applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmRun( CeResBase &rRes )
{
   CeFsmBase::CEFSMSTATE eState = rRes.resourceState();
   CEEVT eEvent = rRes.resourceEvent();

   /* Verifies that the state and event are valid for this resource.
   */
   if ( (eState > CeFsmBase::eCEFSMSTATE_REG_SET_BEGIN) &&
        (eState < CeFsmBase::eCEFSMSTATE_REG_SET_FINISH) &&
        (eEvent > eCEEVT_REG_SET_BEGIN) &&
        (eEvent < eCEEVT_REG_SET_FINISH) )
   {
      CCTK_TRACEI4( "fsmRun (0x%lX - %s) - state (%s)/event (%s).",
                    rRes.resourceGuid(),
                    gpcCeFsmName[rRes.resourceFsm()],
                    gpcCeFsmState[eState],
                    gpcCeFsmEvent[static_cast<BOS_UINT32>(eEvent) - eCEEVT_INV] );

      /* Map the state and event such that they are in range.
      */
      BOS_UINT32 uStateIx, uEventIx;

      uStateIx = static_cast<BOS_UINT32>(eState)
                           - (CeFsmBase::eCEFSMSTATE_REG_SET_BEGIN + 1);
      uEventIx = static_cast<BOS_UINT32>(eEvent) - (eCEEVT_REG_SET_BEGIN + 1);

      (gFsmFnc[ uStateIx ][ uEventIx ])( rRes );
   }
   else if ( eEvent != eCEEVT_DNP )
   {
      /* 'System' error.
      */
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmReg
**
**  PURPOSE:    Registers the user to the network.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmReg( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;
   mxt_result res;
   BOS_UINT32 uRetryAfter = 0;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      if ( rRes.resourceState() == CeFsmBase::eCEFSMSTATE_REG_IDLE )
      {
         MXD_GNS::ISipContext* pContext = NULL;
         
         res =
            pResUser->resourceNetHdl()->RegistrationContext(
                              reinterpret_cast<MXD_GNS::ISipContext**>(&pContext) );
         if ( MX_RIS_S( res ) && ( pContext != NULL ) )
         {
            BOS_UINT8 uTos = CeMain::getResTos( pResUser->resourceGuid() );
            pContext->SetTosOpaque( reinterpret_cast<mxt_opaque> ( uTos ) );

            pContext->ReleaseIfRef();
            pContext = NULL;
         }
      }

      /* Stop the failback timer if it was running 
      */
      if ( pResUser->resourceTmrFailbkPrimReco() != guCeInvalidGuid )
      {
          tkmTimerStop ( pResUser->resourceTmrFailbkPrimReco() );
          pResUser->resourceSetTmrFailbkPrimReco( guCeInvalidGuid );
      }

      if  ( rRes.resourceEvent() == eCEEVT_REG_NET_UNREG )
      {
         /* User is unregistered from the network side.  Typically, when the 
         ** backup and recovery service is enabled, we proceed to checking
         ** the status in this service and moving onto the next target.  
         ** However, if a Retry-after header was specified with certain
         ** negative response code (such as 408, 500...etc), we need to bypass
         ** the backup and recovery service and let the SIP stack retry the
         ** current target after Retry-after header has expired
         */
         uRetryAfter = regGetRetryAfter( pResUser );
         if ( uRetryAfter )
         {
            CCTKEXPIREINFO expInfo;

            /* Stop the current keep alive if one was running.
            */
            nsNatStopKeepAlive( pResUser->resourceGuid() );

            pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGING );

            /* Update the re-registration time.  The actual re-registration will
            ** be taken care of by the SIP stack
            */
            memset ( static_cast<void *> (&expInfo),
                     0,
                     sizeof ( CCTKEXPIREINFO ) );
            expInfo.tUpdateTime = time(NULL);
            expInfo.uReReg = uRetryAfter;
            pResUser->resourceSetRegExp(expInfo);

            /* Special check for 403 code */
            MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl = NULL;
            mxt_result res;
            const MXD_GNS::CSipPacket *pPacket = NULL;
            void *pData = NULL;
         
            /* see if this timeout is due to 403 Forbidden, if so, the OOS
            ** status should be reported to the application
            */
            res = pResUser->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
            if (  MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
            {
               pSceBasExtCtl->GetLastReceivedPacket( pPacket );
               if ( pPacket &&
                    pPacket->IsResponse() &&
                    ( pPacket->GetStatusLine()->GetCode() == MXD_GNS::uFORBIDDEN ) )
               {
                 CCTKRESPCODE eResponseCode = eCCTKRESPCODE_FORBIDDEN;
                 pData = (void *)&eResponseCode;
                  tkmEvent( pResUser->resourceGuid(),
                            guCeInvalidGuid,       
                            eCCTKEVT_USR_OOS,
                           pData );
               }
               /*Release the packet*/
               if (pPacket)
               {
                  pPacket->Release();
               }
            }
            if (pSceBasExtCtl)
            {
               pSceBasExtCtl->ReleaseIfRef();
               pSceBasExtCtl = NULL;
            }
         }
      }

      /* Continue processing only if a retry-after timer was not processed.
      */
      if ( uRetryAfter == 0 )
      {
         /* If the keep alive recovery timer is running, terminate it now and let the
         ** recovery process re-initiate the registration.
         */
         if ( pResUser->resourceTmrKeepAliveReco() != guCeInvalidGuid )
         {
            /* Short circuit the keep alive recovery timer */
            tkmTimerStop ( pResUser->resourceTmrKeepAliveReco() );

            CeMain::CeKeepAliveRecoveryTimeout ( pResUser->resourceTmrKeepAliveReco(),
                                                 NULL );
         }
         else
         {
            /* Reset the authentication information */
            if ( rRes.resourceEvent() != eCEEVT_REG_USR_REG )
            {
               pResUser->resourceNewRegId();
               pResUser->resourceResetAllChallengeInfo();
               pResUser->resourceIncrReRegCount( BOS_FALSE );
            }
            
            /* 
            ** Check the status of the backup and recovery service associated
            ** with this resource.  Depending on the status we may need to wait
            ** before attempting a new registration.
            */
            pResUser->resourceSetNoSub( BOS_FALSE );
            NSBRSTATUS eStatus = nsBrStatus( pResUser->resourceGuid());
            
            /* 
            ** If there is still a target on the nsbr list to try, or if the list
            ** is empty because this is the very first registration for the user,
            ** attempt registraton now. 
            */
            if ( ( !pResUser->resourceApplyBkoffDelay() && 
                     ( eStatus != eNSBRSTA_FAILED ) ) && 
                  (( eStatus != eNSBRSTA_INVALID ) || 
                     (( eStatus == eNSBRSTA_INVALID ) && 
                      (  rRes.resourceEvent() == eCEEVT_REG_USR_REG ) &&
                      ( pResUser->resourceTmrBkupReco() == guCeInvalidGuid ) )) )
            {
               if ( ( rRes.resourceEvent() == eCEEVT_REG_USR_REG ) &&
                    ( rRes.resourceState() == CeFsmBase::eCEFSMSTATE_REG_REGED ) )
               {
                  const CFGSYS *pSysCfg = CFG_SYS;
                  if ( pSysCfg &&
                       UTIL_BMP_IS_ENABLED_ALL(
                                          pSysCfg->xtl.net.prot.gen.uInterop,
                                          CCTK_INTEROP_REREGNOSUB ) )
                  {
                     pResUser->resourceSetNoSub( BOS_TRUE );
                  }
               }

               /* Reset the expiration timer information */
               pResUser->resourceResetRegExp();

               /* Stop the current keep alive if one was running.
               */
               nsNatStopKeepAlive( pResUser->resourceGuid() );

               /* Stop the failback timer if it was running.
               */
               if ( pResUser->resourceTmrFailbkPrimReco() != guCeInvalidGuid )
               {
                   tkmTimerStop ( pResUser->resourceTmrFailbkPrimReco() );
                   pResUser->resourceSetTmrFailbkPrimReco( guCeInvalidGuid );
               }

               pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGING );

               /* If the registration doesn't need to be delayed, register now.
               */
               res = pResUser->resourceNetHdl()->Register();
               if ( MX_RIS_F( res ) )
               {
                  tkmEvent( pResUser->resourceGuid(),
                            guCeInvalidGuid,       
                            eCCTKEVT_USR_OOS,
                            NULL );
               }
            }
            /* 
            ** Otherwise if all the targets have failed or if registration
            ** cannot succeed because the list is still empty, start
            ** a timer pausing for the next registration attempt.
            */
            else if ( pResUser->resourceTmrBkupReco() == guCeInvalidGuid )
            {
               BOS_UINT32 uTmrBr;
               CCTKEXPIREINFO expInfo;

               CCTKSTATUS eStatus =
                           tkmTimerStart ( &uTmrBr,
                                           /* Convert the timer value in milliseconds.
                                           */
                                           1000 * nsBrBackOff( pResUser->resourceGuid() ),
                                           BOS_FALSE,
                                           NULL,
                                           (TKMMSGCB) &CeMain::CeBackupRecoveryTimeout );

            /* Set the re-registration time.
            */
               memset ( static_cast<void *> (&expInfo),
                        0,
                        sizeof ( CCTKEXPIREINFO ) );
               expInfo.tUpdateTime = time(NULL);
               expInfo.uReReg = nsBrBackOff( pResUser->resourceGuid() );
               pResUser->resourceSetRegExp(expInfo);

               /* Stop the current keep alive if one was running.
               */
               nsNatStopKeepAlive( pResUser->resourceGuid() );

               /* Keep track of the timer handle.
               */
               if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
               {
                  pResUser->resourceSetTmrBkupReco( uTmrBr );
                  pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_IDLE );
               }
            }
            /* Already running the recovery timer, short circuit it now.
            */
            else if ( pResUser->resourceTmrBkupReco() != guCeInvalidGuid )
            {
               tkmTimerStop ( pResUser->resourceTmrBkupReco() );

               CeMain::CeBackupRecoveryTimeout ( pResUser->resourceTmrBkupReco(),
                                                 NULL );
            }
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmTrying
**
**  PURPOSE:    Trying to register the user to the network following a failure
**              detected by the network.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmTrying( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGING );

      pResUser->resourceNewRegId();
      pResUser->resourceResetAllChallengeInfo();
      pResUser->resourceIncrReRegCount( BOS_FALSE );

      /* Stop the current keep alive if one was running.
      */
      nsNatStopKeepAlive( pResUser->resourceGuid() );

      /* Stop the failback timer if it was running.
      */
      if ( pResUser->resourceTmrFailbkPrimReco() != guCeInvalidGuid )
      {
          tkmTimerStop ( pResUser->resourceTmrFailbkPrimReco() );
          pResUser->resourceSetTmrFailbkPrimReco( guCeInvalidGuid );
      }

      /* Report an out of service to the application.
      */
      tkmEvent( pResUser->resourceGuid(),
                guCeInvalidGuid,       
                eCCTKEVT_USR_OOS,
                NULL );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmUnReg
**
**  PURPOSE:    Unregisters the user to the network.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmUnReg( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      if ( !pResUser->resourceIsRemUsr() )
      {
         /* Un-Registration is activated by local user.
         */
         pResUser->resourceUserUnreg( BOS_TRUE );

         pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGING );
         pResUser->resourceNetHdl()->Unregister();
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmReged
**
**  PURPOSE:    The user is registered to the network.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmReged( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      BOS_UINT32 uResGuid;
      BOS_BOOL bOutbound = BOS_FALSE;
      BOS_BOOL bSwapped = BOS_FALSE;
      BOS_BOOL bSkipSub = BOS_FALSE;
      const MXD_GNS::CSipPacket *pPacket = NULL;
      MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl = NULL;
      mxt_result res;
      const CFGSENETGEN *pNetCfg = &( CFG_SYS->xtl.net.gen );
      CeResBase *pNewResUser = NULL;
      MXD_GNS::CSocketAddr pktRxSockAddr;
      MXD_GNS::ESipTransport pktRxTransport = MXD_GNS::eINVALID;
      BOS_BOOL bKeepAliveStarted = BOS_FALSE;

      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );
      /* Apply service in the following order (order matters):
      **
      **    [A] Check the backup and recovery status to determine
      **        if there is any action to be taken.
      **
      **    [B] Check whether the user needs to be swapped per configuration
      **        information.  Note that in this case we will re-create fully
      **        a new user and terminate the life of the current user.
      **
      **    [C] Check whether the user credentials need to be updated
      **        due to registration information (note that in this case the
      **        user is not being swapped, meaning we keep working with the
      **        same primary user).
      **
      **    [D] Apply services (example include message-waiting service,
      **        etc) associated with this user.
      */
      pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGED );

      bSkipSub = pResUser->resourceGetNoSub();
      pResUser->resourceSetNoSub( BOS_FALSE );

      /* If this is the backup and recovery user switching back to the primary
      ** target, we need to make that user the new user and terminate the previous
      ** one.
      */
      if ( pResUser->resourceBkupParent() != guCeInvalidGuid )
      {
         CeResBase *pRes;
         CeResUser *pResPrntUser = NULL;

         /* Find the parent to swap it out and destroy it.
         */
         if ( CeMain::locateRes( pResUser->resourceBkupParent(),
                                 pRes ) )
         {
            BOS_UINT32 uNewGuid = pResUser->resourceGuid();

            CeCdrMgr::freeCdr( pResUser->resourceBkupParent() );

            tkmEvent( pResUser->resourceBkupParent(),
                      guCeInvalidGuid,
                      eCCTKEVT_USR_UPDATE,
                      static_cast<void *> (&uNewGuid) );

            
            /* Remove any restrictions placed on the backup user
            */
            nsBrRemoveRestrictions( uNewGuid );

            if ( pRes && pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
            {
               pResPrntUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );
               pResPrntUser->resourceSetBkupConfig( guCeInvalidGuid );
               pResPrntUser->resourceReplacingGuid( uNewGuid );
               
               /* Stop the parent's failback timer if it was running.
               */
               if ( pResPrntUser->resourceTmrFailbkPrimReco() != guCeInvalidGuid )
               {
                   tkmTimerStop ( pResPrntUser->resourceTmrFailbkPrimReco() );
               }

               /* Reparent any call resources from the old/parent user to the new one, so
               ** that calls will not deleted when the old user is terminated.
               */
               CeMain::reparentCallRes( pResPrntUser->resourceGuid(), uNewGuid,
                              ( pResPrntUser->resourceState() == CeFsmBase::eCEFSMSTATE_REG_REGED )? 
                                BOS_TRUE : BOS_FALSE );

               CeMain::shutDown( CeResBase::eCERESTYPE_MAX,
                              pResPrntUser->resourceGuid() );
                              
               /* Clean up the old user resource silently.
               */
               cfgRemoveUsr ( pResPrntUser->resourceGuid()  );
               pResPrntUser->resourceTerminate();  
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }

         pResUser->resourceSetBkupParent( guCeInvalidGuid );
      }

      /* If we have failed registration to the primary target and we are
      ** configured to switch back to the primary server as soon as possible
      ** following the loss of connectivity to it, indicate that on the next
      ** registration refresh we want to attempt the primary target again.
      */
      const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );

      if ( ( pCfgUsr != NULL ) &&
           ( pCfgUsr->xtl.usr.net.brCfg.eBackToPrimMode != eCCTKBRBACKTOPRIMMODE_DISABLED ) &&
           nsBrFailedPrimary( pResUser->resourceGuid() ) )
      {
         pResUser->resourceSetFailbkPrim( BOS_TRUE );
         
         /* Need to start a timer if failback is in dereg mode.
         */
         if ( pCfgUsr->xtl.usr.net.brCfg.eBackToPrimMode == eCCTKBRBACKTOPRIMMODE_SILENT )
         {
            /* Start a timer to trigger failback to primary recovery slighltly before
            ** reregistration is scheduled to occur. The failback timer is configured 
            ** to occur at reregistration time -  min(reregistration/2, 2 minutes).
            */

            BOS_UINT32 uTmrFailBkGuid;
            BOS_UINT32 uReRegMs = (1000 * pResUser->resourceNetHdl()->GetReRegTime());
                                          

            /* Assign the failback timer
            */
            /* If primary recovery start and end timers are configured, then failback timer
            ** will be calculated as: 
            **      uFailBkTmrMs = rand ([startTime], [endTime]);
            ** Otherwise, it will be calculated as:
            **      uFailBkTmrMs = Reregistration Time - min(ReregistrationTime/2, guCeFsmRegBk2PrimTmrMs)
            */
            BOS_UINT32 uFailBkTmrMs;
            if ( ( pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr ) &&
                 ( pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMaxTmr ) && 
                 ( pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr < 
                   pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMaxTmr ) )
            {
               uFailBkTmrMs = rand() % (pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMaxTmr - 
                              pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr + 1) + 
                              pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr;
            }
            else
            {
               uFailBkTmrMs = uReRegMs - (( (uReRegMs/2) < guCeFsmRegBk2PrimTmrMs )? 
                                                    (uReRegMs/2) : guCeFsmRegBk2PrimTmrMs );
            }

            CCTKSTATUS eStatus =
                        tkmTimerStart ( &uTmrFailBkGuid,
                                        uFailBkTmrMs,
                                        BOS_FALSE,
                                        NULL,
                                        (TKMMSGCB) &CeMain::CeBackToPrimaryRecoveryTimeout );
            
            /* Keep track of the timer handle.
            */
            if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
            {
               pResUser->resourceSetTmrFailbkPrimReco( uTmrFailBkGuid );
            }
         }
      }
      else
      {
         pResUser->resourceSetFailbkPrim( BOS_FALSE );
         
         /* Stop the failback timer if it was running.
         */
         if ( pResUser->resourceTmrFailbkPrimReco() != guCeInvalidGuid )
         {
             tkmTimerStop ( pResUser->resourceTmrFailbkPrimReco() );
             pResUser->resourceSetTmrFailbkPrimReco( guCeInvalidGuid );
         }
      }

      /* Check whether we need to swap the user, meaning create a new user
      ** to provide the services that this user was providing until now.  This
      ** user will be terminated as a consequence of the swap.
      */
      if ( pResUser->resourceSwap4PauId() )
      {
         BOS_UINT32 uNewGuid = ceGuid();
         CFGSTATUS eCfgStatus;

         eCfgStatus =
            cfgDupUsr ( uNewGuid, pResUser->resourceGuid() );
         if ( CFGSTATUS_IS_SUCCESS( eCfgStatus ) )
         {
            CCTKSIPNAME sipName;

            /* No need to set the bUsrNameFromTempId flag as the sip name
            ** is derived from the Pau ID */
            memset ( static_cast<void *> (&sipName),
                     0,
                     sizeof ( CCTKSIPNAME ) ); 
            sipName.pcUsrName =
               (BOS_UINT8 *) reinterpret_cast<const BOS_UINT8 *> (
                     pResUser->resourceGetPauId()->GetSipUri()->
                                             GetUser().GetString().CStr() );
            sipName.pcDisplay =
               (BOS_UINT8 *) reinterpret_cast<const BOS_UINT8 *> (
                     pResUser->resourceGetPauId()->GetDisplayName().
                                             GetString().CStr() );

            tkmSetParm ( uNewGuid,
                         eCCTKPARM_MC_SIPNAME,
                         reinterpret_cast<const void *> ( &sipName ) );

            if ( CeMain::allocateRes( CeResBase::eCERESTYPE_USR,
                                      uNewGuid,
                                      guCeInvalidGuid ) != guCeInvalidGuid )
            {
               CeMain::locateRes( uNewGuid, pNewResUser );

               /* Copy the backup and recovery information from the current
               ** user to the newly created user such that transition is
               ** seamless.
               */
               nsBrTargetCopy( pResUser->resourceGuid(),
                               pNewResUser->resourceGuid() );

               /* Issue the user addition completion event for the UASSP
               ** object to be properly created.
               */
               ceNotify ( pNewResUser->resourceGuid(),
                          eCEEVT_REG_USR_ADD_OK,
                          NULL );
                              
               /* Since the swap succeeded, mark it as such so that we do not
               ** process other actions for this user and instead trigger user
               ** termination.
               */
               bSwapped = BOS_TRUE;
            }
         }
      }

      /* Note: The following must be done as an initial step in this function
      **       since it retrieves some piece of information that are later on
      **       used when creating support for the different services.
      */
      if ( !bSwapped )
      {
         res = pResUser->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
         if (  MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
         {
            pSceBasExtCtl->GetLastReceivedPacket( pPacket );
            if ( pPacket &&
                 pPacket->IsResponse() &&
                 ( pPacket->GetStatusLine()->GetCode() == MXD_GNS::uOK ) )
            {
               /* Check whether we need to take into consideration some further
               ** identities that are to be associated with this user.
               */
               const MXD_GNS::CSipHeader* pPAURI =
                        pPacket->GetHeaderList().Get(
                                    MXD_GNS::eHDR_P_ASSOCIATED_URI, res );
               if ( MX_RIS_S( res ) && ( pPAURI != NULL ) )
               {
                  pResUser->resourceSetAssumedId( pPAURI );

                  /* Extract the P-Associated Identities
                  */
                  MXD_GNS::CBlob hdrValue; 
                  CCTKPAUINFO pauInfo;
                  BOS_UINT32 uBufSize;
                  BOS_UINT32 uTotalLen;
                  BOS_UINT32 uIx = 0;

                  memset(&pauInfo, 0, sizeof (CCTKPAUINFO) );
                  
                  while ( (pPAURI != NULL) && (uIx < CCTKCFG_MAX_ASSUMED_ID) )
               {
                        hdrValue.EraseAll();
                        pPAURI->Serialize( hdrValue );
                        uBufSize = strlen( (char*) &pauInfo.pau );
                        
                        /* Calculate the string length including the next identity
                        ** and any "," seperators.
                        */
                        uTotalLen = uBufSize ? 
                                    ( uBufSize + hdrValue.GetSize() + 1 ) : 
                                    hdrValue.GetSize();

                        if( uTotalLen < CCTK_ASSUMED_ID_LEN )
                  {
                           /* Add the ',' to seperate multiple identities.
                           */
                           if ( uBufSize )
                           {
                              sprintf ( (char *)( &pauInfo.pau[uBufSize] ), "," );
                              uBufSize++ ;
                           }
                           
                           memcpy( static_cast<void *> ( &pauInfo.pau[uBufSize] ), 
                              hdrValue.GetFirstIndexPtr(), hdrValue.GetSize() );
                        }
                        
                        /* Increment.
                        */
                        uIx++;
                        pPAURI = pPAURI->GetNextHeader();
                  }

                  tkmEvent( pResUser->resourceGuid(),
                            guCeInvalidGuid,
                            eCCTKEVT_PAU_UPDATE,
                            reinterpret_cast<void *> (&pauInfo) );
               }

               /* Save the service route and path for 504 restoration procedure
               */
               const MXD_GNS::CSipHeader* pHeader =
                  pPacket->GetHeaderList().Get( MXD_GNS::eHDR_PATH, res );
               if ( MX_RIS_S( res ) && ( pHeader != NULL ) )
               {
                  pResUser->resourceSetRegPath( pHeader->GetPath() );
               }

                     CCTKEXPIREINFO expInfo;
                     memset ( static_cast<void *> (&expInfo),
                              0,
                              sizeof ( CCTKEXPIREINFO ) );
               expInfo.tUpdateTime = time(NULL);
               expInfo.uExp = pResUser->resourceNetHdl()->GetRegExpirationTime();
               expInfo.uReReg = pResUser->resourceNetHdl()->GetReRegTime();
                     expInfo.bPrimary = !nsBrFailedPrimary(pResUser->resourceGuid());
                     pResUser->resourceSetRegExp(expInfo);

               /* Check whether we need to enable keep alive signaling based on our 
               ** local configuration and the information received from the remote
               ** peer.
               */
               if ( pNetCfg && pNetCfg->transport.bKeepAlive )
               {
                  if ( UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                                CCTK_INTEROP_KASKIPOUTBOUND ) )
                  {
                     /* Ignore the checks for Outbound option-tag in 
                     ** Required/Support header and always apply keep alive */
                     bOutbound = BOS_TRUE;
                  }
                  else
                  {
                     const MXD_GNS::CSipHeader *pRequire =
                        pPacket->GetHeaderList().Get( MXD_GNS::eHDR_REQUIRE, res );
                     if ( MX_RIS_S( res ) && ( pRequire != NULL ) )
                     {
                        const MXD_GNS::CString &rstrOutbound =
                                          MXD_GNS::GetOptionTag(
                                             MXD_GNS::eOPTIONTAG_OUTBOUND );
                        while ( pRequire != NULL )
                        {
                           if ( pRequire->GetSupported().GetString().
                                                   CaseInsCmp( rstrOutbound ) == 0 )
                           {
                              bOutbound = BOS_TRUE;
                              pRequire = NULL;
                           }
                           else
                           {
                              pRequire = pRequire->GetNextHeader();
                           }
                        }
                     }

                     const MXD_GNS::CSipHeader *pSupported =
                        pPacket->GetHeaderList().Get( MXD_GNS::eHDR_SUPPORTED, res );
                     if ( MX_RIS_S( res ) && ( pSupported != NULL ) )
                     {
                        const MXD_GNS::CString &rstrOutbound =
                                          MXD_GNS::GetOptionTag(
                                             MXD_GNS::eOPTIONTAG_OUTBOUND );
                        while ( pSupported != NULL )
                        {
                           if ( pSupported->GetSupported().GetString().
                                                   CaseInsCmp( rstrOutbound ) == 0 )
                           {
                              bOutbound = BOS_TRUE;
                              pSupported = NULL;
                           }
                           else
                           {
                              pSupported = pSupported->GetNextHeader();
                           }
                        }
                     }
                  }
               }

               /* Save the information on who we received this packet from to use
               ** it later on durring some of the processing.
               */
               pktRxSockAddr = pPacket->GetPeerAddr();
               pktRxTransport = pPacket->GetTransport();
               
               /* Since the user succesfully registered, the back up and recovery timer 
               ** needs to be reset so that the next registration cycle will start fresh.
               */
               nsBrResetBackOff(pResUser->resourceGuid());
               
               /* Indicate that the user found succesful registration on this registration
               ** cycle.
               */
               pResUser->resourceSuccessfulRegCycle(BOS_TRUE);
               
            }
            /* Finished with the signaling packet.
            */
            if ( pPacket )
            {

               pPacket->Release();
               pPacket = NULL;
            }

            pSceBasExtCtl->ReleaseIfRef();
            pSceBasExtCtl = NULL;
         }
      }

      /* Start keep alive if configured to do so
      */
      if ( !bSwapped && bOutbound )
      {
         bKeepAliveStarted = nsNatStartKeepAlive( pResUser->resourceGuid(), &pktRxSockAddr );
      }

      /* Report 'in-service' state if keep alive is not necessary or if keep
      ** alive is necessary and has been properly started.
      **
      ** Further 'in-service' and 'out-of-service' state determination is driven
      ** by a combination of registration state and applicable keep alive.
      */
      if ( !bSwapped &&
           !bSkipSub &&
           (!bOutbound ||
              ( bOutbound && bKeepAliveStarted ) ) )
      {
         BOS_BOOL bInfo = BOS_FALSE;
         CCTKKAINFO kaInfo;

         memset( &kaInfo, 0, sizeof( kaInfo ) );

         if ( pktRxTransport != MXD_GNS::eINVALID )
         {
            BOS_UINT32 uLastArrayIdx;

            bInfo = BOS_TRUE;

            /* Populate the keep-alive data with the proper information.
            */
            kaInfo.bDatagram =
               ( ( pktRxTransport == MXD_GNS::eUDP ) ?
                                             BOS_TRUE : BOS_FALSE );
            uLastArrayIdx = sizeof( kaInfo.uKaPeer ) - 1;

            strncpy( reinterpret_cast< char * >( kaInfo.uKaPeer ),
                     reinterpret_cast< const char * >(
                              pktRxSockAddr.GetAddress().CStr() ),
                     uLastArrayIdx );
            kaInfo.uKaPeer[uLastArrayIdx] = 0;
         }

         tkmEvent( pResUser->resourceGuid(),
                   guCeInvalidGuid,
                   eCCTKEVT_USR_IS,
                   bInfo ? reinterpret_cast<void *> (&kaInfo) : NULL );
      }

      /* Process the message waiting indication related services.
      */
      pCfgUsr = CFG_USR( pResUser->resourceGuid() );
      if ( !bSwapped &&
           !bSkipSub &&
           pCfgUsr &&
           cfgIsHostPortEnabled( pCfgUsr->xtl.mwi.gen.server.sipHost.hostPort ) )
      {
         BOS_BOOL bSub = BOS_FALSE;
         CCTKSIPUSRHOST mwiCfg;
         MXD_GNS::CNameAddr nameAddr;

         /* Select the first assumed identity that is associated with this user
         ** if any was provided.
         */
         pResUser->resourceGetAssumedId( 0, nameAddr );

         /* Update the message waiting indication configuration information first
         ** if necessary.
         */
         if ( nameAddr.GetSipUri() != NULL )
         {
            memcpy ( static_cast<void *> (&mwiCfg.sipHost),
                     static_cast<const void *> (
                                    &pCfgUsr->xtl.mwi.gen.server.sipHost),
                     sizeof ( CCTKSIPHOST ) );
            mwiCfg.pcUsrName =
               (BOS_UINT8 *) (
                     reinterpret_cast<const BOS_UINT8 *> (
                           nameAddr.GetSipUri()->GetUser().GetString().CStr()));

            tkmSetParm( pResUser->resourceGuid(),
                        eCCTKPARM_MC_MWISVRADDR,
                        static_cast<void *> (&mwiCfg) );
         }

         uResGuid = CeMain::allocateGuid();

         CeResBase *pM = pResUser->resourceGetMwiSub();
         CeResSub *pMwiSub = NULL;

         if ( pM != NULL )
         {
            if ( pM->resourceGender() == CeResBase::eCERESTYPE_SUB )
            {
               pMwiSub = reinterpret_cast<CeResSub *> ( pM->resourceAddress() );
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }

         if ( (pM == NULL) ||
              ((pM != NULL) &&
               (pMwiSub->resourceMwiHdl() == NULL)) )
         {
             if ( (uResGuid != guCeInvalidGuid) &&
                  (CeMain::allocateRes( CeResBase::eCERESTYPE_SUB,
                                        uResGuid,
                                        pResUser->resourceGuid(),
                                        BOS_TRUE,
                                        eCCTKSUBEVT_MWI ) != guCeInvalidGuid) )
            {
               CeResBase *pRes;

               CeMain::locateRes( uResGuid, pRes );
               pResUser->resourceSetMwiSub( pRes );
               pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_IDLE );
               bSub = BOS_TRUE;
            }
         }
         /* Do not create the MWI subscription if already existing.
        */
         else
         {
            bSub = BOS_TRUE;
         }

         /* Initiate subscription to this event.
         */
         if ( bSub )
         {
            ceNotify( uResGuid,
                      eCEEVT_SUB_USR_SUBSCRIBE,
                      NULL );
         }
      }

      /* Process the user-agent profile event related services.
      */
      if ( !bSwapped &&
           !bSkipSub &&
           pCfgUsr &&
           pCfgUsr->xtl.usr.net.uapEvt.uNum )
      {
         BOS_UINT32 uIx;

         for ( uIx = 0 ; uIx < pCfgUsr->xtl.usr.net.uapEvt.uNum ; uIx++ )
         {
            BOS_BOOL bSub = BOS_FALSE;

            uResGuid = CeMain::allocateGuid();

            CeResBase *pU = pResUser->resourceGetUapSub( uIx );
            CeResSub *pResUapSub = NULL;

            if ( pU != NULL )
            {
               if ( pU->resourceGender() == CeResBase::eCERESTYPE_SUB )
               {
                  pResUapSub = reinterpret_cast<CeResSub *> ( pU->resourceAddress() );
               }
               else
               {
                  fsmError( rRes );
                  return;
               }
            }

            if ( (pU == NULL) ||
                 ((pU != NULL) &&
                  (pResUapSub->resourceSubHdl() == NULL)) )
            {
               CeResBase *pRes = NULL;
               CFGSTATUS eCfgStatus = eCFGSTATUS_NORSC;
            
               if ( uResGuid != guCeInvalidGuid )
               {
                  eCfgStatus = cfgAddEvt( uResGuid,
                                          pResUser->resourceGuid() );
               }

               if ( CFGSTATUS_IS_SUCCESS( eCfgStatus ) &&
                    (CeMain::allocateRes( CeResBase::eCERESTYPE_SUB,
                                          uResGuid,
                                          pResUser->resourceGuid(),
                                          BOS_TRUE,
                                          eCCTKSUBEVT_UAP ) != guCeInvalidGuid) )
               {
                  CeMain::locateRes( uResGuid, pRes );
                  pResUser->resourceSetUapSub( uIx, pRes );
                  pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_IDLE );

                  if ( pRes->resourceGender() == CeResBase::eCERESTYPE_SUB )
                  {
                     CeResSub *pResProfSet = reinterpret_cast<CeResSub *> ( pRes->resourceAddress() );

                     pResProfSet->resourceProfSet( uIx );
                     bSub = BOS_TRUE;
                  }
                  else
                  {
                     fsmError( rRes );
                     return;
                  }
               }
               else
               {
                  /* Clean up in case of failure.
                  */
                  CeMain::locateRes( uResGuid, pRes );
                  if ( pRes )
                  {
                     pRes->resourceTerminate();
                  }
                  else
                  {
                     cfgRemoveEvt( uResGuid );
                  }
               }
            }
            /* Do not create the User-Agent Profile subscription if already existing.
            */
            else
            {
               bSub = BOS_TRUE;
            }

            /* Initiate subscription to this event.
            */
            if ( bSub )
            {
               ceNotify( uResGuid,
                         eCEEVT_SUB_USR_SUBSCRIBE,
                         NULL );
            }
         }
      }

      /* If the user was swapped, terminate current user life.
      **
      ** This will trigger an unregistration which is expected to clean
      ** up the network side of things, following un-registration completion,
      ** the 'replacement' user will be activated and the current user will
      ** be completely terminated.
      */
      if ( bSwapped && pNewResUser )
      {
         pResUser->resourceLifeEnding( BOS_TRUE );
         pResUser->resourceReplacingGuid( pNewResUser->resourceGuid() );

         ceNotify ( pResUser->resourceGuid(),
                    eCEEVT_REG_USR_ENDED,
                    NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRereged
**
**  PURPOSE:    Re-registration occured due to the registration time out.
**              Update the expiration time.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmRereged( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;
   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      /* Update the registration timer information
       */
      CCTKEXPIREINFO expInfo;
      memset ( static_cast<void *> (&expInfo),
               0,
               sizeof ( CCTKEXPIREINFO ) );
      expInfo.tUpdateTime = time(NULL);
      expInfo.uExp = pResUser->resourceNetHdl()->GetRegExpirationTime();
      expInfo.uReReg = pResUser->resourceNetHdl()->GetReRegTime();
      expInfo.bPrimary = !nsBrFailedPrimary(pResUser->resourceGuid());
      pResUser->resourceSetRegExp(expInfo);
      
      /* If we have failed registration to the primary target and we are
      ** configured to switch back to the primary server as soon as possible
      ** following the loss of connectivity to it, indicate that we should
      ** try to do this before the next re-registration occurs.
      */
      const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );

      if ( ( pCfgUsr != NULL ) &&
           ( pCfgUsr->xtl.usr.net.brCfg.eBackToPrimMode != eCCTKBRBACKTOPRIMMODE_DISABLED ) &&
           !expInfo.bPrimary )
      {
         pResUser->resourceSetFailbkPrim( BOS_TRUE );
         
         /* If failback in in dereg mode, check to see the re-reg threshold has paassed.
         */
         if ( pCfgUsr->xtl.usr.net.brCfg.eBackToPrimMode == eCCTKBRBACKTOPRIMMODE_DEREG )
         {
            /* Update the re-registration count.
            */
            pResUser->resourceIncrReRegCount( BOS_TRUE );
            
            /* If there are no active calls, and the re-registration count
            ** is greater than the minimum re-registration count,
            ** start off the de-registration procedures.
            */
            if ( !CeMain::checkCallRes( pResUser->resourceGuid() ) &&
                 ( pResUser->resourceGetReRegCount() >= guCeFsmRegFbackReRegMin ) )
            {
               pResUser->resourceUserReplace( BOS_TRUE );
               ceNotify ( pResUser->resourceGuid(),
                          eCEEVT_REG_USR_UNREG,
                          NULL );
            }
         }
         /* If failback is in silent mode, start the failback timer.
         */
         else if ( pCfgUsr->xtl.usr.net.brCfg.eBackToPrimMode == eCCTKBRBACKTOPRIMMODE_SILENT )
         {
            /* Start a timer to trigger failback to primary recovery slighltly before
            ** reregistration is scheduled to occur. The failback timer is configured 
            ** to occur at reregistration time -  min(reregistration/2, 2 minutes).
            */
            BOS_UINT32 uTmrFailBkGuid;
            BOS_UINT32 uReRegMs = (1000*expInfo.uReReg) ;

            /* Assign the failback timer
            */
            /* If primary recovery start and end timers are configured, then we need to 
            ** calculate a new failback timer only if one isn't already running.
            **      uFailBkTmrMs = rand ([startTime], [endTime]);
            ** Otherwise, it will be calculated as:
            **      uFailBkTmrMs = Reregistration Time - min(ReregistrationTime/2, guCeFsmRegBk2PrimTmrMs)
            */
            BOS_UINT32 uFailBkTmrMs;
            if ( ( pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr ) &&
                 ( pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMaxTmr ) && 
                 ( pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr < pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMaxTmr ) )
            {
               /* for configurable primary recovery timers, check if the failback timer is already 
               ** started, if so, it should be left alone
               */
               if ( pResUser->resourceTmrFailbkPrimReco() == guCeInvalidGuid )
               {
                  uFailBkTmrMs = rand() % (pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMaxTmr - pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr + 1) + 
                                 pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr;
               }
               else
               {
                  /* failback timer already started */
                  uFailBkTmrMs = 0;
               }
            }
            else
            {
               uFailBkTmrMs = uReRegMs - (( (uReRegMs/2) < guCeFsmRegBk2PrimTmrMs )? 
                                                    (uReRegMs/2) : guCeFsmRegBk2PrimTmrMs );                                 
            }

            if ( uFailBkTmrMs > 0)
            {
               CCTKSTATUS eStatus =
                           tkmTimerStart ( &uTmrFailBkGuid,
                                           uFailBkTmrMs,
                                           BOS_FALSE,
                                           NULL,
                                           (TKMMSGCB) &CeMain::CeBackToPrimaryRecoveryTimeout );
         
               /* Keep track of the timer handle.
               */
               if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
               {
                  pResUser->resourceSetTmrFailbkPrimReco( uTmrFailBkGuid );
               }
            }
         }
      }
      else
      {
         pResUser->resourceSetFailbkPrim( BOS_FALSE );
         
         /* Stop the failback timer if it was running.
         */
         if ( pResUser->resourceTmrFailbkPrimReco() != guCeInvalidGuid )
         {
             tkmTimerStop ( pResUser->resourceTmrFailbkPrimReco() );
             pResUser->resourceSetTmrFailbkPrimReco( guCeInvalidGuid );
         }
      }


      /* Call an in-service event to update
       * the expire information */
      tkmEvent( pResUser->resourceGuid(),
                guCeInvalidGuid,
                eCCTKEVT_USR_IS,
                NULL );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRegFail
**
**  PURPOSE:    The user failed to registered to the network and we likely
**              need to apply some reconfiguration in order for the user to
**              be able to register.  In such case, we need to inform the
**              application for proper action.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmRegFail( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl = NULL;
      mxt_result res;
      const MXD_GNS::CSipPacket *pPacket = NULL;
      void *pData = NULL;

      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );
      
      /* NOTE: Only the temporary backup user should be in the idle state
      ** when this handler is called!
      */
      if ( rRes.resourceState() == CeFsmBase::eCEFSMSTATE_REG_IDLE )
      {
         /* If this registration attempt was for a temporary backup user which
         ** is used to check whether the primary target is alive again during
         ** backup and recovery process, we know it failed, we need to clean up
         ** the temporarily assigned user and user resources.
         */
         if ( pResUser->resourceBkupParent() != guCeInvalidGuid )
         {
            
            BOS_UINT32 uCfgGuid = pResUser->resourceBkupConfig();
            CeResBase *pRes;
            /* Find the parent.
            */
            CeMain::locateRes( pResUser->resourceBkupParent(),
                               pRes );
            pResUser->resourceSetBkupConfig( guCeInvalidGuid );

            /* Shut down all resources for this user immediately
            */
            CeMain::shutDown( CeResBase::eCERESTYPE_MAX,
                              pResUser->resourceGuid() );
            /* Clean up the current user resource silently.
            */
            cfgRemoveUsr ( pResUser->resourceGuid() );
            pResUser->resourceTerminate();

            /* Re-assign the local user in order to continue with the necessary
            ** processing within this handler.
            */
            if ( pRes && pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
            {
               pResUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );

               pResUser->resourceSetBkupConfig( uCfgGuid );
               
               /* Reset the inbound target info - even if the backup user was
               ** dispatched to contact the primary server (and failed to do
               ** so), at the moment we will not queue/act on any inbound  
               ** registration request that occured in the meantime.
               */
               if ( pResUser->resourceGetInbdTgtAddr() != NULL )
               {
                  pResUser->resourceSetInbdTgtAddr( NULL );
               }
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }
      }
      else
      {
         /* Inform the application of the failure.  Let it decide what to do
         ** next (reconfigure, terminate, etc).
         **
         ** If required, inform the children resources of this failure as it may
         ** impact them as well.
         */
         pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_IDLE );
         pResUser->resourceSetNoSub( BOS_FALSE );

         /* Stop the current keep alive if one was running.
         */
         nsNatStopKeepAlive( pResUser->resourceGuid() );

         /* Stop the failback timer if it was running.
         */
         if ( pResUser->resourceTmrFailbkPrimReco() != guCeInvalidGuid )
         {
             tkmTimerStop ( pResUser->resourceTmrFailbkPrimReco() );
             pResUser->resourceSetTmrFailbkPrimReco( guCeInvalidGuid );
         }

         /*
          ** See if this failure is due to a 403 Forbidden
         */
         res = pResUser->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
         if (  MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
         {
            pSceBasExtCtl->GetLastReceivedPacket( pPacket );
            if ( pPacket &&
                 pPacket->IsResponse() &&
                 ( pPacket->GetStatusLine()->GetCode() == MXD_GNS::uFORBIDDEN ) )
            {
              CCTKRESPCODE eResponseCode = eCCTKRESPCODE_FORBIDDEN;
              pData = (void *)&eResponseCode;

               /* Reset the expiration timer information */
               pResUser->resourceResetRegExp();
            }
            /*Release the packet*/
            if (pPacket)
            {
               pPacket->Release();
            }
         }
         
         if (pSceBasExtCtl)
         {
            pSceBasExtCtl->ReleaseIfRef();
            pSceBasExtCtl = NULL;
         }

         tkmEvent( pResUser->resourceGuid(),
                   guCeInvalidGuid,       
                   eCCTKEVT_USR_OOS,
                  pData );
      }
      
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRegTimeOut
**
**  PURPOSE:    The registration attempt timed out while trying to re-register.
**              This event would come prior to a registration failure from the
**              network layer and it informs us of the nature of the failure.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmRegTimeOut( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;
   BOS_UINT32 uRetryAfter = 0;
   MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl = NULL;
   const MXD_GNS::CSipPacket *pPacket = NULL;
   const CFGUSR *pCfgUsr = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );
      pCfgUsr = CFG_USR( pResUser->resourceGuid() );
      void *pData = NULL;

#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
      /* Clean up the local knowledge of the registration event
      ** subscription because it has been terminated internally
      ** by the SCE already.
      */
      nsBrDetach( pResUser->resourceSubRegGuid() );
      pResUser->resourceSetSubRegGuid ( guCeInvalidGuid );
#endif
   
      pResUser->resourceSetNoSub( BOS_FALSE );

      /* If this registration attempt was for a temporary backup user which
      ** is used to check whether the primary target is alive again during
      ** backup and recovery process, we know it failed, we need to clean up
      ** the temporarily assigned user and user resources - which we will do
      ** in the idle state.
      */
      if ( pResUser->resourceBkupParent() != guCeInvalidGuid )
      {
         pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_IDLE );
      }
      else
      {
         /* User regististration timeout from the network side.
         */
        
         /* Stop the failback timer if it was running.
         */
         if ( pResUser->resourceTmrFailbkPrimReco() != guCeInvalidGuid )
         {
             tkmTimerStop ( pResUser->resourceTmrFailbkPrimReco() );
             pResUser->resourceSetTmrFailbkPrimReco( guCeInvalidGuid );
         }
         
         /* Typically, when the backup and recovery service is enabled, we proceed 
         ** to checking the status in this service and moving onto the next target.
         ** However, if this timeout event was triggered as a result of a silent
         ** termination from a network-initiated unregisteration with certain
         ** negative response code (such as 408, 500..etc), we need to bypass the
         ** the backup and recovery service and let the SIP stack retry the
         ** current target after Retry-after header has expired
         */
         uRetryAfter = regGetRetryAfter( pResUser );
         if ( uRetryAfter )
         {
            CCTKEXPIREINFO expInfo;

            /* Stop the current keep alive if one was running.
            */
            nsNatStopKeepAlive( pResUser->resourceGuid() );

            pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGING );

            /* Update the re-registration time.  The actual re-registration will
            ** be taken care of by the SIP stack
            */
            memset ( static_cast<void *> (&expInfo),
                     0,
                     sizeof ( CCTKEXPIREINFO ) );
            expInfo.tUpdateTime = time(NULL);
            expInfo.uReReg = uRetryAfter;
            pResUser->resourceSetRegExp(expInfo);
         }

         /* Continue processing only if a retry-after timer was not processed.
         */
         if ( uRetryAfter == 0 )
         {
            const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );
            NSBRSTATUS eStatus = eNSBRSTA_INVALID;
            BOS_BOOL bBackoffDelay = BOS_FALSE;

            /* Check if the failover feature is enabled.
            */
            if ( pCfgUsr &&
                 pCfgUsr->xtl.usr.net.brCfg.bFailover )
            {
               /* If back up and recovery feature is enabled, check whether we can do
               ** anything about this situation by selecting a different target to 
               ** attempt.
               */
                                                         
               /* Determine whether the backoff delay needs to be applied if all
               ** targets have failed and either:
               ** a) configured to apply delay before starting a new reg cycle regardless
               **    of the status of the current reg cycle.
               ** b) the current reg cycle has failed (initial registration failed on all targets)
               */
               if ( UTIL_BMP_IS_ENABLED_ALL( 
                     CFG_SYS->xtl.net.prot.gen.sigBehave.uTx, 
                           CCTK_SIGBEHVTX_BKOFFSUCCESSREGCYLCE ) ||
                                 ( pResUser->resourceSuccessfulRegCycle() == BOS_FALSE) )
               {
                  bBackoffDelay = BOS_TRUE;
               }
            

               /* Check to see if there are any other targets left, and  if there aren't 
               ** any, let the backup and recovery module know whether the backoff timer needs  
               ** to be assigned, i.e. if we're here because successful registration was
               ** never found with any of the targets.
               */
               mxt_result res;
               MXD_GNS::CSocketAddr peerAddr;
               if ( pCfgUsr )
               {
                  /* Determine the current target address.
                  */
                  res = pResUser->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
                  if (  MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
                  {
                     pSceBasExtCtl->GetLastReceivedPacket( pPacket );
                     if ( pPacket && pPacket->IsResponse() )
                     {
                        /* See if this is a 305 error response that needs to be
                        ** retried at the top of the target list.
                        */
                        if ( pPacket->GetStatusLine()->GetCode() == MXD_GNS::uUSE_PROXY )
                        {
                           /* Reset the target list
                           */
                           nsBrRestart( pResUser->resourceGuid() );
                           nsBrResetBackOff( pResUser->resourceGuid() );
                        }
                        /* For all other error, select a new target.
                        */
                        else
                        {
                           /* In external list mode, the target has to
                           ** be explicitly set to failed in the backup and
                           ** recovery list.
                           */
                           if ( pCfgUsr->xtl.usr.net.brCfg.eMode == eCCTKBRSVCMODE_EXTLIST )
                           {
                              peerAddr = pPacket->GetPeerAddr();
                              nsBrXtlLstSetFailedTgt( pResUser->resourceGuid(),
                                                      peerAddr );
                           }
                           /* Handle the failure.
                           */
                           eStatus = nsBrBackupOnFail( pResUser->resourceGuid(),
                                                       !pResUser->resourceSuccessfulRegCycle() );
                        }
                     }
                     /* Finished with the signaling packet.
                     */
                     if ( pPacket )
                     {
                        pPacket->Release();
                        pPacket = NULL;
                     }

                     pSceBasExtCtl->ReleaseIfRef();
                     pSceBasExtCtl = NULL;
                  }

                  /* Select the new target for external list mode.
                  */
                  if ( ( eStatus != eNSBRSTA_FAILED ) &&
                       ( pCfgUsr->xtl.usr.net.brCfg.eMode == eCCTKBRSVCMODE_EXTLIST ) )
                  {
                     pResUser->resourceSelectNewObProxy();
                  }

                  /* If back up and recovery is enabled and determined that all the targets
                  ** would lead to a failure, or we are configured to apply backoff time 
                  ** even when all targets havent failed, we we want to delay any retry to 
                  ** register for a while to avoid flooding the system with registrations tries.
                  ** In such case, slow down the UASSP internal retry in consequence.
                  */
                  if ( eStatus == eNSBRSTA_FAILED ||
                       ( eStatus == eNSBRSTA_BACKUP_AVAILABLE && 
                           ( pCfgUsr->xtl.usr.net.brCfg.eBackoffMode == 
                            eCCTKBRBKOFFMODE_ANYFAIL ) ) )
                  {
                     mxt_result res;
                     MXD_GNS::ISceUserConfig *pUsrCfg = pCfgUsr->in.pSceUsrCfg;
                     if ( pUsrCfg != NULL )
                     {
                        MXD_GNS::IUaSspRegistrationConfig *pRegCfg = NULL;
                        res = pUsrCfg->QueryIf( &pRegCfg );
                        if ( MX_RIS_S( res ) && ( pRegCfg != NULL ) )
                        {
                           /* Update the UASSP registration retry delay to avoid useless
                           ** re-attempt. Note that the delay must be slightly longer than
                           ** the CE timer delay since the CE timer must time out first in
                           ** order to clean up the backup and recovery information first.
                           */
                           pRegCfg->SetRegistrationRetryDelay(
                                       nsBrBackOff( pResUser->resourceGuid() ) +
                                       /* Add the fixed offset to make sure we got the
                                       ** right timing.
                                       */
                                       guCeFsmRegBrDelayTmr );

                           pRegCfg->ReleaseIfRef();
                           pRegCfg = NULL;
                        }
                     }
                     
                     /* Indicate that the CE timer delay is to be applied when the user becomes
                     ** unregistered instead of immediately applying failover.
                     */
                     pResUser->resourceApplyBkoffDelay( BOS_TRUE );
                  }
               }
            
               /* 
               ** Indicate that this is a special out-of-service as all targets have failed.
               */
               if( (eStatus == eNSBRSTA_FAILED) || (eStatus == eNSBRSTA_INVALID) )
               {
                  CCTKRESPCODE eResponseCode = eCCTKRESPCODE_ALLREGTARGETSFAILED;
                  pData = (void *)&eResponseCode;
               }
            }
         }
      }

      /* Report an out of service event immediately to the application such
      ** that processing can be halted as it should be.
      **
      ** Further cleanup of currently used resources that are necessary in this
      ** circumstance is going to be originated by the SCE layer.
      */
      tkmEvent( pResUser->resourceGuid(),
                guCeInvalidGuid,       
                eCCTKEVT_USR_OOS,
                pData );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRegEnd
**
**  PURPOSE:    The user registration is terminated.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmRegEnd( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      BOS_BOOL bReging = BOS_FALSE;
      BOS_UINT32 uNewGuid = guCeInvalidGuid;
      BOS_BOOL bLifeEnding = BOS_FALSE;
      BOS_BOOL bReplace = BOS_TRUE;

      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );
      bReplace = pResUser->resourceUserReplace();
      bReging = 
         ( ( ( pResUser->resourceState() == CeFsmBase::eCEFSMSTATE_REG_REGING ) &&
               !pResUser->resourceUserUnreg() ) || 
               ( pResUser->resourceUserUnreg() && bReplace ) );
      /* Registration terminated.
      */
      pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_IDLE );
      pResUser->resourceSetNoSub( BOS_FALSE );

      /* Stop the current keep alive if one was running.
      */
      nsNatStopKeepAlive( pResUser->resourceGuid() );
      
      /* Stop the failback timer if it was running.
      */
      if ( pResUser->resourceTmrFailbkPrimReco() != guCeInvalidGuid )
      {
          tkmTimerStop ( pResUser->resourceTmrFailbkPrimReco() );
          pResUser->resourceSetTmrFailbkPrimReco( guCeInvalidGuid );
      }
      
      /* Free up the CDRs allocated for this user */
      CeCdrMgr::freeCdr( pResUser->resourceGuid() );

      /* The user that replaces the current one */
      uNewGuid = pResUser->resourceReplacingGuid();
      
      /* If this user's life is ending and it needs to be replaced
      ** by another.
      */
      bLifeEnding = pResUser->resourceLifeEnding();

      if ( uNewGuid != guCeInvalidGuid )
      {
         /* Specifically do not report an 'out-of-service' condition if
         ** we are terminating usage of a registrant due to replacement
         ** with another one.  Instead wait for the final status of the
         ** replacement registrant to be known to report the status of the
         ** registration.
         */
      }
      else
      {
         tkmEvent( pResUser->resourceGuid(),
                   guCeInvalidGuid,
                   eCCTKEVT_USR_OOS,
                   NULL );
      }

      if ( pResUser->resourceUserUnreg() )
      {
         pResUser->resourceUserUnreg( BOS_FALSE );
      }

      if ( CeMain::ceReset() )
      {
         /* Finally, clean up the resource.
         */
         pResUser->resourceTerminate();
         /* See if we can issue the 'reset completion' event.
         */
         CeMain::checkFinalRes();
      }
      else if ( bLifeEnding )
      {
         
         /* Update CMGR application.
         */
         tkmEvent( pResUser->resourceGuid(),
                   guCeInvalidGuid,
                   eCCTKEVT_USR_UPDATE,
                   static_cast<void *> (&uNewGuid) );

         /* Trigger new user registration.
         */
         ceNotify ( uNewGuid,
                    eCEEVT_REG_USR_REG,
                    NULL );

         /* Reparent any call resources from the old/temp user to the new one, so
         ** that calls will not be affected when the old user is terminated.
         */
         CeMain::reparentCallRes( pResUser->resourceGuid(), uNewGuid, BOS_FALSE );

         /* Clean up the current user resource silently.
         */
         cfgRemoveUsr ( pResUser->resourceGuid() );
         pResUser->resourceTerminate();
      }
      else if ( bReging )
      {
         NSBRBLK nsBrSavedBlk;
         BOS_BOOL bTrgtFailed = pResUser->resourceTargetFailed();
         BOS_BOOL bRegCycleStatus = pResUser->resourceSuccessfulRegCycle();
         BOS_UINT32 uResGuid = pResUser->resourceGuid();

         if ( bTrgtFailed )
         {
            /* Note: nsBrSave/nsBrPrime *MUST* be invoked within the
            **       same execution context to prevent resource leakage.
            */
            nsBrSave ( uResGuid, &nsBrSavedBlk );
         }
         /* If in external list mode, and the user is being replaced for failback
         ** then we need to reset the outbound proxy as well.
         */
         if ( bReplace )
         {
            const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );
            if ( pCfgUsr && pCfgUsr->xtl.usr.net.brCfg.eMode == eCCTKBRSVCMODE_EXTLIST )
            {
               nsBrRestart( uResGuid );
               pResUser->resourceSelectNewObProxy();
            }
         }

         /* Clean up the current resource, note that this will also reset to
         ** default value all the internal members of the CeResUser object, so
         ** anything needed to be preserved for this user must be done prior
         ** to this call.
         */
         pResUser->resourceTerminate();
         /* Re-create the resource.
         */
         pResUser->resourceAllocated( uResGuid, guCeGuidSeed );
         pResUser->resourcePushFsm( CeFsmBase::eCEFSMTYPE_REG );
         pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_IDLE );
         pResUser->resourceCreate( uResGuid );

         if ( bTrgtFailed )
         {
            /* Note: nsBrSave/nsBrPrime *MUST* be invoked within the
            **       same execution context to prevent resource leakage.
            */
            nsBrPrime ( uResGuid, &nsBrSavedBlk );
            
            /* Set the saved reg cycle failure status, as that information
            ** is needed when determining whether to apply backup and recovery
            ** timers.
            */
            pResUser->resourceSuccessfulRegCycle( bRegCycleStatus );
            
            /* Check whether we want to force reset of the service route
            ** information based on configuration.
            **
            ** TBD.
            */
            bTrgtFailed = BOS_TRUE;
         }

         /* Signal a successful user addition to trigger the configuration
         ** of the UASSP resource.
         */
         ceNotify ( pResUser->resourceGuid(),
                    eCEEVT_REG_USR_ADD_OK,
                    static_cast<void *> (&bTrgtFailed) );

         /* Initiate registration of this newly re-created UASSP registration
         ** resource.
         */
         ceNotify ( pResUser->resourceGuid(),
                    eCEEVT_REG_USR_REG,
                    NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmAuthRej
**
**  PURPOSE:    Rejects an authentication challenge from the authentication
**              service.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmAuthRej( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      MXD_GNS::CString rRealm;
      MXD_GNS::ISceUserAuthentication *pSceUsrAuth;
      const CFGUSR *pCfgUsr;
      mxt_result res;

     pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      pResUser->resourceGetRealm( rRealm );

      pCfgUsr = CFG_USR( pResUser->resourceGuid() );
      if ( pCfgUsr != NULL )
      {
         res = pCfgUsr->in.pSceUsrCfg->QueryIf( &pSceUsrAuth );
         if ( MX_RIS_S( res ) && ( pSceUsrAuth != NULL ) )
         {
            pSceUsrAuth->CancelAuthentication( rRealm, "ALL" );
            pSceUsrAuth->ReleaseIfRef();
            pSceUsrAuth = NULL;
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmAuthAck
**
**  PURPOSE:    Process an authentication challenge from the authentication
**              service.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This function may end up rejecting the authentication challenge
**              if we do not have any credentials to offer.
***********************************************************************************/
void CeFsmReg::fsmAuthAck( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      MXD_GNS::CString rRealm;

      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      pResUser->resourceGetRealm( rRealm );

      /* Lookup credentials to be used for this authentication realm we
      ** are being challenged with.
      **
      ** If we have data that can be used, we will inform the authentication
      ** service of such, otherwise we will reject the authentication all
      ** together.
      */
      if ( cfgAddRealm( pResUser->resourceGuid(),
                        reinterpret_cast<const BOS_UINT8 *> (rRealm.CStr()) )
                 != eCFGSTATUS_SUCCESS )
      {
         fsmAuthRej( rRes );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmAuthSrvFail
**
**  PURPOSE:    The server authentication has failed, this means the server that
**              we are talking to is now considered invalid.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmAuthSrvFail( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      /* Put ourself in a proper state that will allow to deal with
      ** the event most appropriately.
      */
      pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGING );

      /* Terminate the current registration and retry a new one right
      ** away.
      */
      fsmRegEnd( rRes );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmAuthNextNonce
**
**  PURPOSE:    We have received a 'next-nonce' directive as part of the
**              authentication information from the remote end, we need to update
**              our current challenge response with that value.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmAuthNextNonce( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      MXD_GNS::ISceBasicExtensionControl *pSceBasicExtCtl = NULL;
      mxt_result res;
      
      /* Update the challenge information for the current user resource.
      */
      res = pResUser->resourceNetHdl()->QueryIf( &pSceBasicExtCtl );
      if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
      {
         pSceBasicExtCtl->AddChallengeInfo(
                              pResUser->resourceChallengeId() );
         pSceBasicExtCtl->SetChallengeType(
                              pResUser->resourceChallengeId(),
                              static_cast<unsigned int> (
                                          MXD_GNS::uUNAUTHORIZED) );
         pSceBasicExtCtl->ReleaseIfRef();
         pSceBasicExtCtl = NULL;
      }

      if ( UTIL_BMP_IS_ENABLED_ALL(
               CFG_SYS->xtl.net.prot.gen.sigBehave.uTx,
               CCTK_SIGBEHVTX_INSCACHEAUTH ) )
      {
         /* Update the challenge information for all resources associated with
         ** this user.
         */
         res =
            pResUser->resourceNetHdl()->QuerySubscriber(
                              MXD_GNS::IID_ISceBasicExtensionControl,
                              reinterpret_cast<void**>(&pSceBasicExtCtl) );
         if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
         {
            pSceBasicExtCtl->AddChallengeInfo(
                                pResUser->resourceChallengeId() );
            pSceBasicExtCtl->SetChallengeType(
                    pResUser->resourceChallengeId(),
                    static_cast<unsigned int> (
                           MXD_GNS::uPROXY_AUTHENTICATION_REQUIRED) );

            pSceBasicExtCtl->ReleaseIfRef();
            pSceBasicExtCtl = NULL;
         }

         /* Invoke the challenge update on all related components which
         ** share the same user.
         */
         CeMain::updateChallenge ( pResUser->resourceGuid() );              
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmOptions
**
**  PURPOSE:    Process an OPTIONS request for the component.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmOptions( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      MXD_GNS::ISipServerEventControl *pSrvEvtCtl = NULL;

      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );
      pSrvEvtCtl = pResUser->resourceGetSrvCtl();

      /* Simply reply to the message now.
      */
      if ( pSrvEvtCtl )
      {
         pSrvEvtCtl->SendResponse( MXD_GNS::uOK,
                                   NULL,           /* Reason = use default. */
                                   NULL,           /* No extra headers. */ 
                                   NULL  );        /* No body. */
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmKaFail
**
**  PURPOSE:    A keep alive failure has been reported for this user.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmKaFail( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;
   const CFGSYS *pSysCfg = CFG_SYS;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      /* Stop the current keep alive.
      */
      nsNatStopKeepAlive( pResUser->resourceGuid() );


      /* Associate keep-alive status for out-of-service status if
      ** configured to do so.  Note that if this association is disabled,
      ** keep alive will essentially terminate until a new (not a refresh)
      ** registration is initiated to trigger a new keep alive request
      */
      if ( pSysCfg &&
           !UTIL_BMP_IS_ENABLED_ALL( pSysCfg->xtl.net.prot.gen.uInterop,
                                     CCTK_INTEROP_KASKIPOOSREPORT ) )
      {
         /* Defer the termination of the registration until after the
         ** keep alive recovery timer has expired
         */
         if ( pResUser->resourceTmrKeepAliveReco() == guCeInvalidGuid )
         {
            BOS_UINT32 uTmrKa;
            CCTKEXPIREINFO expInfo;
            
            CCTKSTATUS eStatus =
                        tkmTimerStart ( &uTmrKa,
                                        /* Convert the timer value in milliseconds.
                                        */
                                        1000 * nsNatKaBackOff( pResUser->resourceGuid() ),
                                        BOS_FALSE,
                                        NULL,
                                        (TKMMSGCB) &CeMain::CeKeepAliveRecoveryTimeout );

            /* Set the re-registration time.
            */
            memset ( static_cast<void *> (&expInfo),
                     0,
                     sizeof ( CCTKEXPIREINFO ) );
                     
            expInfo.tUpdateTime = time(NULL);
            expInfo.uReReg = nsNatKaBackOff( pResUser->resourceGuid() );
            pResUser->resourceSetRegExp(expInfo);

            /* Keep track of the timer handle.
            */
            if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
            {
               pResUser->resourceSetTmrKeepAliveReco( uTmrKa );
            }
         }
         else
         {
            /* Already running the keep alive recovery timer, short circuit it now.
            */
            tkmTimerStop ( pResUser->resourceTmrKeepAliveReco() );

            CeMain::CeKeepAliveRecoveryTimeout ( pResUser->resourceTmrKeepAliveReco(),
                                                 NULL );
         }

         /* Although the registration termination has been deferred, we will report a 
         ** failure (out-of-service) to the application immediately to indicate
         ** that service is no longer available.  If the user triggers a 
         ** re-registration via external means, the keep alive recovery timer
         ** will be terminated.
         */
         tkmEvent( pResUser->resourceGuid(),
                   guCeInvalidGuid,
                   eCCTKEVT_USR_OOS,
                   NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}

/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmKaRecovery
**
**  PURPOSE:    Keep alive recovery timer has expired and keep alive is to be 
**              initiated again.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/

void CeFsmReg::fsmKaRecovery( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;
   const CFGSYS *pSysCfg = CFG_SYS;

   /* In theory, if the association for keep-alive status with 
   ** in-service/out-of-service status is disabled, we should never get here
   ** as the keep alive timer should not have been started.  We will add
   ** an extra layer of error checking here just to ensure that a new
   ** registration is not attempted.
   */
   if ( pSysCfg &&
        !UTIL_BMP_IS_ENABLED_ALL( pSysCfg->xtl.net.prot.gen.uInterop,
                                  CCTK_INTEROP_KASKIPOOSREPORT ) )
   {

      if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );
         const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );

         /* Mark that the target failed so that when the registration is terminated
         ** and a new one started the target info (the nsbr block) for the user
         ** will be preserved.
         */
         pResUser->resourceTargetFailed( BOS_TRUE );

         /* Stop the failback timer if it was running.
         */
         if ( pResUser->resourceTmrFailbkPrimReco() != guCeInvalidGuid )
         {
             tkmTimerStop ( pResUser->resourceTmrFailbkPrimReco() );
             pResUser->resourceSetTmrFailbkPrimReco( guCeInvalidGuid );
         }
         
         /* If outbound failover mode is to be used we need to reset the reg cycle
         ** info as a failure is assosciated to each initial registration, and the
         ** backoff timer will need to be started if the KA recovery registration
         ** fails.
         */
         if ( pCfgUsr &&
               ( pCfgUsr->xtl.usr.net.brCfg.eBackoffMode == 
                                                      eCCTKBRBKOFFMODE_ANYFAIL ))
         {
            pResUser->resourceSuccessfulRegCycle(BOS_FALSE);
         }

         /* Terminate this registration immediately, then create a new one
         ** and attempt to register with it again.
         */
         pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGING );
         pResUser->resourceNetHdl()->Terminate();
      }
      else
      {
         fsmError( rRes );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmKaSuccess
**
**  PURPOSE:    A keep alive success has been reported for this user.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmKaSuccess( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;
   const CFGSYS *pSysCfg = CFG_SYS;

   /* Associate keep-alive status for in-service status if configured to do
   ** so.  Note that if this association is disabled, the keep alive success
   ** event is ignored and thus there is no need to reset any user resources
   ** associated with keep alive.
   */
   if ( pSysCfg &&
        !UTIL_BMP_IS_ENABLED_ALL( pSysCfg->xtl.net.prot.gen.uInterop,
                                  CCTK_INTEROP_KASKIPOOSREPORT ) )
   {
      if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

         if ( pResUser->resourceNewKaInfo() )
         {
            /* Report a success (in-service) to the application with the
            ** added information.
            */
            tkmEvent( pResUser->resourceGuid(),
                      guCeInvalidGuid,
                      eCCTKEVT_USR_IS,
                      static_cast<void *> (pResUser->resourceKaInfo()) );

            pResUser->resourceResetKaInfo();
         }
      }
      else
      {
         fsmError( rRes );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmPublish
**
**  PURPOSE:    Publish information for a user (registration).
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      Currently we only support publishing VoIP metrics.
***********************************************************************************/
void CeFsmReg::fsmPublish( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      if ( pResUser->resourcePublishMetrics() )
      {
         BOS_UINT32 uResGuid = CeMain::allocateGuid();

         /* Reset metrics publication.
         */
         pResUser->resourceSetPublishMetrics( BOS_FALSE );

         /* Attempt to create proper support for such activity.
         */
         if ( (uResGuid != guCeInvalidGuid) &&
              (CeMain::allocateRes( CeResBase::eCERESTYPE_NOT,
                                    uResGuid,
                                    pResUser->resourceGuid(),
                                    BOS_TRUE,
                                    eCCTKPUBLISH_CALLMETRICS ) != guCeInvalidGuid) )
         {
            CeResBase *pRes;

            CeMain::locateRes( uResGuid, pRes );

            if ( pRes && pRes->resourceGender() == CeResBase::eCERESTYPE_NOT )
            {
               CeResNot *pResNot = reinterpret_cast<CeResNot *> ( pRes->resourceAddress() );

               pResNot->resourceSetCall( pResUser->resourcePubGuid() );
            }
            else
            {
               fsmError( rRes );
               return;
            }
            
            /* If succeeded, issue an event to the new resource state
            ** machine that will take care of the work to be done.
            */
            ceNotify( uResGuid,
                      eCEEVT_REG_USR_PUBLISH,
                      static_cast<void *> (pResUser->resourcePubParty()) );
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmMsg
**
**  PURPOSE:    Sends a message for a user (registration).
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmMsg( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      /* Attempt to create proper support for such activity.
      */
      if ( (pResUser->resourceMsgNotGuid() != guCeInvalidGuid) &&
           (CeMain::allocateRes( CeResBase::eCERESTYPE_NOT,
                                 pResUser->resourceMsgNotGuid(),
                                 pResUser->resourceGuid(),
                                 BOS_TRUE,
                                 eCCTKPUBLISH_PASSTHRUMSG ) != guCeInvalidGuid) )
      {
         CeResBase *pRes;

         CeMain::locateRes( pResUser->resourceMsgNotGuid(), pRes );

         if ( pRes && pRes->resourceGender() == CeResBase::eCERESTYPE_NOT )
         {
            CeResNot *pResNot = reinterpret_cast<CeResNot *> ( pRes->resourceAddress() );

            pResNot->resourceSetBody( pResUser->resourceMsgData(),
                                      pResUser->resourceMsgSize() );
         }
         else
         {
            fsmError( rRes );
            return;
         }

         /* If succeeded, issue an event to the new resource state
         ** machine that will take care of the work to be done.
         */
         ceNotify( pResUser->resourceMsgNotGuid(),
                   eCEEVT_REG_USR_MSG,
                   static_cast<void *> (pResUser->resourceMsgParty()) );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRefEnded
**
**  PURPOSE:    A referrer resource associated with this user has been terminated.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmRefEnded( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      pResUser->resourceReferEnd();
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRefRej
**
**  PURPOSE:    An out of dialog refer (transfer) has been received but is being
**              rejected.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmRefRej( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      if ( pResUser->resourceGetReferrer() )
      {
         /* Reject this attempt to transfer a call.
         */
         pResUser->resourceGetReferrer()->RejectRefer();

         /* Clean up the refer information.
         */
         pResUser->resourceReferClean();
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRefer
**
**  PURPOSE:    An out of dialog refer (transfer) has been received and needs to
**              be processed.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmRefer( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;
   BOS_BOOL bRej = BOS_TRUE;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      if ( pResUser->resourceGetReferrer() )
      {
         if ( pResUser->resourceGetTgtDlg() )
         {
            /* Need to locate the call to be associated with this transfer.
            */
            MXD_GNS::IUaSspCall *pCall = NULL;

            pResUser->resourceGetTgtDlg()->QueryIf( &pCall );
            if ( pCall != NULL )
            {
               CeResBase *pRes = NULL;

               if ( CeMain::locateCallRes( pCall, pRes ) &&
                    ( pRes != NULL ) )
               {
                  /* We found the call to which this transfer applies.  Pass
                  ** on the ownership of the transfer to that call.
                  */
                  bRej = BOS_FALSE;

                  if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
                  {
                     CeResCall *pResCall =
                        reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );

                     pResCall->resourceSetReferrer( pResUser->resourceGetReferrer() );
                     pResCall->resourceSetSvc( *(pResUser->resourceGetReferTo()) );

                     pResUser->resourceUnsetReferrer( pResUser->resourceGetReferrer() );

                     ceNotify ( pRes->resourceGuid(),
                                eCEEVT_REG_NET_REFER,
                                NULL );
                  }
                  else
                  {
                     fsmError( rRes );
                     return;
                  }
               }

               pCall->ReleaseIfRef();
               pCall = NULL;
            }
         }
      }

      /* Reject the transfer.
      */
      if ( bRej )
      {
         fsmRefRej( rRes );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmPauId
**
**  PURPOSE:    An new identity for this user needs to be applied via the
**              P-Aassociated-URI content.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmPauId( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );

      if ( pCfgUsr && pResUser->resourceGetPauId() )
      {
         /* The URI of the P-Associated-URI should always be a SIP URI in this
         ** case because it is the first one available.  In such case, we update
         ** our local contact information with this new data for subsequent use.
         */
         if ( pResUser->resourceGetPauId()->GetSipUri() )
         {
            CCTKNAICFG naiCfg;
            CCTKSIPUSRHOST usrHost;

            /* Check whether to swap the user on successful registration
            ** indication.
            */
            if ( CFG_SYS->xtl.net.prot.gen.pau.bPauUsrSwap )
            {
               if ( strcmp(
                        reinterpret_cast<const char *> (
                              pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                        reinterpret_cast<const char *> (
                              pResUser->resourceGetPauId()->GetSipUri()->
                                             GetUser().GetString().CStr()) ) )
               {
                  pResUser->resourceSwap4PauId( BOS_TRUE );
               }
            }

            /* Update the 'Contact' header information if necessary.
            */
            if ( CFG_SYS->xtl.net.prot.gen.pau.bPauUpdCtc )
            {
               CCTKSIPNAME sipName;
               CCTKCONTACT contact;

               /* Set the user name, this is the identification of the
               ** local user, which is overwritten with the identity that
               ** we want to assume.  Hence, there is no need to set the
               ** bUsrNameFromTempId flag.
               */
               memset ( static_cast<void *> (&sipName),
                        0,
                        sizeof ( CCTKSIPNAME ) ); 
               sipName.pcUsrName =
                  (BOS_UINT8 *) reinterpret_cast<const BOS_UINT8 *> (
                        pResUser->resourceGetPauId()->GetSipUri()->
                                                GetUser().GetString().CStr() );
               sipName.pcDisplay =
                  (BOS_UINT8 *) reinterpret_cast<const BOS_UINT8 *> (
                        pResUser->resourceGetPauId()->GetDisplayName().
                                                GetString().CStr() );

               tkmSetParm ( pResUser->resourceGuid(),
                            eCCTKPARM_MC_SIPNAME,
                            reinterpret_cast<const void *> ( &sipName ) );

               /* Add necessary user parameter(s) to the contact.
               */
               memset ( static_cast<void *> (&contact),
                        0,
                        sizeof ( CCTKCONTACT ) ); 

               contact.hdrOverride.pcAddr = NULL;
               contact.hdrOverride.pcHdrParm = NULL;
               contact.pcUsrParm =
                     (BOS_UINT8 *)( &CE_FSM__CONTACT_USER_PHONE );

               tkmSetParm ( pResUser->resourceGuid(),
                            eCCTKPARM_EC_USRCONTACT,
                            reinterpret_cast<const void *> ( &contact ) );
            }

            /* Update the 'P-Preferred-Identity' information to advertize our
            ** preferred information which is basically the one that the server
            ** asks us to use through the P-Associated-URI.
            */
            memcpy ( static_cast<void *> (&naiCfg),
                     reinterpret_cast<const void *>
                        (&pCfgUsr->xtl.dft.call.basic.adv.nai),
                     sizeof ( CCTKNAICFG ) );

            memcpy ( static_cast<void *> (&usrHost.sipHost),
                     reinterpret_cast<const void *>
                        (&pCfgUsr->xtl.reg.gen.registrar),
                     sizeof ( CCTKSIPHOST ) );
            usrHost.pcUsrName =
               (BOS_UINT8 *) reinterpret_cast<const BOS_UINT8 *> (
                        pResUser->resourceGetPauId()->GetSipUri()->
                                                GetUser().GetString().CStr() );  

            naiCfg.cli.pPreferredId = &usrHost;
            tkmSetParm ( pResUser->resourceGuid(),
                         eCCTKPARM_EC_CALLNAI,
                         reinterpret_cast<const void *> ( &naiCfg ) );
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRemUsrUpdate
**
**  PURPOSE:    A remote user aor is updated.  Remote user is
**              third party user which uses the CCTK as a proxy.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmRemUsrUpdate( CeResBase &rRes )
{
#if CCTKCFG_REMOTE_USER_CONTROL
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      /* Mark this user as a remote user such that we can apply proper call and
      ** services processing on it later on.
      */
      pResUser->resourceSetRemUsr ( BOS_TRUE );

      /* Inform the upper layer application of the event.
      */
      const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );
      if ( pCfgUsr )
      {
         CCTKREMUSRINFO remUsrInfo;

         memset ( static_cast<void *> (&remUsrInfo),
                  0,
                  sizeof ( CCTKREMUSRINFO ) ); 

         memcpy ( static_cast<void *> (remUsrInfo.uUsrName),
                  reinterpret_cast<const void *> (      
                     pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                  strlen( reinterpret_cast<const char *> (
                              pCfgUsr->xtl.usr.net.sipName.pcUsrName)) );
         memcpy ( static_cast<void *> (remUsrInfo.uUsrSigHost),
                  reinterpret_cast<const void *> (      
                              pCfgUsr->xtl.reg.gen.aor.pcAddr ),
                  strlen( reinterpret_cast<const char *> (
                              pCfgUsr->xtl.reg.gen.aor.pcAddr ) ) );
         remUsrInfo.uUsrSigPort = pCfgUsr->xtl.reg.gen.aor.uPort;

         /* If succeeded, issue an event to the new resource state
         ** machine that will take care of the work to be done.
         */
         tkmEvent( pResUser->resourceGuid(),
                   guCeInvalidGuid,
                   eCCTKEVT_REM_USR_ADD,
                   static_cast<void *> (&remUsrInfo) );
      }
   }
   else
   {
      fsmError( rRes );
   }
#else
   fsmError( rRes );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
}

/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRemUsrAdd
**
**  PURPOSE:    A remote user was added to the CCTK framework.  Remote user are
**              third party user which uses the CCTK as a proxy.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmRemUsrAdd( CeResBase &rRes )
{
#if CCTKCFG_REMOTE_USER_CONTROL
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      /* Mark this user as a remote user such that we can apply proper call and
      ** services processing on it later on.
      */
      pResUser->resourceSetRemUsr ( BOS_TRUE );

      /* Inform the upper layer application of the event.
      */
      const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );
      if ( pCfgUsr )
      {
         CCTKREMUSRINFO remUsrInfo;

         memset ( static_cast<void *> (&remUsrInfo),
                  0,
                  sizeof ( CCTKREMUSRINFO ) ); 

         memcpy ( static_cast<void *> (remUsrInfo.uUsrName),
                  reinterpret_cast<const void *> (      
                     pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                  strlen( reinterpret_cast<const char *> (
                              pCfgUsr->xtl.usr.net.sipName.pcUsrName)) );
         memcpy ( static_cast<void *> (remUsrInfo.uUsrSigHost),
                  reinterpret_cast<const void *> (      
                              pCfgUsr->xtl.reg.gen.aor.pcAddr ),
                  strlen( reinterpret_cast<const char *> (
                              pCfgUsr->xtl.reg.gen.aor.pcAddr ) ) );
         remUsrInfo.uUsrSigPort = pCfgUsr->xtl.reg.gen.aor.uPort;

         /* If succeeded, issue an event to the new resource state
         ** machine that will take care of the work to be done.
         */
         tkmEvent( pResUser->resourceGuid(),
                   guCeInvalidGuid,
                   eCCTKEVT_REM_USR_ADD,
                   static_cast<void *> (&remUsrInfo) );
      }
   }
   else
   {
      fsmError( rRes );
   }
#else
   fsmError( rRes );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
}

/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRemUsrRemove
**
**  PURPOSE:    A remote user was removed from the CCTK framework.  Remote user are
**              third party user which uses the CCTK as a proxy.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmRemUsrRemove( CeResBase &rRes )
{
#if CCTKCFG_REMOTE_USER_CONTROL
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      /* Mark sure this user is a remote user.
      */
      if ( pResUser->resourceIsRemUsr() )
      {
          pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_IDLE );
          
         /* Inform the upper layer application of the event.
         */
         const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );
         if ( pCfgUsr )
         {
            CCTKREMUSRINFO remUsrInfo;

            memset ( static_cast<void *> (&remUsrInfo),
                     0,
                     sizeof ( CCTKREMUSRINFO ) ); 

            memcpy ( static_cast<void *> (remUsrInfo.uUsrName),
                     reinterpret_cast<const void *> (      
                        pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                     strlen( reinterpret_cast<const char *> (
                                 pCfgUsr->xtl.usr.net.sipName.pcUsrName)) );
            memcpy ( static_cast<void *> (remUsrInfo.uUsrSigHost),
                     reinterpret_cast<const void *> (      
                                 pCfgUsr->xtl.reg.gen.aor.pcAddr ),
                     strlen( reinterpret_cast<const char *> (
                                 pCfgUsr->xtl.reg.gen.aor.pcAddr ) ) );
            remUsrInfo.uUsrSigPort = pCfgUsr->xtl.reg.gen.aor.uPort;

            /* If succeeded, issue an event to the new resource state
            ** machine that will take care of the work to be done.
            */
            tkmEvent( pResUser->resourceGuid(),
                      guCeInvalidGuid,
                      eCCTKEVT_REM_USR_REMOVE,
                      static_cast<void *> (&remUsrInfo) );
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
#else
   fsmError( rRes );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
}

/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmUsrAdded
**
**  PURPOSE:    The user was added properly within the CCTK framework.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmUsrAdded( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;
   MXD_GNS::ISceUserConfig *pUsrCfg = NULL;
   MXD_GNS::IUaSspRegistrationConfig *pRegCfg = NULL;
   mxt_result res;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      const CFGUSR *pCfgUsr = CFG_USR( rRes.resourceGuid() );

      if ( pCfgUsr )
      {
         if ( pResUser->resourceResetRoute() )
         {
            /* Reset route configuration for this user before assigning the
            ** network configuration.  This operation may be needed in case
            ** we want to force the user into a clean state.
            */
            CCTKSVCROUTE svcRte;

            memset ( static_cast<void *> (&svcRte),
                     0,
                     sizeof ( CCTKSVCROUTE ) );
            tkmSetParm ( pResUser->resourceGuid(),
                         eCCTKPARM_EC_SVCROUTE,
                         static_cast<void *> (&svcRte) );
                         
         }


         /* Configure the registration state event package subscription.  This
         ** configuration is required in case a user swap has occured and 
         ** subscription to the registration state event is now required since
         ** we do not apply subscription for temporary users.
         */
         pUsrCfg = pCfgUsr->in.pSceUsrCfg;
         if ( pUsrCfg != NULL )
         {
            res = pUsrCfg->QueryIf( &pRegCfg );
            if ( MX_RIS_S( res ) && ( pRegCfg != NULL ) )
            {
               if ( ( CFG_SYS->xtl.net.prot.gen.pau.bPauUsrSwap ) &&
                    ( pCfgUsr->xtl.usr.net.sipName.bUsrNameFromTempId ) )
               {
                  /* If user swap is enabled and the current user name is derived
                  ** from temporary IDs, we will defer the subscription until
                  ** the user swap is completed.
                  */
                  pRegCfg->SetRegistrationStateMonitoringMode( 
                           MXD_GNS::IUaSspRegistrationConfig::eMONITORING_NONE );
               }
               else
               {
                  /* If user swap is not enabled or if the user swap has 
                  ** completed, enable registration state event subscription
                  ** if directed to do so
                  */
                  pRegCfg->SetRegistrationStateMonitoringMode(
                           ( pCfgUsr->xtl.reg.gen.regEvt.bRegEvtSupport ?
                                 MXD_GNS::IUaSspRegistrationConfig::eMONITORING_STATE :
                                 MXD_GNS::IUaSspRegistrationConfig::eMONITORING_NONE ) );
               }
               pRegCfg->ReleaseIfRef();
               pRegCfg = NULL;
            }
         }

         pResUser->resourceNetHdl()->SetConfiguration( pCfgUsr->in.pSceUsrCfg );

         /* Add the blind (implicit) message waiting service.  The service is added
         ** at this time to allow immediate process of early blind notification.
         */
         if ( pCfgUsr->xtl.mwi.gen.bUnsolNtfy )
         {
            
            BOS_UINT32 uResGuid = CeMain::allocateGuid();

            CeResBase *pBMwi = pResUser->resourceGetBlindMwi();
            CeResSub *pResMwi = NULL;

            if ( pBMwi != NULL )
            {
               if ( pBMwi->resourceGender() == CeResBase::eCERESTYPE_SUB )
               {
                  pResMwi = reinterpret_cast<CeResSub *> ( pBMwi->resourceAddress() );
               }
               else
               {
                  fsmError( rRes );
                  return;
               }
            }

            if ( (pBMwi == NULL) ||
                 ((pBMwi != NULL) &&
                  (pResMwi->resourceBlindMwiHdl() == NULL)) )
            {
               if ( (uResGuid != guCeInvalidGuid) &&
                     (CeMain::allocateRes( CeResBase::eCERESTYPE_SUB,
                                           uResGuid,
                                           pResUser->resourceGuid(),
                                           BOS_TRUE,
                                           eCCTKSUBEVT_BMWI ) != guCeInvalidGuid) )
               {
                  CeResBase *pRes;

                  CeMain::locateRes( uResGuid, pRes );
                  pResUser->resourceSetBlindMwi( pRes );
                  /* Because this is a blind MWI support, we just treat is as a
                  ** subscription that is already subscribed.  This allows to properly
                  ** process incoming blind notification within the framework.
                  */
                  pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_SUBED );
               }
            }
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmSvcEna
**
**  PURPOSE:    Enable a user specific service controlled within CE.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmSvcEna( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      switch ( pResUser->resourceSvcCtrl() )
      {
         case eCCTKSUBEVT_MWI:
         {
            CeResBase *pSub = pResUser->resourceGetMwiSub();
            CeResSub *pResMwiSub = NULL;

            if ( pSub != NULL )
            {
               if ( pSub->resourceGender() == CeResBase::eCERESTYPE_SUB )
               {
                  pResMwiSub = reinterpret_cast<CeResSub *> ( pSub->resourceAddress() );
               }
               else
               {
                  fsmError( rRes );
                  return;
               }
            }

            if ( (pSub == NULL) ||
                 ((pSub != NULL) &&
                  (pResMwiSub->resourceMwiHdl() == NULL)) )
            {
               /* Create service since we do not have one already.
               */
               const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );
               if ( pCfgUsr &&
                    cfgIsHostPortEnabled(
                        pCfgUsr->xtl.mwi.gen.server.sipHost.hostPort ) )
               {
                  CCTKSIPUSRHOST mwiCfg;
                  MXD_GNS::CNameAddr nameAddr;

                  /* Select the first assumed identity that is associated with this user
                  ** if any was provided.
                  */
                  pResUser->resourceGetAssumedId( 0, nameAddr );

                  /* Update the message waiting indication configuration information first
                  ** if necessary.
                  */
                  if ( nameAddr.GetSipUri() != NULL )
                  {
                     memcpy ( static_cast<void *> (&mwiCfg.sipHost),
                              static_cast<const void *> (
                                             &pCfgUsr->xtl.mwi.gen.server.sipHost),
                              sizeof ( CCTKSIPHOST ) );
                     mwiCfg.pcUsrName =
                        (BOS_UINT8 *) (
                              reinterpret_cast<const BOS_UINT8 *> (
                                    nameAddr.GetSipUri()->GetUser().GetString().CStr()));

                     tkmSetParm( pResUser->resourceGuid(),
                                 eCCTKPARM_MC_MWISVRADDR,
                                 static_cast<void *> (&mwiCfg) );
                  }

                  BOS_UINT32 uResGuid = CeMain::allocateGuid();
                  if ( (uResGuid != guCeInvalidGuid) &&
                       (CeMain::allocateRes( CeResBase::eCERESTYPE_SUB,
                                             uResGuid,
                                             pResUser->resourceGuid(),
                                             BOS_TRUE,
                                             eCCTKSUBEVT_MWI ) != guCeInvalidGuid) )
                  {
                     CeResBase *pRes;

                     CeMain::locateRes( uResGuid, pRes );
                     pResUser->resourceSetMwiSub( pRes );
                     pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_IDLE );
                  }
               }
            }
            /* 
            ** Trigger subscription to MWI service.
            */
            if ( pResUser->resourceGetMwiSub() != NULL )
            {
               ceNotify ( (pResUser->resourceGetMwiSub())->resourceGuid(),
                          eCEEVT_SUB_USR_SUBSCRIBE,
                          NULL );
            }
         }
         break;

         default:
         break;
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmSvcDis
**
**  PURPOSE:    Disables a user specific service controlled within CE.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmSvcDis( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      switch ( pResUser->resourceSvcCtrl() )
      {
         case eCCTKSUBEVT_MWI:
         {
            if ( pResUser->resourceGetMwiSub() != NULL )
            {
               /* Disabling a valid MWI service subscription.
               */
               ceNotify ( (pResUser->resourceGetMwiSub())->resourceGuid(),
                          eCEEVT_SUB_USR_ENDED,
                          NULL );
            }
         }
         break;

         default:
         break;
      }
   }
   else
   {
      fsmError( rRes );
   }
}

/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmTargetFail
**
**  PURPOSE:    A target failure has been reported for this user.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmReg::fsmTargetFail( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      const CFGSYS *pSysCfg = CFG_SYS;
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );
      const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );

      /* Check if the failover feature is enabled
       */
      if (pCfgUsr &&
          pCfgUsr->xtl.usr.net.brCfg.bFailover )
      {
      /* Stop the current keep alive.
      */
      nsNatStopKeepAlive( pResUser->resourceGuid() );
      
         /* Stop the failback timer if it was running.
         */
         if ( pResUser->resourceTmrFailbkPrimReco() != guCeInvalidGuid )
         {
             tkmTimerStop ( pResUser->resourceTmrFailbkPrimReco() );
             pResUser->resourceSetTmrFailbkPrimReco( guCeInvalidGuid );
         }

      /* If we need to select a new target for the upcoming 
      ** registration because of the failure detected, apply the
      ** backup-and-recovery magic now.
      */
      if ( UTIL_BMP_IS_ENABLED_ALL(
                           pSysCfg->xtl.net.prot.gen.sigBehave.uTx,
                           CCTK_SIGBEHVTX_FAILONREREG ) )
      {                     
         const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );
         MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl = NULL;
         const MXD_GNS::CSipPacket *pPacket = NULL;
      
         /* In external list mode, the target has to
         ** be explicitly set to failed in the backup and
         ** recovery list.
         */
         if ( pCfgUsr &&
              pCfgUsr->xtl.usr.net.brCfg.eMode == eCCTKBRSVCMODE_EXTLIST )
         {
            mxt_result res;

            /* Determine the current target address, and 'fail' it.
            */
            res = pResUser->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
            if (  MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
            {
               pSceBasExtCtl->GetLastReceivedPacket( pPacket );
               if ( pPacket && pPacket->IsResponse() )
               {
                  MXD_GNS::CSocketAddr peerAddr = pPacket->GetPeerAddr();
                  nsBrXtlLstSetFailedTgt( pResUser->resourceGuid(),
                                          peerAddr );
               }
               /* Finished with the signaling packet.
               */
               if ( pPacket )
               {
                  pPacket->Release();
                  pPacket = NULL;
               }
               pSceBasExtCtl->ReleaseIfRef();
               pSceBasExtCtl = NULL;
            }
         }
          
         /* Find the backup target, or restart the list if none left.
         */
         if ( nsBrBackupOnFail ( pResUser->resourceGuid() , BOS_FALSE )
                                                == eNSBRSTA_FAILED )
         {
            nsBrRestart ( pResUser->resourceGuid() );
            pResUser->resourceSuccessfulRegCycle(BOS_FALSE);
         }

         /* Select the new target for external list mode.
         */
         if ( pCfgUsr->xtl.usr.net.brCfg.eMode == eCCTKBRSVCMODE_EXTLIST )
         {
            pResUser->resourceSelectNewObProxy();
         }

         /* Report a failure (out-of-service) to the application.
         */
         tkmEvent( pResUser->resourceGuid(),
                   guCeInvalidGuid,
                   eCCTKEVT_USR_OOS,
                   NULL );

         /* Mark that keep alive failed, as we may use this information
         ** for future processing.
         */
         pResUser->resourceTargetFailed( BOS_TRUE );

         /* Terminate this registration immediately, then create a new one
         ** and attempt to register with it again.
         */
         pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGING );
         pResUser->resourceNetHdl()->Terminate();
      }
      else
      {
         /* Issue a re-register */
         ceNotify ( pResUser->resourceGuid(),
                    eCEEVT_REG_USR_REG,
                    NULL );
      }
   }
   }
   else
   {
      fsmError( rRes );
   }
}



/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmNoSvc
**
**  PURPOSE:    No service report for a given user.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmNoSvc( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      /* Report an out-of-service to the application.
      */
      tkmEvent( pResUser->resourceGuid(),
                guCeInvalidGuid,
                eCCTKEVT_USR_OOS,
                NULL );
   }
   else
   {
      fsmError( rRes );
   }
}

/***********************************************************************************
**  FUNCTION:   CeFsmReg::fsmRegInbound
**
**  PURPOSE:    Inbound failover (call recieved from non-registered server) reported
**              for a given user.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmReg::fsmRegInbound ( CeResBase &rRes )
{
   CeResUser *pResUser = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_USR )
   {
      pResUser = reinterpret_cast<CeResUser *> ( rRes.resourceAddress() );

      /* If inbound failover is enabled then attempt registration with the 
      ** inbound target if possible
      */
      const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );
      if ( ( pCfgUsr != NULL ) &&
           pCfgUsr->xtl.usr.net.brCfg.bEnableInbdReg )
      {
         CeMain::CeDispatchRestrictedUser( pResUser->resourceGuid(), BOS_FALSE );
      }
   }
   else
   {
      fsmError( rRes );
   }
   
}

/***********************************************************************************
**  FUNCTION:   regGetRetryAfter
**
**  PURPOSE:    Retrieve the value from the Retry-after header in the registration
**              response.  Note that since the Retry-after header is not applicable
**              for the 401 and 423 negative responses and thus is ignored.  Also,
**              if Retry-After header is not included in the 403 response, the 
**              configured Retry-After timer will be retrieved instead.
**
**  PARAMETERS:
**              pResUser - reference to the resource this function applies to.
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/

static BOS_UINT32 regGetRetryAfter( CeResUser *pResUser )
{
   mxt_result res;
   const MXD_GNS::CSipPacket *pPacket = NULL;
   MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl = NULL;
   BOS_UINT32 uRetryAfterValue = 0;

   res = pResUser->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
   if (  MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
   {
      pSceBasExtCtl->GetLastReceivedPacket( pPacket );
      if ( pPacket && pPacket->IsResponse() )
      {
         /* Retry-after header is ignored for 401, 423, and Timer F expiry */
         if ( ( pPacket->GetStatusLine()->GetCode() != MXD_GNS::uUNAUTHORIZED ) &&
              ( pPacket->GetStatusLine()->GetCode() != MXD_GNS::uINTERVAL_TOO_BRIEF ) &&
               !(( pPacket->GetStatusLine()->GetCode() == MXD_GNS::uREQUEST_TIMEOUT ) &&
                 ( pPacket->IsLocallyGenerated() )) )
         {
            /* Fetch the Retry-after value if a Retry-after header
            ** was specified */
            const MXD_GNS::CSipHeader *pRetryHdr =
               	pPacket->GetHeaderList().Get(
               			MXD_GNS::eHDR_RETRY_AFTER, res );

            if( MX_RIS_S( res ) && pRetryHdr != NULL)
            {
               pRetryHdr->GetRetryAfter().GetUint32(
                     reinterpret_cast<uint32_t &>(uRetryAfterValue));
            }
         }
         /* Special case for 403 response code without Retry-After header */
         if ( (uRetryAfterValue == 0) && (pPacket->GetStatusLine()->GetCode() == MXD_GNS::uFORBIDDEN) )
         {
            const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );
            if ( pCfgUsr != NULL)
            {
               uRetryAfterValue = pCfgUsr->xtl.reg.gen.tmr.uRegRetryTmrOn403;
            }
         }
      }
      
      /* 
      ** Finished with the signaling packet.
      */
      if ( pPacket )
      {
         pPacket->Release();
         pPacket = NULL;
      }
            
      pSceBasExtCtl->ReleaseIfRef();
      pSceBasExtCtl = NULL;
   }

   return ( uRetryAfterValue );
}

