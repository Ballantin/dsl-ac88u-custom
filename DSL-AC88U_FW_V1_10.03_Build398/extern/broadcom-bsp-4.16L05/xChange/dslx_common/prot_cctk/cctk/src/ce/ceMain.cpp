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
**      This file implements the Call Engine main.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceMain.h>
#include <ceFsmCallIn.h>
#include <ceFsmCallOut.h>
#if CCTKCFG_REMOTE_USER_CONTROL
#include <cfgMain.h>
#include <ceFsmCallProxyIn.h>
#include <ceFsmCallProxyOut.h>
#include <SipUserAgent/ISipUaAssertedIdentitySvc.h>
#include <SipCoreSvc/ISipStatelessDigestServerAuthSvc.h>
#include <SipCoreSvc/ResultIdSipCoreSvc.h>
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
#include <ceFsmCall.h>
#include <ceFsmHold.h>
#include <ceFsmXfer.h>
#include <ceFsmPub.h>
#include <ceFsmSub.h>
#include <ceFsmReg.h>

#include <ceApi.h>
#include <SipParser/HeaderParameter.h>

#if CCTKCFG_REMOTE_USER_CONTROL
/* Default IMRG Authentication value */
static char gpPassword[] = "boardcom";
static char gpUserId[] = "IMRGUser";
static char gpIMGRealm[] = "IMRG";
#endif /* CCTKCFG_REMOTE_USER_CONTROL */


/* ---- Public Variables -------------------------------------------------------- */

const BOS_UINT32 guCeInvalidGuid    = CCTK_INVALID_HANDLE;
const BOS_UINT32 guCeGuidSeed       = 0xCAFE0001;
const BOS_UINT32 guCeBusyMarker     = 0xCE15B02E;  /* 'CE is busy' */
const BOS_UINT32 guCeSequenceInit   = 1;

/* ---- Private Constants and Types --------------------------------------------- */
#define CE_GUID_MUTEX     "CeGuidMutex"
#define CE_NONCE_COUNT_INIT       1
#define CE_DEF_REMOTE_USR_REG_EXPIRE_VAL    60

// IMS XML strings.
#define CE_CONTENT_TYPE_APPLICATION    "application"
#define CE_CONTENT_TYPE_IMS_XML        "3gpp-ims+xml"

#define CE_IMS_XML_ELEMENT_IMS3GPP     "ims-3gpp"
#define CE_IMS_XML_ELEMENT_ALTSVC      "alternative-service"
#define CE_IMS_XML_ELEMENT_TYPE        "type"
#define CE_IMS_XML_ELEMENT_REASON      "reason"
#define CE_IMS_XML_ELEMENT_ACTION      "action"

#define CE_IMS_XML_ATTRIBUTE_VERSION   "version"

#define UASSPREGMGR       MXD_GNS::IUaSspRegistrationMgr
#define UASSPSUBMGR       MXD_GNS::ISceSubscriberMgr
#define UASSPCALLMGR      MXD_GNS::IUaSspCallMgr

/* ---- Private Variables ------------------------------------------------------- */
CeMain *gpCeMain = NULL;

CeResUser   *CeMain::mpResUser[guCeMainMaxUser];
CeResCall   *CeMain::mpResCall[CCTKCFG_MAX_CALL];
CeResSub    *CeMain::mpResSub[CCTKCFG_MAX_EVT];
CeResNot    *CeMain::mpResNot[CCTKCFG_MAX_NOT];
CeCdrMgr    *CeMain::mpCdrMgr;
CeFsmBase   *CeMain::mpFsm[CeFsmBase::eCEFSMTYPE_MAX];
BOS_UINT32  CeMain::muGuidSeed;
BOS_MUTEX   CeMain::mGuidMutex;
BOS_BOOL    CeMain::mbReset;
BOS_BOOL    CeMain::mbResetNot;
BOS_BOOL    CeMain::mbSuspend;
BOS_UINT32  CeMain::muTmrRst;


/* ---- Private Maps------------------------------------------------------------- */

/* Map of the registration manager statuses to a valid FSM event.  See the
** "IUaSspRegistrationMgr" for event definition.
*/
static CeMain::CEMAINEVTMAP gRegMgrEvtMap[] =
{
   { UASSPREGMGR::eUNREGISTERED,                 eCEEVT_REG_NET_UNREG },
   { UASSPREGMGR::eUNREGISTERED_ONPROBATION,     eCEEVT_REG_NET_TRYING },
   { UASSPREGMGR::eUNREGISTERED_TRYING,          eCEEVT_REG_NET_TRYING },
   { UASSPREGMGR::eUNREGISTERED_TIMEOUT,         eCEEVT_REG_NET_TIMEOUT },
   { UASSPREGMGR::eREGISTERED,                   eCEEVT_REG_NET_OK },
   { UASSPREGMGR::eREGISTERED_REFRESHING,        eCEEVT_REG_NET_REFRESH },
   { UASSPREGMGR::eREGISTRATION_FAILED,          eCEEVT_REG_NET_FAIL },
   { UASSPREGMGR::eREGISTRATION_FAILED_REJECTED, eCEEVT_REG_NET_FAIL },
   { UASSPREGMGR::eTERMINATED,                   eCEEVT_REG_NET_ENDED },
   { eCEEVT_REG_NET_CHALLENGE,                   eCEEVT_REG_NET_CHALLENGE },
   { eCEEVT_REG_NET_SRV_FAIL,                    eCEEVT_REG_NET_SRV_FAIL },
   { eCEEVT_REG_NET_NEXT_NONCE,                  eCEEVT_REG_NET_NEXT_NONCE },
   { eCEEVT_REG_NET_OPTIONS,                     eCEEVT_REG_NET_OPTIONS },
   { eCEEVT_REG_NET_REFER,                       eCEEVT_REG_NET_REFER },
   { eCEEVT_REG_NET_REFENDED,                    eCEEVT_REG_NET_REFENDED },
   { eCEEVT_REG_NET_PAU_ID,                      eCEEVT_REG_NET_PAU_ID },
   { eCEEVT_REG_NET_USR_ADD,                     eCEEVT_REG_NET_USR_ADD },
   { eCEEVT_REG_NET_USR_REMOVE,                  eCEEVT_REG_NET_USR_REMOVE },
   { eCEEVT_REG_USR_REG,                         eCEEVT_REG_USR_REG },
   { eCEEVT_REG_USR_UNREG,                       eCEEVT_REG_USR_UNREG },
   { eCEEVT_REG_USR_ENDED,                       eCEEVT_REG_USR_ENDED },
   { eCEEVT_REG_USR_KA_OK,                       eCEEVT_REG_USR_KA_OK },
   { eCEEVT_REG_USR_KA_FAIL,                     eCEEVT_REG_USR_KA_FAIL },
   { eCEEVT_REG_USR_KA_RECOVERY,                 eCEEVT_REG_USR_KA_RECOVERY },
   { eCEEVT_REG_USR_PUBLISH,                     eCEEVT_REG_USR_PUBLISH },
   { eCEEVT_REG_USR_ADD_OK,                      eCEEVT_REG_USR_ADD_OK },
   { eCEEVT_REG_USR_SVC_ENA,                     eCEEVT_REG_USR_SVC_ENA },
   { eCEEVT_REG_USR_SVC_DIS,                     eCEEVT_REG_USR_SVC_DIS },
   { eCEEVT_REG_USR_TARGET_FAIL,                 eCEEVT_REG_USR_TARGET_FAIL },
   { eCEEVT_REG_USR_MSG,                         eCEEVT_REG_USR_MSG },
   { eCEEVT_REG_USR_INBOUND_TARGET,              eCEEVT_REG_USR_INBOUND_TARGET },

   { eCEEVT_USR_REM,                             eCEEVT_USR_REM },
   { eCEEVT_CALL_ADD,                            eCEEVT_CALL_ADD },
   { eCEEVT_CALL_REM,                            eCEEVT_CALL_REM },
   /* Must be last - always. */
   { 0,                                          eCEEVT_DNP },
};

/* Map of the internally generated CCTK events as intercepted by the
** user resource (registration state machine).
*/
static CeMain::CEMAINEVTMAP gResUsrCctkEvtMap[] =
{
   { eCCTKEVT_FN_USERADD_SUCCESS,                eCEEVT_REG_USR_ADD_OK },

   /* Must be last - always. */
   { 0,                                          eCEEVT_DNP },
};

/* Map of the call manager statuses to a valid FSM event.  Events are either
** coming from one of the "IUaSspCallMgr" callback(in which case they are premapped) 
** or from the MSE status.
*/
static CeMain::CEMAINEVTMAP gCallMgrEvtMap[] =
{
   { eCEEVT_CALL_NET_CALLED,                     eCEEVT_CALL_NET_CALLED },
   { eCEEVT_CALL_NET_TERMUSR,                    eCEEVT_CALL_NET_TERMUSR },
   { eCEEVT_CALL_NET_TERMNET,                    eCEEVT_CALL_NET_TERMNET },
   { eCEEVT_CALL_NET_FAILED,                     eCEEVT_CALL_NET_FAILED },
   { eCEEVT_CALL_NET_ANSWER,                     eCEEVT_CALL_NET_ANSWER },
   { eCEEVT_CALL_NET_PEND_OFFER_SDP,             eCEEVT_CALL_NET_PEND_OFFER_SDP },
   { eCEEVT_CALL_NET_PEND_ANSWER_SDP,            eCEEVT_CALL_NET_PEND_ANSWER_SDP },
   { eCEEVT_CALL_NET_ANSWER_SDP,                 eCEEVT_CALL_NET_ANSWER_SDP },
   { eCEEVT_CALL_NET_INFO,                       eCEEVT_CALL_NET_INFO },
   { eCEEVT_CALL_NET_EARLYMEDIA,                 eCEEVT_CALL_NET_EARLYMEDIA },
   { eCEEVT_CALL_NET_REPLACE,                    eCEEVT_CALL_NET_REPLACE },
   { eCEEVT_CALL_NET_RING,                       eCEEVT_CALL_NET_RING },
   { eCEEVT_HOLD_NET_RING,                       eCEEVT_CALL_NET_RING },
   { eCEEVT_HOLD_NET_TERMNET,                    eCEEVT_CALL_NET_TERMNET },
   { eCEEVT_REG_NET_REFER,                       eCEEVT_CALL_NET_XFER },
   { eCCTKEVT_MEDIA,                             eCEEVT_CALL_NET_MEDIA },
   { eCCTKEVT_MEDIA_FAIL,                        eCEEVT_CALL_NET_MEDIA_FAIL },
   { eCCTKEVT_HOLD,                              eCEEVT_CALL_NET_HOLD },
   { eCEEVT_CALL_USR_CREATE,                     eCEEVT_CALL_USR_CREATE },
   { eCEEVT_CALL_USR_MAKE,                       eCEEVT_CALL_USR_MAKE },
   { eCEEVT_CALL_USR_MEDIA,                      eCEEVT_CALL_USR_MEDIA },
   { eCEEVT_CALL_USR_ANSWER,                     eCEEVT_CALL_USR_ANSWER },
   { eCEEVT_CALL_USR_HOLD,                       eCEEVT_CALL_USR_HOLD },
   { eCEEVT_CALL_USR_FORWARD,                    eCEEVT_CALL_USR_FORWARD },
   { eCEEVT_CALL_USR_XFER,                       eCEEVT_CALL_USR_XFER },
   { eCEEVT_CALL_USR_BXFER,                      eCEEVT_CALL_USR_BXFER },
   { eCEEVT_CALL_USR_XFER_NOTERM,                eCEEVT_CALL_USR_XFER_NOTERM },
   { eCEEVT_CALL_USR_ENDED,                      eCEEVT_CALL_USR_ENDED },
   { eCEEVT_CALL_USR_PROXY,                      eCEEVT_CALL_USR_PROXY },
   { eCEEVT_CALL_USR_DTMF,                       eCEEVT_CALL_USR_DTMF },
   { eCEEVT_CALL_USR_FLASH,                      eCEEVT_CALL_USR_FLASH },
   { eCEEVT_REG_USR_ENDED,                       eCEEVT_CALL_USR_ENDED },
   { eCEEVT_HOLD_USR_UNHOLD,                     eCEEVT_CALL_USR_UNHOLD },

   { eCEEVT_CALL_REM,                            eCEEVT_CALL_REM },
   /* Must be last - always. */
   { 0,                                          eCEEVT_DNP },
};

/* Map of the transfer manager statuses to a valid FSM event.  Events are
** coming from the "IUaSspCallMgr" callback and are premmaped within the callbacks.
*/
static CeMain::CEMAINEVTMAP gXferMgrEvtMap[] =
{
   { eCEEVT_XFER_NET_XFER,                             eCEEVT_XFER_NET_XFER },
   { eCEEVT_XFER_NET_REFENDED,                         eCEEVT_XFER_NET_REFENDED },
   { eCEEVT_XFER_NET_FAIL,                             eCEEVT_XFER_NET_FAIL },
   { eCEEVT_XFER_NET_OKAY,                             eCEEVT_XFER_NET_OKAY },
   { eCEEVT_XFER_NET_ACCEPT,                           eCEEVT_XFER_NET_ACCEPT },
   { eCEEVT_XFER_USR_NTTO,                             eCEEVT_XFER_USR_NTTO },
   { UASSPCALLMGR::eTRANSFER_RESULT_PROGRESSING,       eCEEVT_XFER_NET_PROC },
   { UASSPCALLMGR::eTRANSFER_RESULT_SUCCESS,           eCEEVT_XFER_NET_OKAY },
   { UASSPCALLMGR::eTRANSFER_RESULT_FAILURE,           eCEEVT_XFER_NET_FAIL },
   { UASSPCALLMGR::eTRANSFER_RESULT_REFER_FAILURE,     eCEEVT_XFER_NET_FAIL },
   { UASSPCALLMGR::eTRANSFER_RESULT_REFER_SUCCESS,     eCEEVT_XFER_NET_ACCEPT },
   { eCEEVT_CALL_NET_RING,                             eCEEVT_XFER_NET_RING },
   { eCEEVT_CALL_NET_EARLYMEDIA,                       eCEEVT_XFER_NET_EARLYMEDIA },
   { eCEEVT_CALL_NET_ANSWER,                           eCEEVT_XFER_NET_ANSWER },
   { eCEEVT_CALL_NET_TERMNET,                          eCEEVT_XFER_NET_TERMNET },
   { eCEEVT_CALL_NET_XFER,                             eCEEVT_XFER_NET_XFER },
   { eCEEVT_CALL_USR_ENDED,                            eCEEVT_XFER_USR_ENDED },
   { eCEEVT_CALL_USR_XFER,                             eCEEVT_XFER_USR_XFER },
   { eCEEVT_CALL_USR_BXFER,                            eCEEVT_XFER_USR_BXFER },
   { eCEEVT_CALL_USR_XFER_NOTERM,                      eCEEVT_XFER_USR_XFER_NOTERM },
   { eCEEVT_REG_USR_ENDED,                             eCEEVT_XFER_USR_ENDED },

   { eCEEVT_CALL_REM,                                  eCEEVT_CALL_REM },

   /* We do not want to process those events. */
   { UASSPCALLMGR::eTRANSFER_RESULT_IN_PROGRESS,       eCEEVT_INV },
   { UASSPCALLMGR::eTRANSFER_RESULT_REFER_IN_PROGRESS, eCEEVT_INV },

   /* Must be last - always. */
   { 0,                                                eCEEVT_DNP },
};

/* Map of the subscription manager statuses to a valid FSM event.  See the
** "ISceSubscriberMgr" for event definition.
*/
static CeMain::CEMAINEVTMAP gSubMgrEvtMap[] =
{
   { UASSPSUBMGR::eSUBSCRIBED,                   eCEEVT_SUB_NET_OK },
   { UASSPSUBMGR::eSUBSCRIBED_PENDING,           eCEEVT_SUB_NET_SUBTRYING },
   { UASSPSUBMGR::eSUBSCRIBED_REFRESHING,        eCEEVT_SUB_NET_REFRESH },
   { UASSPSUBMGR::eSUBSCRIPTION_FAILED,          eCEEVT_SUB_NET_FAIL },
   { UASSPSUBMGR::eTERMINATED,                   eCEEVT_SUB_NET_ENDED },
   { UASSPSUBMGR::eUSER_TERMINATED,              eCEEVT_SUB_NET_ENDED },
   { UASSPSUBMGR::eUNSUBSCRIBED,                 eCEEVT_SUB_NET_UNSUB },
   { UASSPSUBMGR::eUNSUBSCRIBED_TRYING,          eCEEVT_SUB_NET_RETRY },
   { UASSPSUBMGR::eUNSUBSCRIBING,                eCEEVT_SUB_NET_UNSUBTRYING },
   { eCEEVT_SUB_NET_ENDED,                       eCEEVT_SUB_NET_ENDED },
   { eCEEVT_SUB_NET_NOTIFY,                      eCEEVT_SUB_NET_NOTIFY },
   { eCEEVT_SUB_NET_REQ,                         eCEEVT_SUB_NET_REQ },
   { eCEEVT_SUB_NET_REQEND,                      eCEEVT_SUB_NET_REQEND },
   { eCEEVT_SUB_USR_CREATE,                      eCEEVT_SUB_USR_CREATE },
   { eCEEVT_SUB_USR_SUBSCRIBE,                   eCEEVT_SUB_USR_SUBSCRIBE },
   { eCEEVT_SUB_USR_UNSUBSCRIBE,                 eCEEVT_SUB_USR_UNSUBSCRIBE },
   { eCEEVT_SUB_USR_ENDED,                       eCEEVT_SUB_USR_ENDED },
   { eCEEVT_REG_USR_ENDED,                       eCEEVT_SUB_USR_ENDED },

   { eCEEVT_SUB_REM,                             eCEEVT_SUB_REM },
   /* Must be last - always. */
   { 0,                                          eCEEVT_DNP },
};

/* Map of the hold manager statuses to a valid FSM event.
*/
static CeMain::CEMAINEVTMAP gHoldMgrEvtMap[] =
{
   { eCCTKEVT_MEDIA,                             eCEEVT_HOLD_NET_MEDIA },
   { eCCTKEVT_HOLD,                              eCEEVT_HOLD_NET_HOLD },
   { eCCTKEVT_UNHOLD,                            eCEEVT_HOLD_NET_UNHOLD },
   { eCEEVT_CALL_NET_HOLD,                       eCEEVT_HOLD_NET_HOLD },
   { eCEEVT_CALL_NET_RING,                       eCEEVT_HOLD_NET_RING },
   { eCEEVT_CALL_NET_TERMUSR,                    eCEEVT_HOLD_NET_TERMUSR }, 
  { eCEEVT_CALL_NET_TERMNET,                    eCEEVT_HOLD_NET_TERMNET },
   { eCEEVT_CALL_NET_XFER,                       eCEEVT_HOLD_NET_XFER },
   { eCEEVT_REG_NET_REFER,                       eCEEVT_HOLD_NET_XFER },
   { eCCTKEVT_LHOLD,                             eCEEVT_HOLD_USR_HOLD_OK },
   { eCCTKEVT_LHOLD_FAIL,                        eCEEVT_HOLD_USR_HOLD_FAIL },
   { eCCTKEVT_LUNHOLD_FAIL,                      eCEEVT_HOLD_USR_UNHOLD_FAIL },
   { eCCTKEVT_LUNHOLD,                           eCEEVT_HOLD_USR_UNHOLD_OK },
   { eCEEVT_CALL_USR_HOLD,                       eCEEVT_HOLD_USR_HOLD },
   { eCEEVT_HOLD_USR_UNHOLD,                     eCEEVT_HOLD_USR_UNHOLD },
   { eCEEVT_CALL_USR_ENDED,                      eCEEVT_HOLD_USR_ENDED },
   { eCEEVT_REG_USR_ENDED,                       eCEEVT_HOLD_USR_ENDED },
   { eCEEVT_CALL_USR_XFER,                       eCEEVT_HOLD_USR_XFER },
   { eCEEVT_CALL_USR_BXFER,                      eCEEVT_HOLD_USR_BXFER },
   { eCEEVT_CALL_USR_XFER_NOTERM,                eCEEVT_HOLD_USR_XFER_NOTERM },
   { eCEEVT_CALL_REM,                            eCEEVT_CALL_REM },
   /* Must be last - always. */
   { 0,                                          eCEEVT_DNP },
};


/* Map of the notification/publication manager statuses to a valid FSM event.
*/
static CeMain::CEMAINEVTMAP gPubMgrEvtMap[] =
{
   { eCEEVT_NOT_NET_OK,                          eCEEVT_NOT_NET_OK },
   { eCEEVT_NOT_NET_FAIL,                        eCEEVT_NOT_NET_FAIL },
   { eCEEVT_NOT_NET_MSG,                         eCEEVT_NOT_NET_MSG },
   { eCEEVT_NOT_USR_RSP,                         eCEEVT_NOT_USR_RSP },
   { eCEEVT_REG_USR_ENDED,                       eCEEVT_NOT_USR_ENDED },
   { eCEEVT_REG_USR_PUBLISH,                     eCEEVT_NOT_USR_PUBLISH },
   { eCEEVT_REG_USR_MSG,                         eCEEVT_NOT_USR_MSG },

   { eCEEVT_NOT_REM,                             eCEEVT_NOT_REM },
   /* Must be last - always. */
   { 0,                                          eCEEVT_DNP },
};


/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */


/* ---- "C" API (START OF) ------------------------------------------------------ */
#ifdef __cplusplus
extern "C"
{
#endif

/***********************************************************************************
**  FUNCTION:   ceInit
**
**  PURPOSE:    External API to initialize (and launch) the Call Engine module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This API must be invoked prior to any further Call Engine work
**              is possible.
***********************************************************************************/
void ceInit ( void )
{
   if ( gpCeMain == NULL )
   {
      if (( gpCeMain = BRCM_NEW( CeMain )) == NULL )
      {
         /* Failed... we are doomed.
         */
         CCTK_TRACEI0( "ceInit - Failure to initialize properly!" );
      }
   }
   else
   {
      /* Print an error.
      */
      CCTK_TRACEI1( "ceInit - Invalid call, already initialized (%p).",
                    gpCeMain );
   }
}


/***********************************************************************************
**  FUNCTION:   ceDeInit
**
**  PURPOSE:    External API to de-initialize the Call Engine module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      After this API has been invoked, there is no further processing
**              possible from the Call Engine.
***********************************************************************************/
void ceDeInit ( void )
{
   if ( gpCeMain != NULL )
   {
      BRCM_DELETE( gpCeMain );
      gpCeMain = NULL;
   }
   else
   {
      /* Already de-init, or never started.  Ignore.
      */
      CCTK_TRACEI0( "ceDeInit - Invalid call, already de-initialized." );
   }
}


/***********************************************************************************
**  FUNCTION:   ceShow
**
**  PURPOSE:    External API to invoke the display of internal information from
**              the Call Engine module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
***********************************************************************************/
void ceShow( void )
{
   if ( gpCeMain != NULL )
   {
      gpCeMain->show();
   }
   else
   {
      CCTK_TRACEI0( "ceShow - Invalid call, Call Engine invalid." );
   }
}


/***********************************************************************************
**  FUNCTION:   ceShutDown
**
**  PURPOSE:    External API to shutdown the Call Engine module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      Shutting down the Call Engine module is equivalent to stopping
**              any ongoing network transactions on that module.
***********************************************************************************/
CCTKSTATUS ceShutDown ( void )
{
   CCTKSTATUS eStatus = eCCTKSTATUS_NOOP;

   if ( gpCeMain != NULL )
   {
      eStatus = gpCeMain->shutDown ();
   }

   return eStatus;
}


/***********************************************************************************
**  FUNCTION:   ceNotify
**
**  PURPOSE:    External API to notify the Call Engine module of an event to
**              be processed.
**
**  PARAMETERS:
**              uHdl  - The handle of the resource on which this event applies.
**              eEvt  - The event being notified.
**              pData - Some supplementary data which may be passed with the
**                      event.  The data is event dependent.
**
**  RETURNS:    Status as defined in the CCTKSTATUS enum.
**
**  NOTES:      .
***********************************************************************************/
CCTKSTATUS ceNotify( BOS_UINT32 uHdl, CEEVT eEvt, void *pData )
{
   CCTKSTATUS eStatus = eCCTKSTATUS_NOOP;

   if ( gpCeMain != NULL )
   {
      CEEVT eMapEvt;
      CeResBase *pRes = NULL;

      /* Map the events as necessary.
      */
      if ( (uHdl != guCeInvalidGuid) && gpCeMain->locateRes( uHdl, pRes ) )
      {
         gpCeMain->fsmMapEvent( pRes->resourceFsm(),
                                static_cast<BOS_UINT32> (eEvt),
                                eMapEvt );
      }
      else
      {
         eMapEvt = eEvt;
      }

      eStatus = gpCeMain->fsmProcessEvt( uHdl,
                                         eMapEvt,
                                         pData );
   }

   return eStatus;
}


/***********************************************************************************
**  FUNCTION:   ceOutEvtInfo
**
**  PURPOSE:    External API to notify the Call Engine module of an event to
**              be processed.
**
**  PARAMETERS:
**              uUsrHdl  - The handle of the user resource on which this event
**                         applies.
**              uEvtHdl  - The specific event handle of the resource on which this
**                         event applies.  This handle can be invalid.
**              eEvt     - The event being passed to the outside application.
**
**  RETURNS:    Status as defined in the CCTKSTATUS enum.
**
**  NOTES:      .
***********************************************************************************/
CCTKSTATUS ceOutEvtInfo( BOS_UINT32 uUsrHdl, BOS_UINT32 uEvtHdl, CCTKEVT eEvt )
{
   CCTKSTATUS eStatus = eCCTKSTATUS_NOOP;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uEvtHdl );

   if ( gpCeMain != NULL )
   {
      CEEVT eMapEvt;
      CeResBase *pRes = NULL;

      /* Map the event from an outside application CCTKEVT to an internal
      ** CEEVT for Call Engine consumption.
      */
      if ( (uUsrHdl != guCeInvalidGuid) && gpCeMain->locateRes( uUsrHdl, pRes ) )
      {
         gpCeMain->fsmMapCctkEvent( pRes->resourceFsm(),
                                    static_cast<BOS_UINT32> (eEvt),
                                    eMapEvt );

         eStatus = gpCeMain->fsmProcessEvt( uUsrHdl,
                                            eMapEvt,
                                            NULL );
      }
   }

   return eStatus;
}


/***********************************************************************************
**  FUNCTION:   ceGuid
**
**  PURPOSE:    External API to generate a unique global identifier that is used
**              to reference a resource within the CCTK application.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    The GUID, may return 'guCeInvalidGuid' if such GUID cannot be
**              retrieved.  In such case, the resource is invalid.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 ceGuid( void )
{
   if ( gpCeMain != NULL )
   {
      return gpCeMain->allocateGuid();
   }
   else
   {
      return guCeInvalidGuid;
   }
}


/***********************************************************************************
**  FUNCTION:   ceResAlloc
**
**  PURPOSE:    External API to check whether resources have been allocated by the
**              Call Engine.  Allocated resources can be of any nature, but as long
**              as at least one is present, this check will be positive.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    BOS_TRUE if at least one resource of any kind is currently in use
**              (i.e. allocated) by the Call Engine.  BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL ceResAlloc( void )
{
   if ( gpCeMain != NULL )
   {
      return gpCeMain->checkFinalRes( BOS_FALSE );
   }
   else
   {
      return BOS_FALSE;
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetMediaInfo
**
**  PURPOSE:    External API to retrieve the media information associated with
**              a specific call resource.
**
**  PARAMETERS:
**              uCid - the call resource handler.
**
**
**  RETURNS:    The pointer to the media information if successfully retrieved
**              from the call resource handle, NULL otherwise.
**
**  NOTES:      .
***********************************************************************************/
CCTKMEDINFO *ceGetMediaInfo( BOS_UINT32 uCid )
{
   if ( gpCeMain != NULL )
   {
      CCTKMEDINFO *pMedInfo;

      if ( gpCeMain->locateMediaInfo( uCid, pMedInfo ) )
      {
         return pMedInfo;
      }
      else
      {
         return NULL;
      }
   }
   else
   {
      return NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetExpInfo
**
**  PURPOSE:    External API to retrieve the expiration information associated with
**              a specific call resource.
**
**  PARAMETERS:
**              uUid - the call resource handler.
**
**
**  RETURNS:    The pointer to the media information if successfully retrieved
**              from the call resource handle, NULL otherwise.
**
**  NOTES:      .
***********************************************************************************/
CCTKEXPIREINFO *ceGetExpInfo( BOS_UINT32 uUid)
{
   if ( gpCeMain != NULL )
   {
      CCTKEXPIREINFO *pExpInfo;
      if ( gpCeMain->locateExpireInfo( uUid, pExpInfo ) )
      {
         return pExpInfo;
      }
      else
      {
         return NULL;
      }
   }
   else
   {
      return NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetPmRes
**
**  PURPOSE:    External API to retrieve the resource associated with a given
**              payload modifier (paylod modifier is an entity that can be used
**              to monitor and in some cases update the payload format of the
**              signaling).
**
**  PARAMETERS:
**              pData - pointer to the payload modifier component.
**
**
**  RETURNS:    The GUID of the associated resource if found; guCeInvalidGuid
**              otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 ceGetPmRes( void *pData )
{
   if ( gpCeMain != NULL )
   {
      /* Always return the primary resource GUID, no need to worry about
      ** possible associated resources which are deal with internally to CE
      ** engine only.
      */
      return gpCeMain->locatePmRes( pData, BOS_FALSE );
   }
   else
   {
      return guCeInvalidGuid;
   }
}


/***********************************************************************************
**  FUNCTION:   ceSetCdrInfo
**
**  PURPOSE:    External API to associate a CDR with a call identification
**              information for that call.
**
**  PARAMETERS:
**              uResGuid  - the resource that the CDR should be updated for.
**              puCid     - pointer to the call identification.
**              puFromTag - pointer to the from tag identification.
**              puToTag   - pointer to the to tag identification.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      Any of the pointer passed may be NULL if the information was
**              not present at the time of the call and because some information
**              is setup iterratively.
***********************************************************************************/
void ceSetCdrInfo( BOS_UINT32 uResGuid, const BOS_UINT8 *puCid,
                   const BOS_UINT8 *puFromTag, const BOS_UINT8 *puToTag )
{
   if ( gpCeMain != NULL )
   {
      gpCeMain->associateCid( uResGuid, puCid, puFromTag, puToTag );
   }
}


/***********************************************************************************
**  FUNCTION:   ceSetCallInfo
**
**  PURPOSE:    External API to associate a CDR with the call statistics
**              information for that call.
**
**  PARAMETERS:
**              uResGuid  - the resource that the CDR should be updated for.
**              pStats - the call statistics of interest.
**
**
**  RETURNS:    BOS_TRUE if the association could be made, BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL ceSetCallInfo( BOS_UINT32 uResGuid, CCTKCALLMETRICS *pStats )
{
   BOS_BOOL bRet = BOS_FALSE;

   if ( gpCeMain != NULL )
   {
      bRet = gpCeMain->associateStats( uResGuid, pStats );
   }

   return bRet;
}

/***********************************************************************************
**  FUNCTION:   ceSetPrivInfo
**
**  PURPOSE:    External API to update the privacy info of a call. Note that
**              this won't actually configure the privacy settings of the call but
**              rather updates the information kept for reference - used only in
**              call transfer scenarios at the moment.
**              
**
**  PARAMETERS:
**              uResGuid  - the resource that the CDR should be updated for.
**              pStats - the call statistics of interest.
**
**
**  RETURNS:    BOS_TRUE if the association could be made, BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL ceSetPrivInfo( BOS_UINT32 uResGuid, CCTKPRIVCFG *pPriv )
{
   BOS_BOOL bRet = BOS_FALSE;

   if ( gpCeMain != NULL )
   {
      bRet = gpCeMain->setPrivInfo( uResGuid, pPriv );
   }

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   ceSetPublishAddr
**
**  PURPOSE:    External API to set the IP address to which PUBLISH messages
**              are being transmitted to
**
**  PARAMETERS:
**              uResGuid  - the resource that the PUBLISH is associated with
**              pPubAddr  - the IP address to which PUBLISH messages are being
**                          transmitted to
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void ceSetPublishAddr( BOS_UINT32 uResGuid, MX_NS CSocketAddr *pPubAddr )
{
   if ( gpCeMain != NULL )
   {
      gpCeMain->setPublishAddr( uResGuid, pPubAddr );
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetLocInfo
**
**  PURPOSE:    External API to retrieve the XML formatted location information
**              associated with a call.
**
**  PARAMETERS:
**              uResGuid - the resource that should lookup the information for.
**              puData   - pointer to the data to be formatted.
**              uData    - size of the buffer to write the data to.
**
**
**  RETURNS:    The actual size of the final buffer containing the formatted
**              information.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 ceGetLocInfo( BOS_UINT8 *puData, BOS_UINT32 uData,
                         BOS_UINT32 uResGuid )
{
   if ( gpCeMain != NULL )
   {
      return gpCeMain->associateLocInfo( uResGuid, puData, uData );
   }
   else
   {
      return 0;
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetLastCallIn
**
**  PURPOSE:    External API to retrieve the last known incoming call information
**              associated with a user resource.
**
**  PARAMETERS:
**              uUid - the user resource handler.
**
**
**  RETURNS:    The pointer to the call record information if successfully
**              retrieved from the user resource handle, NULL otherwise.
**
**  NOTES:      .
***********************************************************************************/
CCTKCDR *ceGetLastCallIn( BOS_UINT32 uUid )
{
   if ( gpCeMain != NULL )
   {
      CCTKCDR *pCdr;

      if ( gpCeMain->locateCallInfo( uUid, CeCdr::eCECDRTYPE_IN, pCdr ) )
      {
         return pCdr;
      }
      else
      {
         return NULL;
      }
   }
   else
   {
      return NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetLastCallOut
**
**  PURPOSE:    External API to retrieve the last known outgoing call information
**              associated with a user resource.
**
**  PARAMETERS:
**              uUid - the user resource handler.
**
**
**  RETURNS:    The pointer to the call record information if successfully
**              retrieved from the user resource handle, NULL otherwise.
**
**  NOTES:      .
***********************************************************************************/
CCTKCDR *ceGetLastCallOut( BOS_UINT32 uUid )
{
   if ( gpCeMain != NULL )
   {
      CCTKCDR *pCdr;

      if ( gpCeMain->locateCallInfo( uUid, CeCdr::eCECDRTYPE_OUT, pCdr ) )
      {
         return pCdr;
      }
      else
      {
         return NULL;
      }
   }
   else
   {
      return NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetRegId
**
**  PURPOSE:    External API to retrieve the registration identifer associated
**              with a specific resource.
**
**  PARAMETERS:
**              uUid - the resource identifier.
**
**
**  RETURNS:    The registration identifier of the associated resource if found;
**              guCeInvalidGuid otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 ceGetRegId( BOS_UINT32 uUid )
{
   if ( gpCeMain != NULL )
   {
      return gpCeMain->locateRegId( uUid );
   }
   else
   {
      return guCeInvalidGuid;
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetPrecond
**
**  PURPOSE:    External API to query wether a specific resource is using
**              precondition.
**
**  PARAMETERS:
**              uUid - the resource identifier.
**
**
**  RETURNS:    BOS_TRUE if the resource of interest is using precondition.
**              BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL ceGetPrecond( BOS_UINT32 uUid )
{
   if ( gpCeMain != NULL )
   {
      return gpCeMain->locatePrecond( uUid );
   }
   else
   {
      return BOS_FALSE;
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetProfInfo
**
**  PURPOSE:    External API to retrieve the profile information associated with
**              a resource.
**
**  PARAMETERS:
**              uResUid - the resource identifier.
**              puParentUid - the resource parent identifier.
**              puProfIx - the associated profile identifier.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      puParentGuid and puProfIx are being updated as a result of this
**              function call and either may return guCeInvalidGuid in case of
**              error.
***********************************************************************************/
void ceGetProfInfo( BOS_UINT32 uResGuid,
                    BOS_UINT32 *puParentGuid,
                    BOS_UINT32 *puProfIx )
{
   if ( gpCeMain != NULL )
   {
      return gpCeMain->locateProfId( uResGuid, puParentGuid, puProfIx );
   }
   else
   {
      *puParentGuid = guCeInvalidGuid;
      *puProfIx = guCeInvalidGuid;
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetFeatInfo
**
**  PURPOSE:    External API to retrieve the feature information for a given user.
**
**  PARAMETERS:
**              uResUid - the resource identifier.
**              pFeatInfo - the information being retrieved.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      pFeatInfo is being updated as a result of this call.
***********************************************************************************/
void ceGetFeatInfo( BOS_UINT32 uResGuid,
                    CCTKFEATINFO *pFeatInfo )
{
   if ( gpCeMain != NULL )
   {
      gpCeMain->featInfo( uResGuid, pFeatInfo );
   }
}


/***********************************************************************************
**  FUNCTION:   ceUsrAuthChg
**
**  PURPOSE:    External API to check whether one should be overriding the
**              authorization header from a registration refresh attempt.
**
**  PARAMETERS:
**              uResUid - the resource identifier.
**
**  RETURNS:    BOS_TRUE if such action should be taken, BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL ceUsrAuthChg( BOS_UINT32 uResGuid )
{
   if ( gpCeMain != NULL )
   {
      return gpCeMain->locateAuthChg( uResGuid );
   }
   else
   {
      return BOS_FALSE;
   }
}


/***********************************************************************************
**  FUNCTION:   ceUsrAuthRem
**
**  PURPOSE:    External API to set whether one should be overriding the
**              authorization header from a registration refresh attempt.
**
**  PARAMETERS:
**              uResUid - the resource identifier.
**              bRem - the value to be set.
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void ceUsrAuthRem( BOS_UINT32 uResGuid, BOS_BOOL bRem )
{
   if ( gpCeMain != NULL )
   {
      gpCeMain->manageAuthChg( uResGuid, bRem );
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetNetHdl
**
**  PURPOSE:    External API to retrieve the network handle associated with a
**              resource.  The network handle corresponds to the UASSP/SCE handle
**              of reference associated with a given Call Engine resource.
**
**  PARAMETERS:
**              uResUid - the resource identifier.
**
**  RETURNS:    Pointer to the network handle if valid, NULL otherwise.
**
**  NOTES:      .
***********************************************************************************/
void *ceGetNetHdl( BOS_UINT32 uResGuid )
{
   if ( gpCeMain != NULL )
   {
      return gpCeMain->getNetHdl( uResGuid );
   }
   else
   {
      return NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   ceGetUsrGuid
**
**  PURPOSE:    External API to retrieve user GUID resource associated with a given
**              GUID.
**
**  PARAMETERS:
**              uGuid - the resource identifier.
**
**  RETURNS:    The associated user resource or guCeInvalidGuid if not applicable.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 ceGetUsrGuid( BOS_UINT32 uGuid )
{
   if ( gpCeMain != NULL )
   {
      return gpCeMain->getUsrGuid( uGuid );
   }
   else
   {
      return guCeInvalidGuid;
   }
}


/***********************************************************************************
**  FUNCTION:   ceRemMgmtRes
**
**  PURPOSE:    External API to retrieve whether a resource is a remote managed one
**              meaning it belongs to a remote user.
**
**  PARAMETERS:
**              uResGuid - the resource identifier.
**
**  RETURNS:    BOS_TRUE if the resource is indeed a remote managed one, BOS_FALSE
**              otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL ceRemMgmtRes( BOS_UINT32 uResGuid )
{
   if ( gpCeMain != NULL )
   {
      return gpCeMain->remoteRes( uResGuid );
   }
   else
   {
      return BOS_FALSE;
   }
}

/***********************************************************************************
**  FUNCTION:   ceGetResRstrictAddr
**
**  PURPOSE:    External API to retrieve the restricted target address info for a
**              a given GUID. Currently only available for call resources.
**
**  PARAMETERS:
**              uResGuid - the resource identifier.
**
**  RETURNS:    Pointer to the restricted address info if valid, NULL otherwise.
**
**  NOTES:      .
***********************************************************************************/
MXD_GNS::CSocketAddr *ceGetResRstrictAddr( BOS_UINT32 uResGuid )
{
   if ( gpCeMain != NULL )
   {
      return gpCeMain->getResRstrictAddr( uResGuid );
   }
   else
   {
      return NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   ceGetCallXRtpStat
**
**  PURPOSE:    External API to retrieve the X-Rtp-Stat associated with
**              a specific outgoing call resource.
**
**  PARAMETERS:
**              uCid - the call resource handler.
**              pStat - the statistics being retrieved
**
**
**  RETURNS:    BOS_TRUE if information can be retrieved,
**              BOS_FALSE otherwise
**
**  NOTES:      pStat is being updated as a result of this call.
***********************************************************************************/
BOS_BOOL ceGetCallXRtpStat( BOS_UINT32 uResGuid, CCTKXRTPSTAT *pStat)
{
   if ( gpCeMain != NULL )
   {
      if (gpCeMain->locateCallXRtpStat( uResGuid, pStat ) )
      {
         return BOS_TRUE;
      }
      else
      {
         return BOS_FALSE;
      }
   }
   else
   {
      return BOS_FALSE;
   }
   return BOS_FALSE;
}


#ifdef __cplusplus
}
#endif
/* ---- "C" API (END OF) -------------------------------------------------------- */



/***********************************************************************************
**  FUNCTION:   CeMain::CeMain
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
CeMain::CeMain( void )
{
   BOS_UINT32 uIx;

   mbReset = BOS_FALSE;
   mbResetNot = BOS_FALSE;
   mbSuspend = BOS_FALSE;
   muGuidSeed = guCeGuidSeed;
   muTmrRst = guCeInvalidGuid;

   /* Create the mutex for the GUID allocator.
   */
   if ( bosMutexCreate( CE_GUID_MUTEX, &mGuidMutex ) != BOS_STATUS_OK )
   {
      CCTK_TRACEI0( "Main - Failed to create GUID mutex!" );
   }

   mpCdrMgr = BRCM_NEW( CeCdrMgr );

   /* Create our pool(s) of resources.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      mpResUser[uIx] = BRCM_NEW( CeResUser );
   }
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      mpResCall[uIx] = BRCM_NEW( CeResCall );
   }
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      mpResSub[uIx] = BRCM_NEW( CeResSub );
   }
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
   {
      mpResNot[uIx] = BRCM_NEW( CeResNot );
   }

   /* Create our pool of FSM.
   */
   mpFsm[CeFsmBase::eCEFSMTYPE_IN]   =
      static_cast<CeFsmBase *> (BRCM_NEW( CeFsmCallIn ));

   mpFsm[CeFsmBase::eCEFSMTYPE_OUT]  =
      static_cast<CeFsmBase *> (BRCM_NEW( CeFsmCallOut ));

   mpFsm[CeFsmBase::eCEFSMTYPE_CALL] =
      static_cast<CeFsmBase *> (BRCM_NEW( CeFsmCall ));
#if CCTKCFG_REMOTE_USER_CONTROL
   mpFsm[CeFsmBase::eCEFSMTYPE_PROXY_IN]   =
      static_cast<CeFsmBase *> (BRCM_NEW( CeFsmCallProxyIn ));

   mpFsm[CeFsmBase::eCEFSMTYPE_PROXY_OUT]   =
      static_cast<CeFsmBase *> (BRCM_NEW( CeFsmCallProxyOut ));
#endif
   mpFsm[CeFsmBase::eCEFSMTYPE_HOLD] =
      static_cast<CeFsmBase *> (BRCM_NEW( CeFsmHold ));

   mpFsm[CeFsmBase::eCEFSMTYPE_XFER] =
      static_cast<CeFsmBase *> (BRCM_NEW( CeFsmXfer ));

   mpFsm[CeFsmBase::eCEFSMTYPE_PUB]  =
      static_cast<CeFsmBase *> (BRCM_NEW( CeFsmPub ));

   mpFsm[CeFsmBase::eCEFSMTYPE_SUB]  =
      static_cast<CeFsmBase *> (BRCM_NEW( CeFsmSub ));

   mpFsm[CeFsmBase::eCEFSMTYPE_REG]  =
      static_cast<CeFsmBase *> (BRCM_NEW( CeFsmReg ));
}


/***********************************************************************************
**  FUNCTION:   CeMain::~CeMain
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
CeMain::~CeMain( void )
{
   BOS_UINT32 uIx;

   if ( muTmrRst != guCeInvalidGuid )
   {
      tkmTimerStop ( muTmrRst );
      muTmrRst = guCeInvalidGuid;
   }

   /* Destroy GUID mutex.
   */
   bosMutexDestroy( &mGuidMutex );

   /* Delete all resources allocated.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      BRCM_DELETE( mpResUser[uIx] );
      mpResUser[uIx] = NULL;
   }
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      BRCM_DELETE( mpResCall[uIx] );
      mpResCall[uIx] = NULL;
   }
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      BRCM_DELETE( mpResSub[uIx] );
      mpResSub[uIx] = NULL;
   }
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
   {
      BRCM_DELETE( mpResNot[uIx] );
      mpResNot[uIx] = NULL;
   }

   /* Delete all FSMs allocated.
   */
   for ( uIx = 0 ; uIx < CeFsmBase::eCEFSMTYPE_MAX ; uIx++ )
   {
      BRCM_DELETE( mpFsm[uIx] );
      mpFsm[uIx] = NULL;
   }

   BRCM_DELETE( mpCdrMgr );
   mpCdrMgr = NULL;
}


/***********************************************************************************
**  FUNCTION:   CeMain::startUp
**
**  PURPOSE:    Startup the Call Engine application.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    BOS_TRUE on success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL CeMain::startUp( void )
{
   /* Ensure we are no longer in reset mode if a previous shutdown
   ** had put us in such and data was not cleared.
   */
   if ( mbReset )
   {
      mbReset = BOS_FALSE;
      mbResetNot = BOS_FALSE;
   }
   
   /* Setup some information to the UA layer properly.
   */
   MXD_GNS::ISceCoreConfig *pSceCfg = NULL;
   BOS_BOOL bRes = BOS_TRUE;

   mxt_result res = CreateEComInstance( MXD_GNS::CLSID_CSceCoreConfig,
                                        NULL,
                                        &pSceCfg );
   if ( MX_RIS_S( res ) && (pSceCfg != NULL) )
   {
      MXD_GNS::IUaSspCallHandler *pCallHdl = NULL;
      MXD_GNS::ISceNotifierHandler *pNotHdl = NULL;
      MXD_GNS::ISceBasicExtensionControl *pBasExtCtl = NULL;
      MXD_GNS::ISceReferHandler *pReferHdl = NULL;

      /* Register the generic request handler, although this is setup,
      ** it is not used at this time.
      */
      pSceCfg->RegisterRequestHandler( gpCeMain );


      /* Setup the UaSsp call handler such that we can process new
      ** calls properly.
      */
      res = pSceCfg->GetHandlerInterface( &pCallHdl );
      if ( MX_RIS_S( res ) && (pCallHdl != NULL) )
      {
         pCallHdl->SetManager( gpCeMain );
         pCallHdl->ReleaseIfRef();
         pCallHdl = NULL;
      }
      else
      {
         bRes = BOS_FALSE;
      }


      /* Setup the Sce notifier handler such that we can process new
      ** subscriptions properly.
      */
      res = pSceCfg->GetHandlerInterface( &pNotHdl );
      if ( MX_RIS_S( res ) && (pNotHdl != NULL) )
      {
         pNotHdl->SetManager( gpCeMain );
         pNotHdl->ReleaseIfRef();
         pNotHdl = NULL;
      }
      else
      {
         bRes = BOS_FALSE;
      }


      /* Setup the Sce notifier handler such that we can process new
      ** subscriptions properly.
      */
      res = pSceCfg->GetHandlerInterface( &pBasExtCtl );
      if ( MX_RIS_S( res ) && (pBasExtCtl != NULL) )
      {
         pBasExtCtl->SetBasicExtensionManager( gpCeMain );
         pBasExtCtl->ReleaseIfRef();
         pBasExtCtl = NULL;
      }
      else
      {
         bRes = BOS_FALSE;
      }

      /* Setup the Sce referrer handler such that we can process new
      ** referrer properly.
      */
      res = pSceCfg->GetHandlerInterface( &pReferHdl );
      if ( MX_RIS_S( res ) && (pReferHdl != NULL) )
      {
         pReferHdl->SetManager( gpCeMain );
         pReferHdl->ReleaseIfRef();
         pReferHdl = NULL;
      }
      else
      {
         bRes = BOS_FALSE;
      }

      pSceCfg->ReleaseIfRef();
      pSceCfg = NULL;
   }
   else
   {
      bRes = BOS_FALSE;
   }

   return bRes;
}


/***********************************************************************************
**  FUNCTION:   CeMain::shutDown
**
**  PURPOSE:    Shuts-down the Call Engine application network resources.
**
**  PARAMETERS:
**              eType - the type of resources to shutdown, passing the
**                      default (CeResBase::eCERESTYPE_MAX) will clean
**                      all the resources managed by Call Engine.
**              uParentGuid - the parent guid for the owner of the
**                            resource to shutdown, passing the default
**                            (guCeInvalidGuid) will clean all the resources
**                            independently of which parent they belong to.
**
**  RETURNS:    eCCTKSTATUS_SUCCESS on success, eCCTKSTATUS_INTERR otherwise.
**
**  NOTES:      Note: this call will always return eCCTKSTATUS_SUCCESS because
**                    such success is guaranteed by the framework underneath.
**              Note: this is not exactly a shutdown of the Call Engine, it is a
**                    shutdown of the signaling operations associated with the
**                    Call Engine via a forcefull termination of the network
**                    resources that may be active at the time.
***********************************************************************************/
CCTKSTATUS CeMain::shutDown( CeResBase::CERESTYPE eType, BOS_UINT32 uParentGuid )
{
   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;
   BOS_UINT32 uIx;

   /* If we are not in reset mode already, go in it now to prevent any
   ** attempt to recreate internal resources while we are shutting down.
   ** Note: If there is a specific resource we're trying to shut down
   ** we're not really in reset mode.
   */
   if ( !mbReset && (uParentGuid == guCeInvalidGuid) )
   {
      mbReset = BOS_TRUE;
   }

   if ( ( eType == CeResBase::eCERESTYPE_NOT ) ||
        ( eType == CeResBase::eCERESTYPE_MAX ) )
   {
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
      {
         if ( mpResNot[uIx] &&
              (mpResNot[uIx]->resourceGuid() != guCeInvalidGuid) &&
              ((uParentGuid == guCeInvalidGuid) ||
               ((uParentGuid != guCeInvalidGuid) &&
                  (uParentGuid == mpResNot[uIx]->resourceParent()))) )
         {
            if ( mpResNot[uIx]->resourceNetOodHdl() )
            {
               /* Immediately terminate the existing resource.
               */
               mpResNot[uIx]->resourceTerminate();
            }
         }
      }
   }

   if ( ( eType == CeResBase::eCERESTYPE_SUB ) ||
        ( eType == CeResBase::eCERESTYPE_MAX ) )
   {
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
      {
         if ( mpResSub[uIx] &&
              (mpResSub[uIx]->resourceGuid() != guCeInvalidGuid) &&
              ((uParentGuid == guCeInvalidGuid) ||
               ((uParentGuid != guCeInvalidGuid) &&
                  (uParentGuid == mpResSub[uIx]->resourceParent()))) )
         {
            if ( mpResSub[uIx]->resourceNotHdl() )
            {
               mpResSub[uIx]->resourceNotHdl()->TerminateImmediately();
            }

            if ( mpResSub[uIx]->resourceMwiHdl() )
            {
               mpResSub[uIx]->resourceMwiHdl()->Terminate();
            }

            if ( mpResSub[uIx]->resourceBlindMwiHdl() )
            {
               mpResSub[uIx]->resourceBlindMwiHdl()->Terminate();
            }
         }
      }
   }

   if ( ( eType == CeResBase::eCERESTYPE_CALL ) ||
        ( eType == CeResBase::eCERESTYPE_MAX ) )
   {
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
      {
         if ( mpResCall[uIx] &&
              (mpResCall[uIx]->resourceGuid() != guCeInvalidGuid) &&
              ((uParentGuid == guCeInvalidGuid) ||
               ((uParentGuid != guCeInvalidGuid) &&
                  (uParentGuid == mpResCall[uIx]->resourceParent()))) )
         {
            if ( mpResCall[uIx]->resourceNetReferrer() )
            {
               mpResCall[uIx]->resourceNetReferrer()->TerminateImmediately();
            }

            if ( mpResCall[uIx]->resourceNetHdl() )
            {
               mpResCall[uIx]->resourceNetHdl()->TerminateImmediately();
            }

            mpResCall[uIx]->resourceForkTermImmediately();
         }
      }
   }

   if ( ( eType == CeResBase::eCERESTYPE_USR ) ||
        ( eType == CeResBase::eCERESTYPE_MAX ) )
   {
      for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
      {
         if ( mpResUser[uIx] &&
              (mpResUser[uIx]->resourceGuid() != guCeInvalidGuid) &&
              mpResUser[uIx]->resourceNetHdl() &&
              ((uParentGuid == guCeInvalidGuid) ||
               ((uParentGuid != guCeInvalidGuid) &&
                  /* Parent GUID is the actual resource GUID for this
                  ** resource type. */
                  (uParentGuid == mpResUser[uIx]->resourceGuid()))) )
         {
            mpResUser[uIx]->resourceNetHdl()->TerminateImmediately();
         }
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   CeMain::show
**
**  PURPOSE:    Shows the dynamic information about current Call Engine controlled
**              resources.
**
**  PARAMETERS: None
**
**  RETURNS:    None.
***********************************************************************************/
void CeMain::show( void )
{
#if CCTKCFG_APPLICATION_STATUS
   BOS_UINT32 uIx;

   CCTK_TRACEI0( "\nCCTK - User Resources.\n" );
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[ uIx ] )
      {
         mpResUser[ uIx ]->resourceShow();
      }
   }

   CCTK_TRACEI0( "\nCCTK - Call Resources.\n" );
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpResCall[ uIx ] )
      {
         mpResCall[ uIx ]->resourceShow();
      }
   }

   CCTK_TRACEI0( "\nCCTK - Event Resources.\n" );
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[ uIx ] )
      {
         mpResSub[ uIx ]->resourceShow();
      }
   }

   CCTK_TRACEI0( "\nCCTK - Notification Resources.\n" );
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
   {
      if ( mpResNot[ uIx ] )
      {
         mpResNot[ uIx ]->resourceShow();
      }
   }

#endif /* CCTKCFG_APPLICATION_STATUS */
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateRes
**
**  PURPOSE:    Locates a resource in the pool given its GUID.
**
**  PARAMETERS:
**              uResGuid - the GUID of the resource looked up.
**              prRes    - pointer (reference) to the found GUID (if applicable).
**
**  RETURNS:    BOS_TRUE  - if resource located.
**              BOS_FALSE - if resource cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, pRes should be ignored.
***********************************************************************************/
BOS_BOOL CeMain::locateRes( BOS_UINT32 uResGuid, CeResBase *&prRes )
{
   BOS_UINT32 uIx;

   /* Catch all case if we are trying to locate an invalid resource.
   */
   if ( uResGuid == guCeInvalidGuid )
   {
      prRes = NULL;
      return BOS_FALSE;
   }

   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] &&
           ( (mpResUser[uIx]->resourceGuid() == uResGuid) ||
             /* Associated GUID leads to the same resource reference
             ** being returned.
             */
             (mpResUser[uIx]->resourceSubRegGuid() == uResGuid) ) )
      {
         prRes = mpResUser[uIx];
         return BOS_TRUE;
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpResCall[uIx] && (mpResCall[uIx]->resourceGuid() == uResGuid))
      {
         prRes = mpResCall[uIx];
         return BOS_TRUE;
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx] && (mpResSub[uIx]->resourceGuid() == uResGuid))
      {
         prRes = mpResSub[uIx];
         return BOS_TRUE;
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
   {
      if ( mpResNot[uIx] && (mpResNot[uIx]->resourceGuid() == uResGuid))
      {
         prRes = mpResNot[uIx];
         return BOS_TRUE;
      }
   }

   /* Could not locate the resource with the GUID of interest.  Must not be
   ** a valid one.
   */
   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateCallRes
**
**  PURPOSE:    Locates a specific call resource in the pool given its network
**              handle.
**
**  PARAMETERS:
**              pCall     - the network handle of the resource to lookup.
**              prRes     - pointer (reference) to the found resource.
**              bInternal - flag to determine whether this call is the result of
**                          an internal action or not.
**
**  RETURNS:    BOS_TRUE  - if resource located.
**              BOS_FALSE - if resource cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, pRes should be ignored.
**
**              When bInternal is set, this is assumed to be used from within the
**              Call Engine (as opposed to being invoked due to a UASSP/SCE action)
**              in which case, we do not take action in case the resource cannot be
**              located.  We simply assume the resource is no longer existing/valid.
***********************************************************************************/
BOS_BOOL CeMain::locateCallRes( MXD_GNS::IUaSspCall* pCall,
                                CeResBase *&prRes,
                                BOS_BOOL bInternal )
{
   BOS_UINT32 uIx;

   prRes = NULL;

   if ( pCall == NULL )
   {
      return BOS_FALSE;
   }

   /* Make sure this is for a known call and retrieve the corresponding
   ** call resource.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpResCall[uIx] && mpResCall[uIx]->resourceEqual( pCall ))
      {
         prRes = mpResCall[uIx];
         break;
      }
   }
   /* If we could not locate a valid resource for this call, check for the need
   ** to release the reference to prevent any misbehavior from the UASSP/SCE side.
   */
   if ( ( prRes == NULL ) && !bInternal )
   {
      /* If this is for a forked call resource or a call that was marked busy
      ** that we need to reject specifically, we simply ignore the situation
      ** and do not release the reference.
      */
      if ( !locateCallForkRes ( pCall, prRes ) &&
           (reinterpret_cast<BOS_UINT32> (pCall->GetOpaque()) != guCeBusyMarker) )
      {
         pCall->ReleaseIfRef();
         pCall = NULL;
      }
      prRes = NULL;
      return BOS_FALSE;
   }
   /* If we could not locate a valid resource for this call, and this is an internal
   ** lookup, the call resource must have been cleared already, so just ignore it.
   */
   else if ( ( prRes == NULL ) && bInternal )
   {
      return BOS_FALSE;
   }
   else
   {
      return BOS_TRUE;
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateBrRes
**
**  PURPOSE:    Locates the resource associated with the backup and recovery GUID
**              information passed in.
**
**  PARAMETERS:
**              uResGuid - the GUID of the resource looked up.
**              prRes    - pointer (reference) to the found GUID (if applicable).
**
**  RETURNS:    BOS_TRUE  - if resource located.
**              BOS_FALSE - if resource cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, pRes should be ignored.
***********************************************************************************/
BOS_BOOL CeMain::locateBrRes( BOS_UINT32 uResGuid, CeResBase *&prRes )
{
   BOS_UINT32 uIx;

   /* Catch all case if we are trying to locate an invalid resource.
   */
   if ( uResGuid == guCeInvalidGuid )
   {
      prRes = NULL;
      return BOS_FALSE;
   }

   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] &&
           ( mpResUser[uIx]->resourceTmrBkupReco() == uResGuid ) )
      {
         prRes = mpResUser[uIx];
         return BOS_TRUE;
      }
   }

   /* Could not locate the resource with the GUID of interest.  Must not be
   ** a valid one.
   */
   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateTmrRes
**
**  PURPOSE:    Locates a specific resource in the pool given the timer
**              associated with it.
**
**  PARAMETERS:
**              uTmrGuid - the timer handle of interest.
**              prRes    - pointer (reference) to the found resource.
**
**  RETURNS:    BOS_TRUE  - if resource located.
**              BOS_FALSE - if resource cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, pRes should be ignored.
***********************************************************************************/
BOS_BOOL CeMain::locateTmrRes( BOS_UINT32 uTmrGuid,
                               CeResBase *&prRes )
{
   BOS_UINT32 uIx;

   prRes = NULL;

   /* Currently, we only use timer with call resources and only for the call
   ** transfer services.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpResCall[uIx] &&
           (mpResCall[uIx]->resourceXferTimerGuid() == uTmrGuid) )
      {
         prRes = mpResCall[uIx];
         return BOS_TRUE;
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateCallForkRes
**
**  PURPOSE:    Locates a specific forked call resource in the pool given its
**              network handle.
**
**  PARAMETERS:
**              pCall    - the network handle of the forked resource to lookup.
**              prRes    - pointer (reference) to the found resource.
**
**  RETURNS:    BOS_TRUE  - if resource located.
**              BOS_FALSE - if resource cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, pRes should be ignored.
***********************************************************************************/
BOS_BOOL CeMain::locateCallForkRes( MXD_GNS::IUaSspCall* pCall,
                                    CeResBase *&prRes )
{
   BOS_UINT32 uIx;

   prRes = NULL;

   /* Make sure this is for a known call and retrieve the corresponding
   ** call resource.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpResCall[uIx] && mpResCall[uIx]->resourceEqualFork( pCall ))
      {
         prRes = mpResCall[uIx];
         break;
      }
   }

   if ( prRes == NULL )
   {
      return BOS_FALSE;
   }
   else
   {
      return BOS_TRUE;
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateReferRes
**
**  PURPOSE:    Locates a specific referrer resource in the pool given its network
**              handle.
**
**  PARAMETERS:
**              pRefer   - the network handle of the resource to lookup.
**              prRes    - pointer (reference) to the found resource.
**
**  RETURNS:    BOS_TRUE  - if resource located.
**              BOS_FALSE - if resource cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, pRes should be ignored.
***********************************************************************************/
BOS_BOOL CeMain::locateReferRes( MXD_GNS::ISceReferrer* pRefer,
                                 CeResBase *&prRes )
{
   BOS_UINT32 uIx;

   prRes = NULL;

   /* Check whether this referrer is associated with a user resource or a call
   ** resource.  A referrer may be associated with a user resource if it has just
   ** been created and is not yet allocated to any call, or when the call it was
   ** associated with has been terminated and ownership of the referrer has been
   ** transmitted back to the user.  Otherwise, a referrer is associated with a
   ** call resource.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] && mpResUser[uIx]->resourceOwner( pRefer ))
      {
         prRes = mpResUser[uIx];
         break;
      }
   }

   if ( prRes == NULL )
   {
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
      {
         if ( mpResCall[uIx] && mpResCall[uIx]->resourceOwner( pRefer ))
         {
            prRes = mpResCall[uIx];
            break;
         }
      }
   }

   /* Determine if resource was located.
   */
   if ( prRes == NULL )
   {
      return BOS_FALSE;
   }
   else
   {
      return BOS_TRUE;
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateMediaInfo
**
**  PURPOSE:    Locates the media information associated with a specific
**              resource.
**
**  PARAMETERS:
**              uResGuid  - the GUID of the resource looked up.
**              prMedInfo - pointer (reference) to the associated media
**                          information retrieved (if applicable).
**
**  RETURNS:    BOS_TRUE  - if information found.
**              BOS_FALSE - if information cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, prMedInfo should be ignored.
***********************************************************************************/
BOS_BOOL CeMain::locateMediaInfo( BOS_UINT32 uResGuid,
                                  CCTKMEDINFO *&prMedInfo )
{
   CeResBase *pRes = NULL;

   if ( locateRes( uResGuid, pRes ) )
   {
      CeResCall *pResCall = NULL;
      MXD_GNS::MseSdpSession *pMse = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
      {
         pResCall = reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );

         pResCall->resourceGetMse( pMse );
         if ( pMse )
         {
            prMedInfo = const_cast<CCTKMEDINFO *> (pMse->getMediaInfo());
            return (( prMedInfo == NULL ) ? BOS_FALSE : BOS_TRUE );
         }
      }
      else
      {
         CCTK_TRACEI1( "Locating media info - Invalid call 0x%lX!", uResGuid );
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateExpireInfo
**
**  PURPOSE:    Locates the expiration information associated with a specific
**              resource.
**
**  PARAMETERS:
**              uResGuid  - the GUID of the resource looked up.
**              prExpInfo - pointer (reference) to the associated media
**                          information retrieved (if applicable).
**
**  RETURNS:    BOS_TRUE  - if information found.
**              BOS_FALSE - if information cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, prExpInfo should be ignored.
***********************************************************************************/
BOS_BOOL CeMain::locateExpireInfo( BOS_UINT32 uResGuid,
      CCTKEXPIREINFO *&prExpInfo )
{
   CeResBase *pRes = NULL;

   if ( locateRes( uResGuid, pRes ) )
   {
      CeResUser *pResUser = NULL;
      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         pResUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );
         prExpInfo = const_cast<CCTKEXPIREINFO *> (pResUser->resourceGetRegExp());
         return BOS_TRUE;
      }
      else
      {
         CCTK_TRACEI1( "Locating reg expiry info - Invalid user 0x%lX!", uResGuid );
      }
   }
   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateCallInfo
**
**  PURPOSE:    Locates the call information associated with a specific
**              resource.
**
**  PARAMETERS:
**              uResGuid  - the GUID of the resource looked up.
**              eType     - the type of information to locate.
**              prCdr     - pointer (reference) to the associated call
**                          information (CDR) retrieved (if applicable).
**
**  RETURNS:    BOS_TRUE  - if information found.
**              BOS_FALSE - if information cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, prCdr should be ignored.
***********************************************************************************/
BOS_BOOL CeMain::locateCallInfo( BOS_UINT32 uResGuid,
                                 CeCdr::CECDRTYPE eType,
                                 CCTKCDR *&prCdr )
{
   CeResBase *pRes = NULL;

   if ( locateRes( uResGuid, pRes ) )
   {
      CeResUser *pResUser = NULL;
      CeCdr *pCdr = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         pResUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );

         pCdr = pResUser->resourceGetLastCall( eType );
         if ( pCdr && pCdr->cdrData() )
         {
            prCdr = const_cast<CCTKCDR *> (pCdr->cdrData());
            return BOS_TRUE;
         }
      }
      else
      {
         CCTK_TRACEI1( "Locating call info - Invalid user 0x%lX!", uResGuid );
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::locatePmRes
**
**  PURPOSE:    Locates the resource associated with a given payload modifier.
**
**  PARAMETERS:
**              pData  - the payload modifier to lookup.
**              bAssData - whether we care about associated data GUID when such
**                         is available.  If BOS_TRUE, we would return the GUID
**                         for the data (possibly the associated one) which maps
**                         the best to the pData looked up.  If BOS_FALSE, we
**                         only care about the primary resource, and would always
**                         use that GUID to identify the data pointed to by pData.
**
**  RETURNS:    The GUID of the located resource if valid.
**              Otherwise: guCeInvalidGuid.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 CeMain::locatePmRes( void *pData,
                                BOS_BOOL bAssData )
{
   BOS_UINT32 uIx;
   BOS_BOOL bAssRes = BOS_FALSE;
   mxt_result res;
   MXD_GNS::ISceBasicExtensionControl *pMyBasicExtCtl;
   BOS_BOOL bMatch = BOS_FALSE;
   BOS_UINT32 uGuid;

   /* Locate the component associated with this request.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] && mpResUser[uIx]->resourceBasicCtlEqual(
               reinterpret_cast<MXD_GNS::ISceBasicExtensionControl *>( pData ),
               bAssRes ))
      {
         return ( ( bAssRes && bAssData ) ?
                     /* We have an associated resource and we care about knowing
                     ** about it.
                     */
                     mpResUser[uIx]->resourceSubRegGuid() :
                     /* By default, return the user GUID which is the primary
                     ** identifier for all information associated with this user
                     ** including associated resources unless we specifically need
                     ** to know otherwise.
                     */
                     mpResUser[uIx]->resourceGuid() );
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpResCall[uIx] && mpResCall[uIx]->resourceBasicCtlEqual(
               reinterpret_cast<MXD_GNS::ISceBasicExtensionControl *>( pData ) ))
      {
         return mpResCall[uIx]->resourceGuid();
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx] && mpResSub[uIx]->resourceBasicCtlEqual(
               reinterpret_cast<MXD_GNS::ISceBasicExtensionControl *>( pData ) ))
      {
         return mpResSub[uIx]->resourceGuid();
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
   {
      if ( mpResNot[uIx] && mpResNot[uIx]->resourceBasicCtlEqual(
               reinterpret_cast<MXD_GNS::ISceBasicExtensionControl *>( pData ) ))
      {
         return mpResNot[uIx]->resourceGuid();
      }
   }

   /* If we cannot match any of the resources, this may be a payload modifier 
   ** associated with a refer.  Check against the user resources first.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] )
      {
         uGuid = mpResUser[uIx]->resourceReferrerLookupBasicCtl( 
               reinterpret_cast<MXD_GNS::ISceBasicExtensionControl *>( pData ) );
         if ( uGuid != guCeInvalidGuid )
         {
            return ( uGuid );
         }
      }
   }

   /* Next, check the call resources for the payload modifier associated with
   ** a refer.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpResCall[uIx] )
      {
         if  ( mpResCall[uIx]->resourceNetReferrer() )
         {
            pMyBasicExtCtl = NULL;
            res = mpResCall[uIx]->resourceNetReferrer()->QueryIf( &pMyBasicExtCtl );
            if ( MX_RIS_S( res ) && ( pMyBasicExtCtl != NULL ) )
            {
               if ( pMyBasicExtCtl == 
                        reinterpret_cast<MXD_GNS::ISceBasicExtensionControl *>( pData ) )
               {
                   bMatch = BOS_TRUE;
               }

               pMyBasicExtCtl->ReleaseIfRef();
               pMyBasicExtCtl = NULL;

               if ( bMatch )
               {
                   return ( mpResCall[uIx]->resourceGuid() );
               }
            }
         }
      }
   }


   return guCeInvalidGuid;
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateRegId
**
**  PURPOSE:    Locates the resource registration identifier.
**
**  PARAMETERS:
**              uResGuid  - the resource GUID.
**
**  RETURNS:    The registration identifier for the resource if valid.
**              Otherwise: guCeInvalidGuid.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 CeMain::locateRegId( BOS_UINT32 uResGuid )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] && ( mpResUser[uIx]->resourceGuid() == uResGuid ))
      {
         return mpResUser[uIx]->resourceRegId();
      }
   }

   return guCeInvalidGuid;
}


/***********************************************************************************
**  FUNCTION:   CeMain::locatePrecond
**
**  PURPOSE:    Locates the resource precondition support.
**
**  PARAMETERS:
**              uResGuid  - the resource GUID.
**
**  RETURNS:    BOS_TRUE if precondition is in use with this resource.
**              BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeMain::locatePrecond( BOS_UINT32 uResGuid )
{
   CeResBase *pRes = NULL;

   if ( locateRes( uResGuid, pRes ) )
   {
      CeResCall *pResCall = NULL;
      MXD_GNS::MseSdpSession *pMse = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
      {
         pResCall = reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );

         pResCall->resourceGetMse( pMse );
         if ( pMse && pMse->isPreconditionRequired() )
         {
            return BOS_TRUE;
         }
      }
      else
      {
         CCTK_TRACEI1( "Locating precond - Invalid call 0x%lX!", uResGuid );
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateAuthChg
**
**  PURPOSE:    Locates the resource authentication change value.
**
**  PARAMETERS:
**              uResGuid  - the resource GUID.
**
**  RETURNS:    BOS_TRUE if the authentication should be overwritten.
**              BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeMain::locateAuthChg( BOS_UINT32 uResGuid )
{
   CeResBase *pRes = NULL;

   if ( locateRes( uResGuid, pRes ) )
   {
      CeResUser *pResUsr = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         pResUsr = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );

         return pResUsr->resourceGetAuthChg();
      }
      else
      {
         CCTK_TRACEI1( "Locating authentication change - Invalid user 0x%lX!",\
                       uResGuid );
      }
   }

   return BOS_FALSE;
}

/***********************************************************************************
**  FUNCTION:   CeMain::locateKaRes
**
**  PURPOSE:    Locates the resource associated with the keep alive GUID
**              information passed in.
**
**  PARAMETERS:
**              uResGuid - the GUID of the resource looked up.
**              prRes    - pointer (reference) to the found GUID (if applicable).
**
**  RETURNS:    BOS_TRUE  - if resource located.
**              BOS_FALSE - if resource cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, pRes should be ignored.
***********************************************************************************/
BOS_BOOL CeMain::locateKaRes( BOS_UINT32 uResGuid, CeResBase *&prRes )
{
   BOS_UINT32 uIx;

   /* Catch all case if we are trying to locate an invalid resource.
   */
   if ( uResGuid == guCeInvalidGuid )
   {
      prRes = NULL;
      return BOS_FALSE;
   }

   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] &&
           ( mpResUser[uIx]->resourceTmrKeepAliveReco() == uResGuid ) )
      {
         prRes = mpResUser[uIx];
         return BOS_TRUE;
      }
   }

   /* Could not locate the resource with the GUID of interest.  Must not be
   ** a valid one.
   */
   return BOS_FALSE;
}

/***********************************************************************************
**  FUNCTION:   CeMain::locateFailBkRes
**
**  PURPOSE:    Locates the resource associated with the failback to primary GUID
**              information passed in.
**
**  PARAMETERS:
**              uResGuid - the GUID of the resource looked up.
**              prRes    - pointer (reference) to the found GUID (if applicable).
**
**  RETURNS:    BOS_TRUE  - if resource located.
**              BOS_FALSE - if resource cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, pRes should be ignored.
***********************************************************************************/
BOS_BOOL CeMain::locateFailBkRes( BOS_UINT32 uResGuid, CeResBase *&prRes )
{
   BOS_UINT32 uIx;

   /* Catch all case if we are trying to locate an invalid resource.
   */
   if ( uResGuid == guCeInvalidGuid )
   {
      prRes = NULL;
      return BOS_FALSE;
   }

   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] &&
           ( mpResUser[uIx]->resourceTmrFailbkPrimReco() == uResGuid ) )
      {
         prRes = mpResUser[uIx];
         return BOS_TRUE;
      }
   }

   /* Could not locate the resource with the GUID of interest.  Must not be
   ** a valid one.
   */
   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateCallXRtpStat
**
**  PURPOSE:    Locates the X-Rtp-Stat associated with a specific outgoing call
**              resource.
**
**  PARAMETERS:
**              uResGuid  - the GUID of the resource looked up.
**              pStatInfo - x-rtp-stat information to be updated 
**
**  RETURNS:    BOS_TRUE  - if information found.
**              BOS_FALSE - if information cannot be located.
**
**  NOTES:      If BOS_FALSE is returned, pStatInfo should be ignored.
**              Since X-Rtp-Stat is only reported by the originator, 
**              BOS_FALSE is also returned if the call is not outgoing.
***********************************************************************************/
BOS_BOOL CeMain::locateCallXRtpStat( BOS_UINT32 uResGuid,
                                 CCTKXRTPSTAT *prStat )
{
   CeResBase *pRes = NULL;

   if ( locateRes( uResGuid, pRes ) )
   {
      CeCdr *pCdr = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
      {
         BOS_UINT32 uIx;
         for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
         {
            if ( mpResCall[uIx] && ( mpResCall[uIx]->resourceGuid() == uResGuid ))
            {
               pCdr = ceCdrMgr()->locatePrevCdr( mpResCall[uIx]->resourceParent(), uResGuid );
               if ( pCdr && pCdr->cdrMetrics() && pCdr->cdrCnx() )
               {
                  CCTKCALLMETRICS *pMetrics = const_cast<CCTKCALLMETRICS *> (pCdr->cdrMetrics());
                  CeCdr::CECDRCNX *pCnx = pCdr->cdrCnx();
                  memcpy( &(prStat->callMetrics), pMetrics, sizeof( CCTKCALLMETRICS ) );
                  memcpy ( &prStat->ipLcl, &pCnx->ipLcl, sizeof ( BOS_IP_ADDRESS ) );
                  prStat->uPortLcl = pCnx->uPortLcl;
                  prStat->uPtimeLcl = pCnx->uPtimeLcl;
                  memcpy ( &prStat->ipRem, &pCnx->ipRem, sizeof ( BOS_IP_ADDRESS ) );
                  prStat->uPortRem = pCnx->uPortRem;
                  prStat->uPtimeRem = pCnx->uPtimeRem;
                  prStat->tSetupDuration = pCnx->tSetupDuration;
                  prStat->tCallDuration = pCnx->tCallDuration;
                  return BOS_TRUE;
               }
               else
               {
                  return BOS_FALSE;
               }
            }
         }
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::manageAuthChg
**
**  PURPOSE:    Manages the resource authentication change value.
**
**  PARAMETERS:
**              uResGuid  - the resource GUID.
**              bAuthChg  - the value to be set.
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void CeMain::manageAuthChg( BOS_UINT32 uResGuid, BOS_BOOL bAuthChg )
{
   CeResBase *pRes = NULL;

   if ( locateRes( uResGuid, pRes ) )
   {
      CeResUser *pResUsr = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         pResUsr = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );

         pResUsr->resourceSetAuthChg( bAuthChg );
      }
      else
      {
         CCTK_TRACEI1( "Managing authentication change - Invalid user 0x%lX!",\
                       uResGuid );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::getNetHdl
**
**  PURPOSE:    Used to retrieve the network handle associated with a
**              resource.  The network handle corresponds to the UASSP/SCE handle
**              of reference associated with a given Call Engine resource.
**
**  PARAMETERS:
**              uResUid - the resource identifier.
**
**  RETURNS:    Pointer to the network handle if valid, NULL otherwise.
**
**  NOTES:      We are only interested in looking up the network handle (UASSP)
**              handle for a call resource.
***********************************************************************************/
void *CeMain::getNetHdl( BOS_UINT32 uResGuid )
{
   CeResBase *pRes = NULL;

   if ( locateRes( uResGuid, pRes ) )
   {
      CeResCall *pResCall = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
      {
         pResCall = reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );

         return (void *) pResCall->resourceNetHdl();
      }
      else
      {
         return NULL;
      }
   }

   return NULL;
}


/***********************************************************************************
**  FUNCTION:   CeMain::getUsrGuid
**
**  PURPOSE:    Retrieve the user GUID associated with a resource GUID passed in.
**
**  PARAMETERS:
**              uGuid - the resource identifier.
**
**  RETURNS:    The GUID for the user or guCeInvalidGuid if not applicable.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 CeMain::getUsrGuid( BOS_UINT32 uGuid )
{
   CeResBase *pRes = NULL;

   if ( locateRes( uGuid, pRes ) )
   {
      switch ( pRes->resourceGender() )
      {
         case CeResBase::eCERESTYPE_USR:
         {
            return pRes->resourceGuid();
         }
         break;

         case CeResBase::eCERESTYPE_CALL:
         case CeResBase::eCERESTYPE_SUB:
         case CeResBase::eCERESTYPE_NOT:
         {
            return pRes->resourceParent();
         }
         break;

         default:
         break;
      }
   }

   return guCeInvalidGuid;
}


/***********************************************************************************
**  FUNCTION:   CeMain::getResRstrictAddr
**
**  PURPOSE:    Get the restricted address info associated with the resource.
**              Currently only valid for call resources.
**
**  PARAMETERS:
**              uResGuid  - the resource GUID.
**
**  RETURNS:    The restricted addresss info if the resource is valid.
**
**  NOTES:      .
***********************************************************************************/
MXD_GNS::CSocketAddr *CeMain::getResRstrictAddr( BOS_UINT32 uResGuid )
{
   CeResBase *pRes = NULL;

   if ( ( locateRes( uResGuid, pRes ) )&&
        ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL ) )
   {
         CeResCall *pResCall = NULL;
         
         pResCall = reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );
         return pResCall->resourceGetSigRestrictedToAddr();
   }
   
   return NULL;
}


/***********************************************************************************
**  FUNCTION:   CeMain::getResTos
**
**  PURPOSE:    Get the TOS to be associated with the resource.
**
**  PARAMETERS:
**              uResGuid  - the resource GUID.
**
**  RETURNS:    The TOS or 0 if the resource is invalid.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT8 CeMain::getResTos( BOS_UINT32 uResGuid )
{
   BOS_UINT8 uTos = 0;
   BOS_UINT32 uUsrGuid = cfgGetUsrId( uResGuid );

   /* Validate input - may be invalid in some scenario.
   */
   if ( uResGuid == guCeInvalidGuid )
   {
      return 0;
   }

   /* Retrieve the associated user configuration.
   */
   if ( uUsrGuid == guCeInvalidGuid )
   {
      return 0;
   }

   const CFGUSR *pCfgUsr = CFG_USR( uUsrGuid );
   CCTK_ASSERT( pCfgUsr != NULL );
   if ( pCfgUsr == NULL )
   {
      CCTK_TRACEI1( "Getting TOS failed - user config unknown 0x%lX",
                    uResGuid );
      return 0;
   }

   /* Get the default TOS octet value.
   */
   uTos = pCfgUsr->xtl.usr.net.portCfg.tosCfg.uDefault;

   /* If we are dealing with an emergency call, override the TOS octet with
   ** the configured emergency TOS value.
   */
   if ( cfgGetType( uResGuid ) != eCFGTYPE_CALL )
   {
      /* Not a call resource, cannot be an emergency call.
      */
      return uTos;
   }

   const CFGCALL *pCfgCall = CFG_CALL( uResGuid );
   CCTK_ASSERT( pCfgCall != NULL );
   if ( pCfgCall == NULL )
   {
      CCTK_TRACEI1( "Getting TOS failed - call config unknown 0x%lX",
                    uResGuid );
      return 0;
   }

   /* If this is an emergency call, update the TOS value to be set.
   */
   if ( pCfgCall->xtl.basic.gen.emgCallCfg.bEmergency )
   {
      uTos = pCfgUsr->xtl.usr.net.portCfg.tosCfg.uEmgCall;
   }

   return uTos;
}


/***********************************************************************************
**  FUNCTION:   CeMain::locateProfId
**
**  PURPOSE:    Locates the profile information associated with a resource.
**
**  PARAMETERS:
**              uResUid - the resource identifier.
**              puParentUid - the resource parent identifier.
**              puProfIx - the associated profile identifier.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      puParentGuid and puProfIx are being updated as a result of this
**              function call and either may return guCeInvalidGuid in case of
**              error.
***********************************************************************************/
void CeMain::locateProfId( BOS_UINT32 uResGuid, BOS_UINT32 *puParentGuid,
                           BOS_UINT32 *puProfIx )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx] && ( mpResSub[uIx]->resourceGuid() == uResGuid ))
      {
         *puParentGuid = mpResSub[uIx]->resourceParent();
         *puProfIx = mpResSub[uIx]->resourceProfGet();
         return;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::associateLastCall
**
**  PURPOSE:    Associate the last known call to a user resource.
**
**  PARAMETERS:
**              uResGuid - the GUID of the user resource.
**              pCdr     - the last valid CDR for this user.
**              eType    - the type of call (incoming or outgoing).
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void CeMain::associateLastCall( BOS_UINT32 uResGuid, CeCdr *pCdr,
                                CeCdr::CECDRTYPE eType )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] &&
           ( uResGuid != guCeInvalidGuid ) &&
           ( mpResUser[uIx]->resourceGuid() == uResGuid ) )
      {
         /* Set the 'last call'.
         */
         mpResUser[uIx]->resourceSetLastCall( pCdr, eType );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::associateCid
**
**  PURPOSE:    Associate a CID with the CDR for this resource.
**
**  PARAMETERS:
**              uResGuid  - the GUID of the call resource.
**              puCid     - the CID to keep track of.
**              puFromTag - the 'from' tag to keep track of.
**              puToTag   - the 'to' tag to keep track of.
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void CeMain::associateCid( BOS_UINT32 uResGuid,
                           const BOS_UINT8 *puCid,
                           const BOS_UINT8 *puFromTag,
                           const BOS_UINT8 *puToTag )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpResCall[uIx] &&
           ( uResGuid != guCeInvalidGuid ) &&
           ( mpResCall[uIx]->resourceGuid() == uResGuid ) )
      {
         CeCdr *pCdr;

         pCdr = ceCdrMgr()->locateCdr( mpResCall[uIx]->resourceParent(),
                                       mpResCall[uIx]->resourceGuid() );
         if ( pCdr )
         {
            /* Set the CID for this call.
            */
            if ( puCid )
            {
               pCdr->cdrCid( puCid );
            }
            /* Set the From tag for this call.
            */
            if ( puFromTag )
            {
               pCdr->cdrFromTag( puFromTag );
            }
            /* Set the To tag for this call.
            */
            if ( puToTag )
            {
               pCdr->cdrToTag( puToTag );
            }
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::associateStats
**
**  PURPOSE:    Associate the statistics with the CDR for this resource.
**
**  PARAMETERS:
**              uResGuid  - the resource that the CDR should be updated for.
**              pStats - the call statistics of interest.
**
**
**  RETURNS:    BOS_TRUE if the association could be made, BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeMain::associateStats( BOS_UINT32 uResGuid,
                                 CCTKCALLMETRICS *pStats )
{
   BOS_BOOL bRet = BOS_FALSE;
   CeCdr *pCdr;

   pCdr = ceCdrMgr()->locatePrevCdr( pStats->uUserGuid,
                                     uResGuid );
   if ( pCdr )
   {
      pCdr->cdrMetrics( pStats );
      bRet = BOS_TRUE;
   }

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   CeMain::associatedDlg
**
**  PURPOSE:    Determine whether a call resource (dialog) is associated with the
**              requested user resource.
**
**  PARAMETERS:
**              uResGuid - the GUID of the user resource.
**
**  RETURNS:    BOS_TRUE if at least one call resource is associated to the user
**              resource.
**              BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeMain::associatedDlg( BOS_UINT32 uResGuid )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uJx;

   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] &&
           ( uResGuid != guCeInvalidGuid ) &&
           ( mpResUser[uIx]->resourceGuid() == uResGuid ) )
      {
         for ( uJx = 0 ; uJx < CCTKCFG_MAX_CALL ; uJx++ )
         {
            if ( mpResCall[uJx] &&
                 ( mpResCall[uJx]->resourceParent() ==
                              mpResUser[uIx]->resourceGuid() ) )
            {
               /* One match is enough...
               */
               return BOS_TRUE;
            }
         }
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::countDlg
**
**  PURPOSE:    Counts the number of dialog subscription created for a given user
**              resource.
**
**  PARAMETERS:
**              uResGuid - the GUID of the user resource.
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 CeMain::countDlg( BOS_UINT32 uResGuid, BOS_BOOL bOrig )
{
   BOS_UINT32 uResCnt = 0;
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( (mpResSub[uIx] != NULL) &&
           (mpResSub[uIx]->resourceParent() == uResGuid) &&
           (mpResSub[uIx]->resourceType() == eCCTKSUBEVT_DLG) )
      {
         if ( (bOrig && mpResSub[uIx]->resourceOriginator()) ||
              (!bOrig && !mpResSub[uIx]->resourceOriginator()) )
         {
            uResCnt++;
         }
      }
   }

   return uResCnt;
}


/***********************************************************************************
**  FUNCTION:   CeMain::refreshDlg
**
**  PURPOSE:    Checks whether a refresh of the dialog information is necessary
**              based on the new knowledge of the system.  If a refresh is found
**              necessary, issue the event to trigger the refresh.
**
**  PARAMETERS:
**              uResGuid - the GUID of the user resource.
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void CeMain::refreshDlg( BOS_UINT32 uResGuid )
{
   BOS_UINT32 uIx;

   /* Check wether we need to refresh some third party that have
   ** subscribed to some of our event information package.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx] &&
           ( mpResSub[uIx]->resourceParent() == uResGuid ) &&
           ( mpResSub[uIx]->resourceType() == eCCTKSUBEVT_DLG ) &&
           mpResSub[uIx]->resourceNotHdl() &&
           mpResSub[uIx]->resourceNotDlg() )
      {
         BOS_BOOL bDlg = associatedDlg( uResGuid );

         /* If there has been a change in dialog support, notify it.
         */
         if ( bDlg != mpResSub[uIx]->resourceNotDlg()->ceDlgInfo() )
         {
            ceNotify ( mpResSub[uIx]->resourceGuid(),
                       eCEEVT_SUB_USR_NOTIFY,
                       static_cast<void *> (&bDlg) );
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::matchCall
**
**  PURPOSE:    Attempts to match an existing call for the same user which call
**              credentials correspond to the one advertized in the join header
**              (see RFC 3911) information.
**
**  PARAMETERS:
**              uResGuid - the current call resource.
**              uResParent - the current user (parent of the call) resource.
**              puCallCredJoin - the credential to match.
**
**  RETURNS:    The GUID of the associated call resource if found, guCeInvalidGuid
**              otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 CeMain::matchCall( BOS_UINT32 uResGuid,
                              BOS_UINT32 uResParent,
                              const BOS_UINT8 *puCallCredJoin )
{
   BOS_UINT32 uGuid = guCeInvalidGuid;
   BOS_UINT32 uIx;
   BOS_UINT32 uJx;

   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] &&
           ( uResGuid != guCeInvalidGuid ) &&
           ( mpResUser[uIx]->resourceGuid() == uResParent ) )
      {
         for ( uJx = 0 ; uJx < CCTKCFG_MAX_CALL ; uJx++ )
         {
            if ( mpResCall[uJx] &&
                 ( uGuid == guCeInvalidGuid ) &&
                 ( mpResCall[uJx]->resourceParent() ==
                              mpResUser[uIx]->resourceGuid() ) &&
                 ( mpResCall[uJx]->resourceGuid() != guCeInvalidGuid ) &&
                 ( mpResCall[uJx]->resourceGuid() != uResGuid ) )
            {
               CeCdr *pCdr = NULL;
               pCdr = ceCdrMgr()->locateCdr( mpResCall[uJx]->resourceParent(),
                                             mpResCall[uJx]->resourceGuid() );
               if ( pCdr )
               {
                  CCTKCDR *pCdrData = pCdr->cdrData();

                  if ( !strncmp( reinterpret_cast<const char *> (puCallCredJoin),
                                 reinterpret_cast<const char *> (pCdrData->uCallId),
                                 strlen( reinterpret_cast<const char *>
                                                         (pCdrData->uCallId) ) ) )
                  {
                     uGuid = mpResCall[uJx]->resourceGuid();
                  }
               }
            }
         }
      }
   }

   return uGuid;
}


/***********************************************************************************
**  FUNCTION:   CeMain::featInfo
**
**  PURPOSE:    Gets the feature information for this user based on the possible
**              multiple information sources.
**
**  PARAMETERS:
**              uResGuid - the user resource.
**              pFeatInfo - the feature information to be updated.
**
**  RETURNS:    Nothing.
**
**  NOTES:      This consolidates the data from all the user-agent profile
**              subscriptions, each of which could be responsible for setting
**              some of the data.
***********************************************************************************/
void CeMain::featInfo( BOS_UINT32 uResGuid, CCTKFEATINFO *pFeatInfo )
{
   BOS_UINT32 uIx;
   CeResUser *pResUsr = NULL;
   CCTKFEATINFO *pInfo = NULL;

   memset ( static_cast<void *> (pFeatInfo),
            0,
            sizeof( CCTKFEATINFO ) );

   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] &&
           ( uResGuid != guCeInvalidGuid ) &&
           ( mpResUser[uIx]->resourceGuid() == uResGuid ) )
      {
         pResUsr = mpResUser[uIx];
         break;
      }
   }

   if ( pResUsr )
   {
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
      {
         if ( mpResSub[uIx] &&
              ( mpResSub[uIx]->resourceParent() == uResGuid ) &&
              ( mpResSub[uIx]->resourceType() == eCCTKSUBEVT_UAP ))
         {
            pInfo = mpResSub[uIx]->resourceGetFeatInfo();

            /* If any of the element is enabled for this particular subscription
            ** then it is enabled for the user in general.
            */
            pFeatInfo->bCfv |= pInfo->bCfv;
            pFeatInfo->bDnd |= pInfo->bDnd;
            pFeatInfo->bScf |= pInfo->bScf;
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::associateLocInfo
**
**  PURPOSE:    Associates (and creates) the location information that is to
**              be used for a particular call.
**
**  PARAMETERS:
**              uResGuid - the resource that should lookup the information for.
**              puData   - pointer to the data to be formatted.
**              uData    - size of the buffer to write the data to.
**
**
**  RETURNS:    The actual size of the final buffer containing the formatted
**              information.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 CeMain::associateLocInfo( BOS_UINT32 uResGuid,
                                     BOS_UINT8 *puData,
                                     BOS_UINT32 uData )
{
   BOS_UINT32 uRes = 0;
   BOS_UINT32 uIx;

   if ( puData )
   {
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
      {
         if ( mpResCall[uIx] &&
              ( mpResCall[uIx]->resourceGuid() != guCeInvalidGuid ) &&
              ( mpResCall[uIx]->resourceGuid() == uResGuid ) )
         {
            CeLoc ceLoc;

            if ( ceLoc.ceLocWrite( uData,
                                   puData,
                                   mpResCall[uIx]->resourceParent() ) )
            {
               return ceLoc.ceLocSize();
            }
         }
      }
   }

   return uRes;
}


/***********************************************************************************
**  FUNCTION:   CeMain::allocateGuid
**
**  PURPOSE:    Allocates a GUID that is system wide unique.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    The (guaranteed) system wide GUID.
**
**  NOTES:      This not only guarantees the unicity of the GUID system wide, but
**              it also promotes a mechanism such that GUIDs are not going to be
**              re-used by recycled resources anytime soon, which also allows to
**              catch any possible mis-behavior of the upper/lower application.
**
**              If a command or event is received for an 'expired' GUID, we know
**              there has been some resource leakage and we can take appropriate
**              corrective action immediately.
***********************************************************************************/
BOS_UINT32 CeMain::allocateGuid( void )
{
   BOS_BOOL bGuid = BOS_TRUE;
   CeResBase *pRes = NULL;
   BOS_UINT32 uGuid = muGuidSeed;

   /* Acquire the protection mutex.
   */
   if ( bosMutexAcquire( &mGuidMutex ) != BOS_STATUS_OK )
   {
      /* Return an invalid value if one cannot lock the mutex.  The
      ** resource will be invalid.
      */
      CCTK_TRACEI0( "allocateGuid - Failed mutex lock." );
      return guCeInvalidGuid;
   }

   while ( bGuid )
   {
      if ( uGuid < (guCeInvalidGuid - 1))
      {
         uGuid++;
      }
      else
      {
         /* Start again from initial seed possible.
         */
         uGuid = guCeGuidSeed;
      }
      /* Lookup all possible system resources to see if the GUID is already
      ** in use.
      */
      bGuid = locateRes( uGuid, pRes );
   };

   /* Release the protection mutex.
   */
   bosMutexRelease( &mGuidMutex );

   /* GUID identified.
   **   - Return the value.
   **   - Save the new seed to speed up next search.
   */
   muGuidSeed = uGuid;
   CCTK_TRACEI1( "allocateGuid - Allocated: 0x%lX.",
                 uGuid );
   return uGuid;
}


/***********************************************************************************
**  FUNCTION:   CeMain::manageRes
**
**  PURPOSE:    Applies a management action on a resource.  Management actions
**              are driven by the CCTK framework typically following a user
**              application action.
**
**  PARAMETERS:
**              uResGuid - the GUID for the resource of interest.
**              eEvt     - the action to apply.
**              pData    - some supplementary data which may be passed
**                         with the event.
**
**  RETURNS:    BOS_TRUE if the action is successful.  BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeMain::manageRes( BOS_UINT32 uResGuid,
                            CEEVT eEvt,
                            void *pData )
{
   BOS_BOOL bRes = BOS_FALSE;
   BOS_UINT32 uParentGuid = guCeInvalidGuid;

   /* This is the only case when an invalid GUID is accepted.  This is a
   ** for either a global startup, global shutdown, global suspend of new
   ** activity or global resume of all activity.  All of which are invoked
   ** internally by the framework as part of the boot up, shutdown or memory
   ** usage process.
   */
   if ( (uResGuid == guCeInvalidGuid) &&
        ((eEvt == eCEEVT_STARTUP) || (eEvt == eCEEVT_RESET) ||
         (eEvt == eCEEVT_SUSPEND_NEW) || (eEvt == eCEEVT_RESUME_ALL)) )
   {
      switch ( eEvt )
      {
         case eCEEVT_STARTUP:
         {
            return startUp();
         }
         break;

         case eCEEVT_RESET:
         {
            CCTKRSTMODE eRstMode = eCCTKRSTMODE_IMMEDIATE;

            if ( pData )
            {
               eRstMode = *(static_cast<CCTKRSTMODE *> (pData));
            }
            
            mbReset = BOS_TRUE;
            switch ( eRstMode )
            {
               case eCCTKRSTMODE_GRACEFULNONBLOCKING:
               /* Not fully handled at this time, apply same processing as for
               ** the "graceful non blocking" case.
               */
               case eCCTKRSTMODE_GRACEFULBLOCKING:
               {
                  /* First, terminate all the CE resources properly, this
                  ** will ensure proper termination signaling is sent to the
                  ** network for those resources that are still active.
                  */
                  terminateAllRes();

                  /* Second, wait a bit to allow for proper termination of the
                  ** previous step (but do not specifically look for termination
                  ** action) and then continue with shutting down.
                  */
                  tkmTimerStart ( &muTmrRst,
                                  guCeMainRstTmr,
                                  BOS_FALSE,
                                  NULL,
                                  (TKMMSGCB) &CeMain::CeResetTimeout );
               }
               break;

               case eCCTKRSTMODE_IMMEDIATE:
               default:
               {
                  /* Take down any of the stack component to prevent any
                  ** signaling to leak to the network.
                  **
                  ** As a component gets terminated, it will signal the
                  ** termination to CE which will in turn clean up its 
                  ** associated resources for the component.
                  */
                  shutDown ();
               }
               break;
            }

            return BOS_TRUE;
         }
         break;

         case eCEEVT_SUSPEND_NEW:
         {
            mbSuspend = BOS_TRUE;

            return BOS_TRUE;
         }
         break;

         case eCEEVT_RESUME_ALL:
         {
            mbSuspend = BOS_FALSE;

            return BOS_TRUE;
         }
         break;

         default:
         break;
      }
   }

   /* At this point, this is the GUID passed to us by CCTK framework/application,
   ** so it may be invalid, therefore we apply some quick sanity checks.
   */
   if ( (uResGuid == guCeInvalidGuid) || (uResGuid < guCeGuidSeed) )
   {
      return BOS_FALSE;
   }

   /* Apply the correct management action on the resource.
   */
   switch ( eEvt )
   {
      case eCEEVT_USR_ADD:
      {
         /* No matter what we would need to issue a new reset complete notification
         ** under this condition because some resources are being re-created so we
         ** are out of a reset phase.
         */
         mbResetNot = BOS_FALSE;

         bRes =
            (allocateRes( CeResBase::eCERESTYPE_USR,
                          uResGuid, guCeInvalidGuid ) != guCeInvalidGuid);
#if CCTKCFG_REMOTE_USER_CONTROL
         if ( bRes && CFG_USR( uResGuid )->xtl.remUsr.bRemUsr )
         {
            CeResBase *pRes = NULL;
            if ( locateRes( uResGuid, pRes ) )
            {
               CeResUser *pResUser = NULL;
               if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
               {
                  pResUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );
                  pResUser->resourceSetRemUsr ( BOS_TRUE );  
               }
            }
         }
#endif /* CCTKCFG_REMOTE_USER_CONTROL */   
      }
      break;

      /* This event can be re-entrant when invoked from a child.  We need can tell
      ** from the event resource targeted which one it is.
      */
      case eCEEVT_CALL_ADD:
      {
         CeResBase *pRes = NULL;

         if ( locateRes( uResGuid, pRes ) &&
              (pRes->resourceGender() == CeResBase::eCERESTYPE_USR) )
         {
            bRes = BOS_TRUE;
            refreshDlg( pRes->resourceGuid() );
         }
         else
         {
            if ( pData )
            {
               uParentGuid = *( static_cast<const BOS_UINT32 *> ( pData ) );
            }

            bRes =
               (allocateRes( CeResBase::eCERESTYPE_CALL,
                             uResGuid, uParentGuid ) != guCeInvalidGuid);

            if ( bRes )
            {
               CeResBase *pRes = NULL;

               /* Resource creation is successful, set it up accordingly such that
               ** FSM can take over.
               */
               locateRes( uResGuid, pRes );
#if CCTKCFG_REMOTE_USER_CONTROL
               if ( remoteRes ( uParentGuid ))
               {
                  pRes->resourcePushFsm( CeFsmBase::eCEFSMTYPE_PROXY_OUT );
               }
               else
               {
                  pRes->resourcePushFsm( CeFsmBase::eCEFSMTYPE_OUT );
               }
#else
               pRes->resourcePushFsm( CeFsmBase::eCEFSMTYPE_OUT );
#endif
               pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_CALLOUT_IDLE );
            }
         }
      }
      break;

      case eCEEVT_SUB_ADD:
      {
         const TKMSUBINFO *pSubInfo = NULL;

         if ( pData )
         {
            pSubInfo = static_cast<const TKMSUBINFO *> ( pData );

            bRes =
               (allocateRes( CeResBase::eCERESTYPE_SUB,
                             uResGuid,
                             pSubInfo->uGuid,
                             BOS_TRUE,
                             pSubInfo->eSubType ) != guCeInvalidGuid);
         }

         if ( bRes )
         {
            CeResBase *pRes = NULL;

            /* Resource creation is successful, set it up accordingly such that
            ** FSM can take over.
            */
            locateRes( uResGuid, pRes );
            pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_IDLE );
         }
      }
      break;

      case eCEEVT_NOT_ADD:
      {
         if ( pData )
         {
            uParentGuid = *( static_cast<const BOS_UINT32 *> ( pData ) );
         }

         bRes =
            (allocateRes( CeResBase::eCERESTYPE_NOT,
                          uResGuid, uParentGuid ) != guCeInvalidGuid);
      }
      break;

      case eCEEVT_USR_REM:
      {
         CeResBase *pRes = NULL;

         if ( locateRes( uResGuid, pRes ) )
         {
            CeResUser *pResUser = NULL;

            bRes = BOS_TRUE;
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
            {
               pResUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );

               /* Inform all children of the termination of the parent.
               ** All children will also be terminated.
               */
               fsmDistributeChild( pResUser->resourceGuid(),
                                   eCEEVT_REG_USR_ENDED );

               pResUser->resourceTerminate();
            }
            else
            {
               CeFsmBase::fsmError( *pRes );
            }
         }
      }
      break;

      /* This event can be re-entrant when invoked from a child.  We need can tell
      ** from the event resource targeted which one it is.
      */
      case eCEEVT_CALL_REM:
      {
         CeResBase *pRes = NULL;

         if ( locateRes( uResGuid, pRes ) )
         {
            switch ( pRes->resourceGender() )
            {
               case CeResBase::eCERESTYPE_CALL:
               {
                  CeResCall *pResCall = NULL;

                  bRes = BOS_TRUE;

                  pResCall = reinterpret_cast<CeResCall *>
                     ( pRes->resourceAddress() );

                  /* Inform parent that this resource is being terminated.
                  */
                  fsmInvokeParent( *pRes,
                                   eCEEVT_CALL_REM );

                  pResCall->resourceTerminate();
               }
               break;

               case CeResBase::eCERESTYPE_USR:
               {
                  bRes = BOS_TRUE;
                  refreshDlg( pRes->resourceGuid() );
               }
               break;

               default:
               break;
            }
         }
      }
      break;

      /* This event can be re-entrant when invoked from a child.  We need can tell
      ** from the event resource targeted which one it is.
      */
      case eCEEVT_SUB_REM:
      {
         CeResBase *pRes = NULL;

         if ( locateRes( uResGuid, pRes ) )
         {
            switch ( pRes->resourceGender() )
            {
               case CeResBase::eCERESTYPE_SUB:
               {
                  CeResSub *pResSub = NULL;

                  bRes = BOS_TRUE;

                  pResSub = reinterpret_cast<CeResSub *>
                     ( pRes->resourceAddress() );

                  /* Inform parent that this resource is being terminated.
                  */
                  fsmInvokeParent( *pRes,
                                   eCEEVT_SUB_REM );

                  pResSub->resourceTerminate();
               }
               break;

               case CeResBase::eCERESTYPE_USR:
               {
                  CeResUser *pResUser = NULL;

                  bRes = BOS_TRUE;

                  pResUser = reinterpret_cast<CeResUser *>
                     ( pRes->resourceAddress() );

                  /* Check if we lost our MWI subscription resource.  If so,
                  ** remove its knowledge from the user resource.
                  */
                  if ( pResUser->resourceGetMwiSub() ==
                                 static_cast<const CeResBase *> (pData) )
                  {
                     pResUser->resourceSetMwiSub( NULL );
                  }
               }
               break;

               default:
               break;
            }
         }
      }
      break;

      /* This event can be re-entrant when invoked from a child.  We need can tell
      ** from the event resource targeted which one it is.
      */
      case eCEEVT_NOT_REM:
      {
         CeResBase *pRes = NULL;

         if ( locateRes( uResGuid, pRes ) )
         {
            switch ( pRes->resourceGender() )
            {
               case CeResBase::eCERESTYPE_NOT:
               {
                  CeResNot *pResNot = NULL;

                  bRes = BOS_TRUE;

                  pResNot = reinterpret_cast<CeResNot *>
                     ( pRes->resourceAddress() );

                  /* Inform parent that this resource is being terminated.
                  */
                  fsmInvokeParent( *pRes,
                                   eCEEVT_NOT_REM );

                  pResNot->resourceTerminate();
               }
               break;

               default:
               break;
            }
         }
      }
      break;

      default:
      {
         bRes = BOS_FALSE;
      }
      break;
   }

   /* If this is a management event and we are reseting the call engine,
   ** check for outstanding 'alive' resources.  If none is present any longer,
   ** we notify the framework that we are done.
   */
   if ( bRes && mbReset &&
        ( (eEvt == eCEEVT_USR_REM) || (eEvt == eCEEVT_CALL_REM)
          || (eEvt == eCEEVT_SUB_REM) || (eEvt == eCEEVT_NOT_REM) ))
   {
      checkFinalRes();
   }

   return bRes;
}


/***********************************************************************************
**  FUNCTION:   CeMain::checkFinalRes
**
**  PURPOSE:    Checks whether we still have some resource allocated.
**
**  PARAMETERS:
**              bIssueEvent - whether to issue an event to the upper level
**                            application or not.
**
**  RETURNS:    BOS_TRUE if at least one resource is active, BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeMain::checkFinalRes( BOS_BOOL bIssueEvt )
{
   BOS_UINT32 uIx;
   BOS_BOOL bResActive = BOS_FALSE;

   /* Loop through all the resources and issue the 'termination' event to each.
   ** The resource will take care of the actual work.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx]->resourceGuid() != guCeInvalidGuid )
      {
         bResActive = BOS_TRUE;
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpResCall[uIx]->resourceGuid() != guCeInvalidGuid )
      {
         bResActive = BOS_TRUE;
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx]->resourceGuid() != guCeInvalidGuid )
      {
         bResActive = BOS_TRUE;
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
   {
      if ( mpResNot[uIx]->resourceGuid() != guCeInvalidGuid )
      {
         bResActive = BOS_TRUE;
      }
   }


   /* Notify the framework of the status of the reset in case it is expecting it.
   ** When there are no longer active resources, we assume reset is complete.
   */
   if ( !bResActive && mbReset && bIssueEvt && !mbResetNot )
   {
      mbReset = BOS_FALSE;
      mbResetNot = BOS_TRUE;

      tkmEvent( guCeInvalidGuid,
                guCeInvalidGuid,
                eCCTKEVT_RESET_COMPLETE,
                NULL );
   }

   return bResActive;
}

/***********************************************************************************
**  FUNCTION:   CeMain::checkCallRes
**
**  PURPOSE:    Checks to see if the parent resource has any current call resources.
**
**  PARAMETERS:
**              uParentGuid  - the user resource to lookup
**
**  RETURNS:    BOS_TRUE if the parent has any call resources, BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeMain::checkCallRes( BOS_UINT32 uParentGuid )
{
   CeResBase *pRes;
   BOS_UINT32 uIx;
   
   if ( locateRes( uParentGuid, pRes ) )
   {
      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {   
         for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
         {
            if ( (mpResCall[uIx]->resourceGuid() != guCeInvalidGuid) &&
                 (mpResCall[uIx]->resourceParent() == uParentGuid ) )
            {
               return BOS_TRUE;
            }
         }
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::terminateAllRes
**
**  PURPOSE:    Terminates all active resource by forcing termination of
**              their processing, which may include:
**
**                  - Terminating active call.
**                  - Shutting down registration.
**                  - Freeing subscription and outsanding notification.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void CeMain::terminateAllRes( void )
{
   BOS_UINT32 uIx;

   /* Loop through all the resources and issue the 'termination' event to each.
   ** The resource will take care of the actual work.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
   {
      if ( mpResNot[uIx] &&
           (mpResNot[uIx]->resourceGuid() != guCeInvalidGuid) )
      {
         fsmProcessEvt( mpResNot[uIx]->resourceGuid(),
                        eCEEVT_NOT_USR_ENDED );
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx] &&
           (mpResSub[uIx]->resourceGuid() != guCeInvalidGuid) )
      {
         fsmProcessEvt( mpResSub[uIx]->resourceGuid(),
                        eCEEVT_SUB_USR_ENDED );
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpResCall[uIx] &&
           (mpResCall[uIx]->resourceGuid() != guCeInvalidGuid) )
      {
         /* Ensure that all calls are in the correct state for termination.
         **
         ** Since this is a 'forced' ending, we do not care about doing things
         ** with grace, we just terminate all as quickly and efficiently as
         ** possible.
         */
         if ( mpResCall[uIx]->resourceFsm() != CeFsmBase::eCEFSMTYPE_CALL )
         {
            mpResCall[uIx]->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
         }

         mpResCall[uIx]->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
         fsmProcessEvt( mpResCall[uIx]->resourceGuid(),
                        eCEEVT_CALL_USR_ENDED );
      }
   }

   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx] )
      {
         if ( mpResUser[uIx]->resourceGuid() != guCeInvalidGuid )
         {
            fsmProcessEvt( mpResUser[uIx]->resourceGuid(),
                           eCEEVT_REG_USR_ENDED );
         }
         else
         {
            mpResUser[uIx]->resourceLiberate();
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::allocateRes
**
**  PURPOSE:    Allocates a resource of a given type from the resource pool.
**
**  PARAMETERS:
**              eType - the type of resource to be allocated.
**              uResGuid - the GUID for the resource.
**              uParentGuid - the GUID of the parent resource.
**              bCreate - whether to create the physical resource or not.
**              uSubType - certain resource need a sub-type to further qualify
**                         them.
**
**
**  RETURNS:    The GUID of the allocated resource, or gCeInvalidGuid if one
**              resource cannot be allocated.
**
**  NOTES:      The Call Engine applies rules for assigning resources.  Once a
**              resource is being allocated, its GUID and parent GUID are set
**              accordingly and the resource is linked to its default FSM.
**
**              The usage of the 'bCreate' flag (default override set to BOS_TRUE)
**              is to allow creation of Call Engine resource without creating the
**              associated physical UASSP/SCE resource.  This is used for example
**              when handling new incoming calls or subscriptions.
***********************************************************************************/
BOS_UINT32 CeMain::allocateRes( CeResBase::CERESTYPE eType,
                                BOS_UINT32 uResGuid,
                                BOS_UINT32 uParentGuid,
                                BOS_BOOL bCreate,
                                BOS_UINT32 uSubType )
{
   BOS_UINT32 uGuid = guCeInvalidGuid;
   BOS_UINT32 uIx;
   BOS_UINT32 uResCnt;
   BOS_UINT32 uSelectIx;

   /* If we are suspended, we do not allow any new resource to be created.
   ** We however allow current resource to continue working as intended.
   */
   if ( mbSuspend )
   {
      CCTK_TRACED2( "Allocate Resource %d, 0x%lX - Reject suspended.",
                    eType, uResGuid );
      return uGuid;
   }

   switch ( eType )
   {
      case CeResBase::eCERESTYPE_USR:
      {
         BOS_BOOL bCreated;

         /* To locate a free user resource, we need to insure that its
         ** parent GUID is invalid.  If so, the user was not allocated
         ** to be used so far and can be claimed.
         **
         ** Note that a user resource parent GUID is the original seed.
         */
         for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
         {
            /* Assume default always successful.
            */
            bCreated = BOS_TRUE;

            if ( (mpResUser[uIx] != NULL) &&
                 (mpResUser[uIx]->resourceParent() == guCeInvalidGuid) &&
                 ((bCreate &&
                      ((bCreated =
                           mpResUser[uIx]->resourceCreate( uResGuid ))
                                                               == BOS_TRUE)) ||
                  !bCreate) )
            {
               mpResUser[uIx]->resourcePushFsm( CeFsmBase::eCEFSMTYPE_REG );
               mpResUser[uIx]->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_IDLE );
               uGuid = mpResUser[uIx]->resourceGuid();
               break;
            }

            /* If a creation attempt took place and failed, we need to clean up
            ** the resource GUID reference to free up the existing resource block
            ** in case we are trying to re-use it later on.
            */
            if ( !bCreated )
            {
               mpResUser[uIx]->resourceAllocated( guCeInvalidGuid, guCeInvalidGuid );
            }
         }
      }
      break;

      /* To locate a free resource for any of those types below, we need
      ** to not only locate a resource which parent GUID is invalid, one also
      ** need to ensure that we are not attempting to allocate more resource
      ** than allowed to for a given user since all of those resources are
      ** user dependent and have some limits.
      */
      case CeResBase::eCERESTYPE_CALL:
      {
         uResCnt = 0;
         uSelectIx = guCeInvalidGuid;
         for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
         {
            if ( mpResCall[uIx] != NULL )
            {
               if ( (mpResCall[uIx]->resourceParent() == guCeInvalidGuid) &&
                    (uSelectIx == guCeInvalidGuid) )
               {
                  uSelectIx = uIx;
               }
               else if ( mpResCall[uIx]->resourceParent() == uParentGuid )
               {
                  uResCnt++;
               }
            }
         }

         if ( (uSelectIx != guCeInvalidGuid) && (uResCnt < guCeMainMaxCallPerUser)
              && ((bCreate && mpResCall[uSelectIx]->resourceCreate( uResGuid ))
                  || !bCreate) )
         {
            mpResCall[uSelectIx]->resourceAllocated( uResGuid, uParentGuid );
            mpResCall[uSelectIx]->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
            uGuid = mpResCall[uSelectIx]->resourceGuid();
         }
      }
      break;

      case CeResBase::eCERESTYPE_SUB:
      {
         BOS_UINT32 uDlgCnt = 0;
         uResCnt = 0;
         uSelectIx = guCeInvalidGuid;
         for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
         {
            if ( mpResSub[uIx] != NULL )
            {
               if ( (mpResSub[uIx]->resourceParent() == guCeInvalidGuid) &&
                    (uSelectIx == guCeInvalidGuid) )
               {
                  uSelectIx = uIx;
               }
               else if ( mpResSub[uIx]->resourceParent() == uParentGuid )
               {
                  uResCnt++;
                  /* Count the number of specific resources of a certain type
                  ** that may have been allocated already.
                  */
                  if ( (static_cast<CCTKSUBEVT> (uSubType) == eCCTKSUBEVT_DLG) &&
                       (mpResSub[uIx]->resourceType() == eCCTKSUBEVT_DLG) )
                  {
                     if ( (bCreate && mpResSub[uIx]->resourceOriginator()) ||
                          (!bCreate && !mpResSub[uIx]->resourceOriginator()) )
                     {
                        uDlgCnt++;
                     }
                  }
               }
            }
         }

         /* Ensure we have not reached our configured maximum allowed for this
         ** resource type.
         */
         if ( static_cast<CCTKSUBEVT> (uSubType) == eCCTKSUBEVT_DLG )
         {
            const CFGUSR *pCfgUsr = CFG_USR( uParentGuid );
            if ( pCfgUsr )
            {
               if ( (bCreate &&
                        (uDlgCnt >= pCfgUsr->xtl.usr.net.diagEvt.uSuberMax)) ||
                    (!bCreate &&
                        (uDlgCnt >= pCfgUsr->xtl.usr.net.diagEvt.uSubeeMax)) )

               {
                  /* Do not allow more subscription.
                  */
                  uSelectIx = guCeInvalidGuid;
               }
            }
         }

         if ( (uSelectIx != guCeInvalidGuid) && (uResCnt < guCeMainMaxSubsPerUser)
              && ((bCreate && mpResSub[uSelectIx]->resourceCreate(
                       (((static_cast<CCTKSUBEVT> (uSubType) == eCCTKSUBEVT_MWI) ||
                         (static_cast<CCTKSUBEVT> (uSubType) == eCCTKSUBEVT_BMWI)) ?
                                                          uParentGuid : uResGuid ),
                                 static_cast<CCTKSUBEVT> (uSubType)))
                  || !bCreate) )
         {
            mpResSub[uSelectIx]->resourceAllocated( uResGuid, uParentGuid );
            mpResSub[uSelectIx]->resourcePushFsm( CeFsmBase::eCEFSMTYPE_SUB );
            uGuid = mpResSub[uSelectIx]->resourceGuid();
         }
      }
      break;

      case CeResBase::eCERESTYPE_NOT:
      {
         uResCnt = 0;
         uSelectIx = guCeInvalidGuid;
         for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
         {
            if ( mpResNot[uIx] != NULL )
            {
               if ( (mpResNot[uIx]->resourceParent() == guCeInvalidGuid) &&
                    (uSelectIx == guCeInvalidGuid) )
               {
                  uSelectIx = uIx;
               }
               else if ( mpResNot[uIx]->resourceParent() == uParentGuid )
               {
                  uResCnt++;
               }
            }
         }

         /* Note that notifier resources are associated with the parent (user)
         ** configuration.
         */
         if ( (uSelectIx != guCeInvalidGuid) && (uResCnt < guCeMainMaxNotsPerUser)
              && ((bCreate && mpResNot[uSelectIx]->resourceCreate( uParentGuid,
                                          static_cast<CCTKPUBLISH> (uSubType) ))
                  || !bCreate) )
         {
            mpResNot[uSelectIx]->resourceAllocated( uResGuid, uParentGuid );
            mpResNot[uSelectIx]->resourcePushFsm( CeFsmBase::eCEFSMTYPE_PUB );
            mpResNot[uSelectIx]->resourcePushState( CeFsmBase::eCEFSMSTATE_PUB_IDLE );
            uGuid = mpResNot[uSelectIx]->resourceGuid();
         }
      }
      break;

      default:
         /* Invalid.
         */
      break;
   }

   return uGuid;
}



/***********************************************************************************
**  FUNCTION:   CeMain::allocateReferrer
**
**  PURPOSE:    Allocates a referrer towards a user resource as a placeholder for
**              handling out of dialog transfer.
**
**  PARAMETERS:
**              uResGuid - the GUID for the user resource.
**              pReferrer - the reference to the referrer resource of interest.
**
**
**  RETURNS:    BOS_TRUE if the referrer could successfully be associated.
**              BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeMain::allocateReferrer( BOS_UINT32 uResGuid,
                                   MXD_GNS::ISceReferrer* pReferrer )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( ( mpResUser[uIx] != NULL ) &&
           ( mpResUser[uIx]->resourceGuid() == uResGuid ) )
      {
         if ( mpResUser[uIx]->resourceSetReferrer( pReferrer ) )
         {
            const CFGUSR *pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );

            /* Accepting this referrer, we need to keep track of it by adding
            ** reference count and we need to setup the proper configuration.
            */
            pReferrer->AddIfRef();
            pReferrer->SetConfiguration( pCfgUsr->in.pSceUsrCfg );
            /* Note: We do not have access the to transfer configuration set
            **       from the CFG_USR config at this time, so we hardcode the
            **       behavior here.
            */
            pReferrer->SetInvalidTargetDialogReferBehaviour( 
                              MXD_GNS::ISceReferrer::eREJECT_WITH_481 );

            return BOS_TRUE;
         }
      }
   }

   /* Could not accept the referrer, the attempt will be rejected by the
   ** SCE framework.
   */
   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeMain::reparentCallRes
**
**  PURPOSE:    Execute with caution - updates all calls assosicated to a specifed 
**              parent to a new one. If required will also ensure that any calls 
**              that move to a new parent are signalling restricted to the old
**              parent's active target.
**              
**
**  PARAMETERS:
**              uFromParentGuid - the GUID for the user resource to move calls from.
**              uToParentGuid - the GUID for the user resource to move calls to.
**              bRstrictSig - Whether to restrict call signalling to current user's
**                            active target.
**
**
**  RETURNS:    BOS_TRUE if the calls could successfully be reparented.
**              BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeMain::reparentCallRes( BOS_UINT32 uFromParentGuid,
                                  BOS_UINT32 uToParentGuid,
                                  BOS_BOOL bRstrictSig)
{
   
   CeResBase *pResFrom = NULL;
   CeResBase *pResTo = NULL;
   BOS_BOOL bSuccess = BOS_FALSE;
   BOS_UINT32 uIx;

   /* First make sure that both resources are infact valid parent resources.
   */
   if ( ( uFromParentGuid != uToParentGuid ) &&
        ( locateRes( uFromParentGuid, pResFrom )) && 
        ( locateRes( uToParentGuid, pResTo )))
   {
      if ( pResFrom->resourceGender() == CeResBase::eCERESTYPE_USR &&
           pResTo->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         /* Find the calls assosciated to the parent to move from
         ** and apply signalling restrictions if needed.
         */
         for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
         {
            if ( mpResCall[uIx] &&
                 ( mpResCall[uIx]->resourceGuid() != guCeInvalidGuid ) &&
                  ( uFromParentGuid == mpResCall[uIx]->resourceParent()) )
            {
               
               mpResCall[uIx]->resourceAllocated( mpResCall[uIx]->resourceGuid(), uToParentGuid );
               
               CCTK_TRACED3( "reparentCallRes: 0x%lX from 0x%lX to 0x%lX",  
                  mpResCall[uIx]->resourceGuid(), uFromParentGuid, mpResCall[uIx]->resourceParent() );
              
              /* If the call doesn't have any prior signalling restrictions, then make sure
               ** that subsequent signalling will be maintained to the current registered target for
               ** old user.
               */
               MXD_GNS::CSocketAddr actAddr;
               if ( bRstrictSig && !mpResCall[uIx]->resourceIsSigRestrictedToAddr() )
               {
                  if ( nsBrGetActiveTgt( uFromParentGuid, actAddr ) )
                  {
                     CCTK_TRACED2( "reparentCallRes: restricting call 0x%lX to active target of 0x%lX ", 
                                      mpResCall[uIx]->resourceGuid(), uFromParentGuid );
                                      
                     mpResCall[uIx]->resourceSetSigRestrictedToAddr( BOS_TRUE, &actAddr );
                  }
               }
               /* If the call already has signalling restrictions remove them now if
               ** the active target for the new parent is the address the call is
               ** restricted to.
               */
               else if ( mpResCall[uIx]->resourceIsSigRestrictedToAddr() )
               {
                  MXD_GNS::CSocketAddr *pCurrRstrictAddr = mpResCall[uIx]->resourceGetSigRestrictedToAddr();
                  if ( nsBrGetActiveTgt( uToParentGuid, actAddr ) && 
                       actAddr == *pCurrRstrictAddr)
                  {
                     CCTK_TRACED1( "reparentCallRes: removing restriction on 0x%lX", 
                                       mpResCall[uIx]->resourceGuid() );
                                       
                     mpResCall[uIx]->resourceSetSigRestrictedToAddr( BOS_FALSE, NULL );
                  }
               }
               
               /* Update the user for the call configuration.
               */
               cfgUpdCallUsr( mpResCall[uIx]->resourceGuid(), uFromParentGuid, uToParentGuid );
               
               bSuccess = BOS_TRUE;
            }
         }
      }
   }
   
   return bSuccess;
}


/***********************************************************************************
**  FUNCTION:   CeMain::fsmProcessEvt
**
**  PURPOSE:    Process an event for a resource.
**
**  PARAMETERS:
**              uResGuid - the resource GUID to associate to this event.
**              eEvt     - the event to run for the resource FSM.
**              pData    - some supplementary data which may be passed
**                         with the event.
**
**  RETURNS:    Status as defined in the CCTKSTATUS enum.
**
**  NOTES:      'pData' is NULL unless otherwise specified.  Its usage is to
**              convey information that requires to be saved by a resource
**              typically in order to be able to properly process the event or
**              in prevision of subsequent event(s).
***********************************************************************************/
CCTKSTATUS CeMain::fsmProcessEvt( BOS_UINT32 uResGuid, CEEVT eEvt,
                                  void *pData )
{
   CeResBase *pRes = NULL;

   /* Intercept special events which have to do with resource
   ** management from the framework (addition or removal of resources).
   */
   if ( manageRes( uResGuid, eEvt, pData ) )
   {
      CCTK_TRACED2( "Process Event - Managing resource 0x%lX, %d.",
                    uResGuid, eEvt );
      return eCCTKSTATUS_SUCCESS;
   }
   /* This must be a non management event, the resource must be valid.
   */
   else if ( locateRes( uResGuid, pRes ))
   {
      if ( pRes->resourceFsm() < CeFsmBase::eCEFSMTYPE_MAX )
      {
         /* Push the event to the resource.
         */
         pRes->resourcePushEvent( eEvt );

         if ( pData )
         {
            /* Push the data to the resource if necessary.  Some data
            ** may be required in order to properly process the event,
            ** or some data simply needs to be saved to the resource for
            ** correct processing.
            */
            pRes->resourcePushData( eEvt, pData );
         }

         /* Run the FSM for this resource.
         */
         mpFsm[pRes->resourceFsm()]->fsmRun( *pRes );
         return eCCTKSTATUS_SUCCESS;
      }
      else
      {
         /* This can happen when a management event (for example) is sent to
         ** a resource that is in fact not allocated (such as a user initiated
         ** reset), in which case, there is nothing to do.
         */
         if ( pRes->resourceGuid() == guCeInvalidGuid )
         {
            return eCCTKSTATUS_SUCCESS;
         }
         /* Major problem.  Invalid FSM for this resource.
         */
         else
         {
            CCTK_TRACEI2( "Process Event - Invalid FSM %d for resource 0x%lX.",
                          pRes->resourceFsm(), pRes->resourceGuid() );
            return eCCTKSTATUS_BADSTATE;
         }
      }
   }
   /* None of the above...  We either cannot manage of cannot locate the
   ** resource.  It could be because the resource was freed up or was never
   ** properly created yet the application is trying to use it.
   **
   ** We inform back the external application of such situation by issuing
   ** the appropriate termination behavior, hopefully the application will
   ** understand that the resource is BAD and cannot be used again.
   */
   else
   {
      fsmTerminated( uResGuid );
      return eCCTKSTATUS_BADSTATE;
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::fsmTerminated
**
**  PURPOSE:    Signify to the outside application that this handle is terminated.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::fsmTerminated( BOS_UINT32 uResGuid )
{
   /* Invoke termination callback.
   */
   (void) uResGuid;

   CCTK_TRACEI1( "Resource (0x%lX) terminated.",
                 uResGuid );
}


/***********************************************************************************
**  FUNCTION:   CeMain::fsmTimer
**
**  PURPOSE:    Start/stop a timer on bahalf of a resource.
**
**  PARAMETERS:
**              rRes     - the resource.
**              uTimerMs - the time (in milliseconds) prior to timer expiry.
**
**  RETURNS:    None
**
**  NOTES:      If the time is set to 0, this has for effect to actually cancel
**              the timer that may be running for this resource.
***********************************************************************************/
void CeMain::fsmTimer( CeResBase &rRes, BOS_UINT32 uTimeMs )
{
   /* Timer hook-up.
   */
   (void) uTimeMs;
   (void) rRes;
}


/***********************************************************************************
**  FUNCTION:   CeMain::fsmProcessFsm
**
**  PURPOSE:    Update the FSM associated with a resource.
**
**  PARAMETERS:
**              rRes     - the resource.
**              eType    - the new FSM type to be associated with the resource.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::fsmProcessFsm( CeResBase &rRes,
                            CeFsmBase::CEFSMTYPE eType )
{
   if ( (eType < CeFsmBase::eCEFSMTYPE_MAX) )
   {
      /* Update the FSM for this resource.
      */
      rRes.resourcePushFsm( eType );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::fsmProcessState
**
**  PURPOSE:    Update the state of a resource.
**
**  PARAMETERS:
**              rRes     - the resource.
**              eState   - the new state to be associated with the resource.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::fsmProcessState( CeResBase &rRes,
                              CeFsmBase::CEFSMSTATE eState )
{
   /* Update the state for this resource.
   */
   rRes.resourcePushState( eState );
}


/***********************************************************************************
**  FUNCTION:   CeMain::fsmInvokeParent
**
**  PURPOSE:    Invoke the parent of this resource with an event generated by the
**              child.
**
**  PARAMETERS:
**              rRes     - the child resource.
**              eEvt     - the event to be passed to the parent.
**
**  RETURNS:    None
**
**  NOTES:      It is the responsibility of the parent to decide whether the event
**              from the child is of any interest to it.
***********************************************************************************/
void CeMain::fsmInvokeParent( CeResBase &rRes,
                              CEEVT eEvt )
{
   /* Invoke the event processing on the parent, but only if
   ** the resource is not a top level resource (with no parent).
   */
   if ( rRes.resourceParent() != guCeGuidSeed )
   {
      fsmProcessEvt( rRes.resourceParent(),
                     eEvt,
                     static_cast<void *> (&rRes) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::fsmDistributeChild
**
**  PURPOSE:    Distribute an event to all the children from this parent.
**
**  PARAMETERS:
**              uParentGuid     - the parent GUID.
**              eEvt            - the event to be passed to the child.
**
**  RETURNS:    None
**
**  NOTES:      The child will take the event from the parent and may do
**              something with it.
***********************************************************************************/
void CeMain::fsmDistributeChild( BOS_UINT32 uParentGuid,
                                 CEEVT eEvt )
{
   BOS_UINT32 uIx;
   CEEVT eMapEvt;

   /* Distribute the event to each child which parent is identified
   ** with the parent GUID.
   **
   ** Note that this cannot be applied to a user resource since users are the
   ** top most resources and do not share child(s).
   */
   if ( uParentGuid != guCeGuidSeed )
   {
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
      {
         if ( mpResCall[uIx] && (mpResCall[uIx]->resourceParent() == uParentGuid))
         {
            fsmMapEvent( mpResCall[uIx]->resourceFsm(), eEvt, eMapEvt );
            fsmProcessEvt( mpResCall[uIx]->resourceGuid(), eMapEvt );
         }
      }

      for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
      {
         if ( mpResSub[uIx] && (mpResSub[uIx]->resourceParent() == uParentGuid))
         {
            fsmMapEvent( mpResSub[uIx]->resourceFsm(), eEvt, eMapEvt );
            fsmProcessEvt( mpResSub[uIx]->resourceGuid(), eMapEvt );
         }
      }

      for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
      {
         if ( mpResNot[uIx] && (mpResNot[uIx]->resourceParent() == uParentGuid))
         {
            fsmMapEvent( mpResNot[uIx]->resourceFsm(), eEvt, eMapEvt );
            fsmProcessEvt( mpResNot[uIx]->resourceGuid(), eMapEvt );
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::fsmMapEvent
**
**  PURPOSE:    Mapping function to map events from external source to internal
**              value.
**
**  PARAMETERS:
**              eType    - the type of mapping invoked.
**              eEvtIn   - the event to be mapped.
**              reEvtOut - the event mapped.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::fsmMapEvent( CeFsmBase::CEFSMTYPE eType,
                          BOS_UINT32 uEvtIn,
                          CEEVT &reEvtOut )
{
   CEMAINEVTMAP *pEvtMap = NULL;

   reEvtOut = eCEEVT_INV;

   /* Select the proper map to locate the event.
   */
   switch ( eType )
   {
      case CeFsmBase::eCEFSMTYPE_REG:
      {
         pEvtMap = gRegMgrEvtMap;
      }
      break;

      case CeFsmBase::eCEFSMTYPE_IN:
      case CeFsmBase::eCEFSMTYPE_OUT:
      case CeFsmBase::eCEFSMTYPE_CALL:
#if CCTKCFG_REMOTE_USER_CONTROL
      case CeFsmBase::eCEFSMTYPE_PROXY_IN:
      case CeFsmBase::eCEFSMTYPE_PROXY_OUT:
#endif
      {
         pEvtMap = gCallMgrEvtMap;
      }
      break;

      case CeFsmBase::eCEFSMTYPE_XFER:
      {
         pEvtMap = gXferMgrEvtMap;
      }
      break;

      case CeFsmBase::eCEFSMTYPE_SUB:
      {
         pEvtMap = gSubMgrEvtMap;
      }
      break;

      case CeFsmBase::eCEFSMTYPE_HOLD:
      {
         pEvtMap = gHoldMgrEvtMap;
      }
      break;

      case CeFsmBase::eCEFSMTYPE_PUB:
      {
         pEvtMap = gPubMgrEvtMap;
      }
      break;

      default:
      {
         eType = CeFsmBase::eCEFSMTYPE_MAX;
      }
      break;
   }

   /* Lookup the mapped event.
   */
   if ( pEvtMap != NULL )
   {
      while ( (pEvtMap->eEvt != eCEEVT_DNP) &&
              (pEvtMap->uEvt != uEvtIn) )
      {
         pEvtMap++;
      }

      reEvtOut = pEvtMap->eEvt;
   }

   CCTK_TRACEI4( "Map Event - FSM %s - In: 0x%lX, Out: 0x%x (%s)",
                 gpcCeFsmName[eType],
                 uEvtIn,
                 reEvtOut,
                 gpcCeFsmEvent[static_cast<BOS_UINT32>(reEvtOut) - eCEEVT_INV] );
}


/***********************************************************************************
**  FUNCTION:   CeMain::fsmMapCctkEvent
**
**  PURPOSE:    Mapping function to map events from CCTK internal source to Call
**              Engine internal processing value.
**
**  PARAMETERS:
**              eType    - the type of mapping invoked.
**              eEvtIn   - the event to be mapped.
**              reEvtOut - the event mapped.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::fsmMapCctkEvent( CeFsmBase::CEFSMTYPE eType,
                              BOS_UINT32 uEvtIn,
                              CEEVT &reEvtOut )
{
   CEMAINEVTMAP *pEvtMap = NULL;

   reEvtOut = eCEEVT_INV;

   /* Select the proper map to locate the event.  Note that only
   ** registration FSM is of interest because that is the one corresponding
   ** to the user resource and only that resource has an interest in the
   ** internal CCTK event processing at this time.
   */
   switch ( eType )
   {
      case CeFsmBase::eCEFSMTYPE_REG:
      {
         pEvtMap = gResUsrCctkEvtMap;
      }
      break;

      case CeFsmBase::eCEFSMTYPE_IN:
      case CeFsmBase::eCEFSMTYPE_OUT:
      case CeFsmBase::eCEFSMTYPE_CALL:
      case CeFsmBase::eCEFSMTYPE_XFER:
      case CeFsmBase::eCEFSMTYPE_SUB:
      case CeFsmBase::eCEFSMTYPE_HOLD:
      case CeFsmBase::eCEFSMTYPE_PUB:
      default:
      {
         eType = CeFsmBase::eCEFSMTYPE_MAX;
      }
      break;
   }

   /* Lookup the mapped event.
   */
   if ( pEvtMap != NULL )
   {
      while ( (pEvtMap->eEvt != eCEEVT_DNP) &&
              (pEvtMap->uEvt != uEvtIn) )
      {
         pEvtMap++;
      }

      reEvtOut = pEvtMap->eEvt;
   }

   CCTK_TRACEI4( "Map CCTK Event - FSM %s - In: 0x%lX, Out: 0x%x (%s)",
                 gpcCeFsmName[eType],
                 uEvtIn,
                 reEvtOut,
                 gpcCeFsmEvent[static_cast<BOS_UINT32>(reEvtOut) - eCEEVT_INV] );
}


/***********************************************************************************
**  FUNCTION:   CeMain::updateChallenge
**
**  PURPOSE:    Update the challenge information associated with a component due to
**              reception of an updated challenge from the SCE layer.
**
**  PARAMETERS:
**              uGuidParent - the parent GUID for which this change applies, so
**                            the parent for which all childs need update.
**              opaque      - opaque reference to the challenge information.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::updateChallenge( BOS_UINT32 uGuidParent )
{
   BOS_UINT32 uIx;
   MXD_GNS::ISceBasicExtensionControl *pSceBasicExtCtl = NULL;
   mxt_result res;
   CeResBase *pRes;
   
   if ( !locateRes( uGuidParent, pRes ) )
   {
      return;
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpResCall[uIx] && (mpResCall[uIx]->resourceParent() == uGuidParent))
      {
         res = mpResCall[uIx]->resourceNetHdl()->QueryIf( &pSceBasicExtCtl );
         if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
         {
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
            {
               CeResUser *pUser = reinterpret_cast<CeResUser *>
                  ( pRes->resourceAddress() );
               pSceBasicExtCtl->AddChallengeInfo( pUser->resourceChallengeId() );
               pSceBasicExtCtl->SetChallengeType( pUser->resourceChallengeId(),
                  static_cast<unsigned int> (
                        MXD_GNS::uPROXY_AUTHENTICATION_REQUIRED) );

               pSceBasicExtCtl->ReleaseIfRef();
               pSceBasicExtCtl = NULL;
            }
         }
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx] && (mpResSub[uIx]->resourceParent() == uGuidParent))
      {
         if ( mpResSub[uIx]->resourceBasicCtl( &pSceBasicExtCtl ) )
         {
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
            {
               CeResUser *pUser = reinterpret_cast<CeResUser *>
                  ( pRes->resourceAddress() );
               pSceBasicExtCtl->AddChallengeInfo( pUser->resourceChallengeId() );
               pSceBasicExtCtl->SetChallengeType( pUser->resourceChallengeId(),
                  static_cast<unsigned int> (
                        MXD_GNS::uPROXY_AUTHENTICATION_REQUIRED) );

               pSceBasicExtCtl->ReleaseIfRef();
               pSceBasicExtCtl = NULL;
            }
         }
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
   {
      if ( mpResNot[uIx] && (mpResNot[uIx]->resourceParent() == uGuidParent))
      {
         res = mpResNot[uIx]->resourceNetOodHdl()->QueryIf( &pSceBasicExtCtl );
         if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
         {
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
            {
               CeResUser *pUser = reinterpret_cast<CeResUser *>
                  ( pRes->resourceAddress() );
               pSceBasicExtCtl->AddChallengeInfo( pUser->resourceChallengeId() );
               pSceBasicExtCtl->SetChallengeType( pUser->resourceChallengeId(),
                  static_cast<unsigned int> (
                        MXD_GNS::uPROXY_AUTHENTICATION_REQUIRED) );

               pSceBasicExtCtl->ReleaseIfRef();
               pSceBasicExtCtl = NULL;
            }
         }
      }
   }
}

/***********************************************************************************
**  FUNCTION:   CeMain::CeDispatchRestrictedUser
**
**  PURPOSE:    Creates a temporary clone of the user to contact a specific target. 
**              The target contacted depends on the signalling restrictions in place.
**
**  PARAMETERS:
**              uResGuid  - the user resource to trigger the backup to primary on
**              bBackupToPrimary - if enabled the backup user is to contact the
**                      the user's primary target. Otherwise contact the inbound
**                      target if available.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::CeDispatchRestrictedUser( BOS_UINT32 uResGuid, BOS_BOOL bBackupToPrimary )
{
   CeResBase *pRes;
   CeResUser *pResUser = NULL;
   
   if ( !locateRes( uResGuid, pRes ) )
   {
      return;
   }
   else
   {
      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         pResUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );
         BOS_BOOL bValid = BOS_TRUE;

         /* If we're initiating this user to attempt to registration with the
         ** the primary target.
         */
         if ( bBackupToPrimary )
         {
            BOS_BOOL bResActive = BOS_FALSE;
            bValid = BOS_FALSE;
         
            CCTK_TRACEI1( "CeDispatchRestrictedUser - (0x%X)", pResUser->resourceGuid() );
         
             /* Check to see if we have active calls on this user currently.
             */
            bResActive = checkCallRes( pResUser->resourceGuid() );

            /* If we don't have any active calls on this user currently, apply
            ** the attempt to recover to the primary now.
            */
            if ( !bResActive )
            {
               /* If the current primary is no longer the highest preferred target, then
               ** a new registration cycle needs to be started to clean up our current
               ** backup and recovery knowledge, and to learn of the new available
               ** primary.
               */
               if( !nsBrPrimaryStillPreferred( pResUser->resourceGuid() ) )
               {
                  CCTK_TRACEI1(
                        "CeDispatchRestrictedUser - User 0x%lX has a new preferred primary, terminating registration",
                        pRes->resourceGuid());
                  
                  nsBrRestart( pResUser->resourceGuid() );
                  pResUser->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGING );
                  pResUser->resourceNetHdl()->Terminate();
               }
               /* If the primary target hasn't changed, see if failback to the primary
               ** needs to be applied.
               */
               else if ( pResUser->resourceFailbkPrim() && 
                         nsBrFailedPrimary( pResUser->resourceGuid() ) )
               {
                  bValid = BOS_TRUE;
               }
            }
            else
            {
               /* We have active calls on this user currently.
               ** If primary recovery start and end timers are configured,
               ** then we will restart the failback timer.  Otherwise, 
               ** we will attempt to recover to the primary target next time 
               ** the registration needs to be refreshed.
               */
               BOS_UINT32 uFailBkTmrMs;
               BOS_UINT32 uTmrFailBkGuid;
               const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );
               CCTK_ASSERT( pCfgUsr != NULL );
               if ( ( pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr ) &&
                    ( pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMaxTmr ) && 
                    ( pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr < pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMaxTmr ) )
               {
                  /* failback timer is calculated as: 
                  **      uFailBkTmrMs = rand ([startTime], [endTime]);
                  */
                  uFailBkTmrMs = rand() % (pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMaxTmr - pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr + 1) + 
                                 pCfgUsr->xtl.usr.net.brCfg.uBackToPrimMinTmr;

                  CCTK_TRACEI1(
                        "CeDispatchRestrictedUser - User 0x%lX has active calls, restart primary recovery timer",
                        pResUser->resourceGuid());
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
         /* Otherwise this is to attempt registration for inbound failover.
         ** Make sure that there is a restricted target for this user.
         */
         else if ( pResUser->resourceGetInbdTgtAddr() == NULL )
         {
            bValid = BOS_FALSE;
         }
         
         /* If we don't have a backup user configured for the current user then
         ** we can't do anything at the moment.
         */
         if ( pResUser->resourceBkupConfig() == guCeInvalidGuid )
         {
            bValid = BOS_FALSE;
         }
         
         /* If we're ok to dispatch this restricted user to attempt registration,
         ** add the user and apply the restrictions.
         */
         if ( bValid )
         {
            CeResBase *pResBkupUser;

            /* Create the user that will be used to target the non registered
            ** peer for service.  Note that this user is based on the backup
            ** initial configuration of the current user in order to
            ** preserve the same original credentials for identification
            ** (those credentials may have been updated on the current user
            ** following the initial registration of the latter).
            */
            ceNotify( pResUser->resourceBkupConfig(),
                      eCEEVT_USR_ADD,
                      NULL );

            if ( locateRes( pResUser->resourceBkupConfig(),
                            pResBkupUser ) )
            {
               pResUser->resourceSetBkupConfig( guCeInvalidGuid );
               
               if ( pResBkupUser->resourceGender() == CeResBase::eCERESTYPE_USR )
               {
                  CeResUser *pUser = reinterpret_cast<CeResUser *>
                     ( pResBkupUser->resourceAddress() );
                  pUser->resourceSetBkupParent( pResUser->resourceGuid() );

                  /* Setup the backup and recovery module such that this new user
                  ** will only attempt to target the restricted target and will return a
                  ** status immediately for this attempt (i.e. the new user is not
                  ** required to support full backup discovery at this time, only to
                  ** see whether the restricted peer is reachable again).
                  */
                  nsBrTargetCopy( pResUser->resourceGuid(),
                                  pResBkupUser->resourceGuid() );
                  if ( bBackupToPrimary )
                  {
                     bValid = nsBrRestrict2Primary( pResBkupUser->resourceGuid(),
                                              BOS_TRUE );
                  }
                  else
                  {
                     bValid = nsBrRestrict2Address( pResBkupUser->resourceGuid(),
                                        BOS_TRUE, pResUser->resourceGetInbdTgtAddr() );
                  }

                  /* Ensure the UASSP resource associated with this user is properly
                  ** configured.
                  */
                  if ( bValid )
                  {
                     ceNotify ( pResBkupUser->resourceGuid(),
                                eCEEVT_REG_USR_ADD_OK,
                                NULL );
                    
                     /* Force initial registration, follow up action will come from
                     ** the result of that registration attempt.
                     */
                     ceNotify ( pResBkupUser->resourceGuid(),
                                eCEEVT_REG_USR_REG,
                                NULL );
                  }
                  /* Otherwise restore it as the backup user.
                  */
                  else
                  {
                     BOS_UINT32 uCfgGuid = pUser->resourceBkupConfig();
                     pUser->resourceSetBkupConfig( guCeInvalidGuid );
                     
                     ceNotify( pResBkupUser->resourceGuid(),
                               eCEEVT_USR_REM,
                               NULL );
                      
                     pResUser->resourceSetBkupConfig( uCfgGuid );
                  }
               }
            }
         }
         
         CCTK_TRACEI3(
                        "CeDispatchRestrictedUser - for parent 0x%lX, restricted to %s, success = %d ",
                        pResUser->resourceGuid(), bBackupToPrimary? "primary" : "inbound", bValid );
      }
   }
}

/***********************************************************************************
**  FUNCTION:   CeMain::remoteRes
**
**  PURPOSE:    Checks whether the resource identified by the parent GUID is a
**              remote resource or not.
**
**  PARAMETERS:
**              uParentGuid - the parent GUID.  If a parent is a remote resource
**                            all children associated to this parent will also be
**                            considered remote resource.
**
**  RETURNS:    BOS_TRUE if the parent is a remote resource, BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeMain::remoteRes( BOS_UINT32 uParentGuid )
{
   BOS_BOOL bRet = BOS_FALSE;

#if CCTKCFG_REMOTE_USER_CONTROL
   CeResBase *pRes;
   
   if ( locateRes( uParentGuid, pRes ) )
   {
      CeResUser *pResUser = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         pResUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );
         
         if ( pResUser->resourceIsRemUsr() )
         {
            bRet = BOS_TRUE;
         }
      }
      else
      {
         /* Nothing to do, just report BOS_FALSE.
         */
      }
   }
#else
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uParentGuid );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   parse3gppImsXmlContent
**
**  PURPOSE:    Parses the given packet for the application/3gpp-ims+xml
**              content type, and returns the parsed <type>, <reason> and <action>
**              sub-elements if found.
**
**  PARAMETERS:
**              pPacket - pointer to sip packet to parse.
**              pDocument - pointer (reference) to the created xml document
**                          used for parsing the packet's xml body. If not NULL
**                          the caller is reponsible of releasing the reference.
**              pAllocator - pointer (reference) to the created sub allocator
**                           used for parsing the packet's xml body. If not NULL
**                           the caller is reponsible of deallocating after use.
**              pType - pointer (reference) to the parsed <type> sub element.
**              pReason - pointer (reference) to the parsed <reason> sub element.
**              pAction - pointer (reference) to the parsed <action> sub element.
**
**  RETURNS:    TRUE if the packet's message body could be parsed, false otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL CeMain::parse3gppImsXmlContent( const MXD_GNS::CSipPacket *pPacket,
                                      MXD_GNS::IXmlDocument *&pDocument,
                                      MXD_GNS::CSubAllocator  *&pAllocator,
                                      char *&pType, char *&pReason, char *&pAction )
{
   pDocument = NULL;
   pAllocator = NULL;
   pType = NULL;
   pReason = NULL;
   pAction = NULL;

   if ( pPacket != NULL )
   {
      /* Retreive the Content header.
      */
      const MXD_GNS::CSipHeader *pHeader =
                  pPacket->GetHeaderList().Get( MXD_GNS::eHDR_CONTENT_TYPE );

      /* Inspect the Content header for "application" as the type and
      ** "3gpp-ims+xml" as the subtype.
      */
      if ( pHeader &&
           ( pHeader->GetContentTypeMType().GetString().CaseInsCmp(
             CE_CONTENT_TYPE_APPLICATION ) == 0 ) &&
           ( pHeader->GetContentTypeMSubType().GetString().CaseInsCmp(
             CE_CONTENT_TYPE_IMS_XML ) == 0 ) )
      {
         const MXD_GNS::CSipMessageBody *pBody = NULL;
         const MXD_GNS::CBlob *pBlobBody = NULL;

         /* Retrieve the packet's message body to parse the XML.
         */
         pBody = pPacket->CreateSipMessageBody();
         if ( pBody )
         {
            pBlobBody = pPacket->GetSipMessageBody()->GetBlobBody();
            if ( pBlobBody )
            {
               CreateEComInstance( MXD_GNS::CLSID_CXmlDocument,
                                   NULL,
                                   OUT &pDocument );
               if ( pDocument != NULL )
               {
                  mxt_result resReturn = resFE_FAIL;

                  pAllocator = BRCM_NEW( MXD_GNS::CSubAllocator );
                  CCTK_ASSERT( pAllocator != NULL );

                  pDocument->SetAllocator( pAllocator );
                  resReturn = pDocument->Parse( pBlobBody->GetFirstIndexPtr(),
                                                pBlobBody->GetSize() );

                  if ( MX_RIS_S( resReturn ) )
                  {
                     const char *pVersionAttr = NULL;

                     /* Get the list element.
                     */
                     MXD_GNS::CXmlElement *pElement =
                                                pDocument->GetRootElement();
                     CCTK_ASSERT( pAllocator != NULL );

                     resReturn = pElement->GetAttribute( NULL,
                                                   CE_IMS_XML_ATTRIBUTE_VERSION,
                                                   pVersionAttr );

                     /* Ensure the <ims-3gpp> element is the root element and the
                     ** version attribe is present.
                     */
                     if ( pElement && pVersionAttr &&
                          MxStringCaseCompareLength( pElement->GetName(),
                                    CE_IMS_XML_ELEMENT_IMS3GPP,
                                    sizeof( CE_IMS_XML_ELEMENT_IMS3GPP ) ) == 0 )
                     {
                        /* Look for the <alternative-service> child element.
                        */
                        pElement = pElement->FindChildElement(
                                          NULL, CE_IMS_XML_ELEMENT_ALTSVC, 0 );
                        if ( pElement != NULL )
                        {
                           MXD_GNS::CXmlElement *pAltSvcChildElement = NULL;
                           /* Look for the <type>.
                           */
                           pAltSvcChildElement = pElement->FindChildElement(
                                             NULL, CE_IMS_XML_ELEMENT_TYPE, 0 );
                           if ( pAltSvcChildElement != NULL )
                           {
                              pType = const_cast<char *>(
                                             pAltSvcChildElement->GetValue() );
                           }
                           /* Look for the <reason>.
                           */
                           pAltSvcChildElement = pElement->FindChildElement(
                                          NULL, CE_IMS_XML_ELEMENT_REASON, 0 );
                           if ( pAltSvcChildElement != NULL )
                           {
                              pReason = const_cast<char *>(
                                             pAltSvcChildElement->GetValue() );
                           }
                           /* Look for the <action>.
                           */
                           pAltSvcChildElement = pElement->FindChildElement(
                                          NULL, CE_IMS_XML_ELEMENT_ACTION, 0 );
                           if ( pAltSvcChildElement != NULL )
                           {
                              pAction = const_cast<char *>(
                                             pAltSvcChildElement->GetValue() );
                           }

                           CCTK_TRACEI0(
                              "parse3gppImsXmlContent - parsed child elements" );
                           return BOS_TRUE;
                        }
                     }
                  }
               }
            }
         }
      }
   }
   CCTK_TRACEI0( "parse3gppImsXmlContent - failed" );
   return BOS_FALSE;
}

/* ------------------------------------------------------------------------------ */
/* ---- BELOW THIS POINT, THE CALLBACK INTERFACE FROM THE STACK IS IMPLEMENTED -- */
/* ---- THIS IS THE ONLY PORTION OF THE CALL ENGINE MODULE THAT MAY (SHOULD) ---- */
/* ---- BE IN DIRECT CONTACT WITH THIRD PARTY PROCESSING. ----------------------- */
/* ------------------------------------------------------------------------------ */

/***********************************************************************************
**  FUNCTION:   CeMain::EvRegistrationStatus
**
**  PURPOSE:    Callback from UASSP IUaSspRegistrationMgr object for status
**              update on registration information.
**
**  PARAMETERS:
**              pRegistration   - the registration target for this event.
**              eStatus         - the status information.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvRegistrationStatus( IN MXD_GNS::IUaSspRegistration* pRegistration,
                                   IN EStatus eStatus )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;

   /* Make sure this is for a known registration and retrieve the corresponding
   ** user resource.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx]->resourceEqual( pRegistration ))
      {
         pRes = mpResUser[uIx];
         break;
      }
   }
   /* If we could not locate a valid resource for this registration,
   ** terminate the registration to prevent any mis-behavior.
   */
   if ( pRes == NULL )
   {
      pRegistration->ReleaseIfRef();
      pRegistration = NULL;
   }
   /* ...otherwise, map the event and send it to the FSM.
   */
   else
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eStatus ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt );
   }
}

/***********************************************************************************
**  FUNCTION:   CeMain::EvExpiring
**
**  PURPOSE:    Callback from UASSP IUaSspRegistrationMgr object when the
**              registration is about to be refreshed.
**
**  PARAMETERS:
**              pRegistration   - the registration target for this event.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvExpiring( IN MXD_GNS::IUaSspRegistration* pRegistration)
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pRegistration );
}

/***********************************************************************************
**  FUNCTION:   CeMain::EvInternalSubscriberCreated
**
**  PURPOSE:    Callback from UASSP IUaSspRegistrationMgr object for reporting
**              the creation of the internal subscriber object for registration.
**
**  PARAMETERS:
**              pRegistration   - the registration target for this event.
**              pSubscriber     - the newly created subscriber object.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvInternalSubscriberCreated(
                  IN MXD_GNS::IUaSspRegistration *pRegistration,
                  IN MXD_GNS::ISceSubscriber *pSubscriber )
{
   MXD_GNS::ISceBasicExtensionControl *pSceBasicExtCtl = NULL;
   mxt_result res;
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;
   CeResUser *pResUser = NULL;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pSubscriber );

   /* Make sure this is for a known registration and retrieve the corresponding
   ** user resource.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx]->resourceEqual( pRegistration ))
      {
         pRes = mpResUser[uIx];
         break;
      }
   }

   if ( pRes )
   {
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
      /* Per IMS specification, we configure the subscription refresh
      ** threshold to be 600 seconds.
      */
      pSubscriber->SetSubscriptionExpiringThreshold( 600 );
#endif

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         pResUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );

         /* Set the manager and packet obeserver for this new subscriber.
         ** Note we may already have a subscriber assosciated for this registration
         ** but if that's the case the cleanup for the current/known subscriber will be 
         ** done in EvSipContextCreated.
         */
         res =
            pRegistration->QuerySubscriber(
                              MXD_GNS::IID_ISceBasicExtensionControl,
                              reinterpret_cast<void**>(&pSceBasicExtCtl) );
         if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
         {
            pSceBasicExtCtl->SetBasicExtensionManager( gpCeMain );
            pSceBasicExtCtl->SetSipPacketObserver(
               reinterpret_cast<MXD_GNS::ISceSipPacketObserver *>(
                                                          pmGetPktObs() ) );

            /* Associate a unique GUID with this resource even though it is an
            ** internal resource of the stack, we still need to keep track of it
            ** via its own GUID for other services hook up.
            */
            pResUser->resourceSetSubRegGuid( ceGuid() );

            pSceBasicExtCtl->ReleaseIfRef();
            pSceBasicExtCtl = NULL;
         }
      }
      else
      {
        
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvPauReceived
**
**  PURPOSE:    Callback from UASSP IUaSspRegistrationMgr object for information
**              about received P-Associated-URI content.
**
**  PARAMETERS:
**              pRegistration   - the registration target for this event.
**              raddrPau        - the P-Associated-URI information.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvPauReceived( IN MXD_GNS::IUaSspRegistration* pRegistration,
                            IN const MXD_GNS::CNameAddr& raddrPau )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;

   /* Make sure this is for a known registration and retrieve the corresponding
   ** user resource.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      if ( mpResUser[uIx]->resourceEqual( pRegistration ))
      {
         pRes = mpResUser[uIx];
         break;
      }
   }
   /* If we could not locate a valid resource for this registration,
   ** terminate the registration to prevent any mis-behavior.
   */
   if ( pRes == NULL )
   {
      pRegistration->ReleaseIfRef();
      pRegistration = NULL;
   }
   /* ...otherwise, map the event and send it to the FSM.
   */
   else
   {
      CEEVT eEvt = eCEEVT_INV;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         CeResUser *pUser = reinterpret_cast<CeResUser *>
            ( pRes->resourceAddress() );
         pUser->resourceSetPauId( raddrPau );

         fsmMapEvent( pRes->resourceFsm(),
                      static_cast<BOS_UINT32> ( eCEEVT_REG_NET_PAU_ID ),
                      eEvt );

         fsmProcessEvt( pRes->resourceGuid(),
                        eEvt );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvNewCallObject
**
**  PURPOSE:    Callback from UASSP IUaSspNewCallMgr object for creating support
**              for a new incoming call on the system.
**
**  PARAMETERS:
**              pCall           - the call reference from UASSP.
**              pUserConfig     - the associated user (if non NULL).
**              rPacket         - reference to the packet causing this action.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvNewCallObject( IN MXD_GNS::IUaSspCall* pCall,
                              IN MXD_GNS::ISceUserConfig* pUserConfig,
                              IN const MXD_GNS::CSipPacket& rPacket )
{
   BOS_UINT32 uParentGuid = guCeInvalidGuid;
   CeResBase *pRes = NULL;
   BOS_UINT32 uIx;
   const CFGUSR *pCfgUsr = NULL;
   mxt_result res;

   /* This could happen if the call is coming directly from a peer which
   ** does not know our registration contact matching.  This would mean that
   ** the call is not coming through a proxy with which we have registered.
   **
   ** We could reject this scenario, but in an effort to accept peer-to-peer
   ** scenario, we would attempt to match the SIP URI to a known configured user.
   */
   if ( pUserConfig == NULL )
   {
      const MXD_GNS::CSipUri *pUri = rPacket.GetRequestLine()->GetSipUri();
      if ( pUri )
      {
         MXD_GNS::CSipUri *pTmpUri = BRCM_NEW( MXD_GNS::CSipUri ) ( *pUri );
         MXD_GNS::CNameAddr addr;

         addr.Reset();
         addr.SetUri( pTmpUri );

         for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
         {
            if ( (mpResUser[uIx] != NULL) && (pUserConfig == NULL) )
            {
               pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
               if ( (pCfgUsr != NULL) &&
                      /* Check if this is a known user.
                      */
                    ( !strcmp(
                       reinterpret_cast<const char *> (
                                       pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                         pUri->GetUser().GetString().CStr() ) ||
                      /* Check if this is an assumed identity for a known
                      ** user.
                      */
                      mpResUser[uIx]->resourceIsAssumedId( addr ) ) )
               {
                  pUserConfig = pCfgUsr->in.pSceUsrCfg;
                  pCall->SetConfiguration( pUserConfig );
                  break;
               }
            }
         }
      }
   }

#if CCTKCFG_REMOTE_USER_CONTROL
   /* If we are allowing remote user control, then we need to see whether
   ** this call is to be associated with a remote user contact information.
   **
   ** In such case, it means the call is an outgoing call from the remote
   ** user that appears to us as incoming towards our local view of the
   ** remote user.
   */
   if ( pUserConfig == NULL )
   {
      const MXD_GNS::CSipHeader *pContact =
               rPacket.GetHeaderList().Get( MXD_GNS::eHDR_CONTACT, res );

      if ( MX_RIS_S( res ) && ( pContact != NULL ) )
      {
         const MXD_GNS::CNameAddr nameAddr = pContact->GetContact();
         const MXD_GNS::CSipUri *pUri =
            reinterpret_cast<const MXD_GNS::CSipUri *> ( nameAddr.GetUri() );
         if ( pUri )
         {
            for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
            {
               if ( (mpResUser[uIx] != NULL) && (pUserConfig == NULL) )
               {
                  pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
                  if ( (pCfgUsr != NULL) &&
                       /* Check if this is a known user.
                       */
                       !strcmp(
                          reinterpret_cast<const char *> (
                             pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                          pUri->GetUser().GetString().CStr() ) )
                  {
                     pUserConfig = pCfgUsr->in.pSceUsrCfg;
                     pCall->SetConfiguration( pUserConfig );
                     break;
                  }
               }
            }
         }
      }
   }
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

#if CCTKCFG_IP_ADDRESS_DIALING
   /* If we are allowing IP address dialing support, we need to check
   ** whether this call is targetted to an IP address exclusively, if that
   ** is the case, we will associate this call with the first user on the
   ** system by default.
   */
   if ( pUserConfig == NULL )
   {
      const MXD_GNS::CSipUri *pUri = rPacket.GetRequestLine()->GetSipUri();
      if ( pUri )
      {
         MXD_GNS::CSipUri *pTmpUri = BRCM_NEW( MXD_GNS::CSipUri ) ( *pUri );
         MXD_GNS::CNameAddr addr;
         BOS_IP_ADDRESS bosIp;

         addr.Reset();
         addr.SetUri( pTmpUri );

         if ( bosIpAddrCreateFromStr (
                  reinterpret_cast<const char *> (
                     pTmpUri->GetHostPort().GetHost().CStr() ),
                  &bosIp ) == BOS_STATUS_OK )
         {
            if ( bosIpAddrIsEqual ( &bosIp,
                                    /* Assume the first IP address configured for
                                    ** the device is the one we want to target.
                                    */
                                    &CFG_SYS->xtl.net.gen.host.addr[0].ipAddr ) )
            {
               /* Ensure the first user is properly configured locally and use
               ** that user for targetting the incoming call to.
               */
               if ( mpResUser[0] != NULL )
               {
                  pCfgUsr = CFG_USR( mpResUser[0]->resourceGuid() );
                  if ( pCfgUsr != NULL )
                  {
                     pUserConfig = pCfgUsr->in.pSceUsrCfg;
                     pCall->SetConfiguration( pUserConfig );
                  }
               }
            }
         }
      }
   }
#endif /* CCTKCFG_IP_ADDRESS_DIALING */

   /* Make sure the call is for a registered/configured user.  We do not
   ** accept calls for unknown target user(s).
   */
   if ( pUserConfig != NULL )
   {
      /* Locate the user resource for this call and create a call resource
      ** to handle it.
      */
      BOS_UINT32 uResGuid = allocateGuid();

      /* Loop through the known configured user to identify the one corresponding
      ** to this config.
      */
      for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
      {
         if ( (mpResUser[uIx] != NULL) && (uParentGuid == guCeInvalidGuid) )
         {
            pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
            if ( (pCfgUsr != NULL) && (pCfgUsr->in.pSceUsrCfg == pUserConfig) )
            {
               uParentGuid = mpResUser[uIx]->resourceGuid();
               break;
            }
         }
      }

      /* Emergency call.
      **
      ** Emergency call and emergency related services from the network are
      ** marked with priority header setting (see RFC 4119).
      */
      const MXD_GNS::CSipHeader *pPriority =
         rPacket.GetHeaderList().Get( MXD_GNS::eHDR_PRIORITY, res );
      BOS_BOOL bEmergency = BOS_FALSE;

      if ( MX_RIS_S( res ) && ( pPriority != NULL ) )
      {
         const MXD_GNS::CToken priority = pPriority->GetPriority();
         if ( !priority.GetString().CaseInsCmp( CE_MAIN__PRI__EMERGENCY ) )
         {
            bEmergency = BOS_TRUE;
         }
      }
      
      /* The presence of a P-Asserted Identity header with service sos urn
      ** indicates that this is an emergency call as well.
      */
      if( !bEmergency )
      {
         const MXD_GNS::CSipHeader *pPAssertedIdentityHeader =
           rPacket.GetHeaderList().Get( MXD_GNS::eHDR_P_ASSERTED_IDENTITY, res);
         if ( MX_RIS_S( res ) && ( pPAssertedIdentityHeader != NULL ) )
         {
            while ( pPAssertedIdentityHeader != NULL )
            {
               const MXD_GNS::CNameAddr assertedIdentity = pPAssertedIdentityHeader->GetPAssertedIdentity();
               const MXD_GNS::IUri *pUri = assertedIdentity.GetUri();
               
               if ( pUri && 
                    pUri->GetUriType() == MXD_GNS::IUri::eABSOLUTE )
               {
                  const MXD_GNS::CAbsoluteUri *pAbsUri =
                  reinterpret_cast<const MXD_GNS::CAbsoluteUri *> (
                                                                   pUri );

                  if ( !strCmpNoCase( pAbsUri->GetScheme(),
                                      "urn" ) &&
                        !strCmpNoCase( pAbsUri->GetBody().CStr(),
                                      CE_MAIN_SERVICE_SOS ) )
                  {
                     bEmergency = BOS_TRUE;
                     pPAssertedIdentityHeader = NULL;
                  }
               }
               
               if ( pPAssertedIdentityHeader )
               {
                  pPAssertedIdentityHeader = pPAssertedIdentityHeader->GetNextHeader();
               }
            }
         }           
      }

      /* Check whether we need to terminate immediately any other call(s)
      ** associated with this user because the incoming call is an emergency
      ** one.  In the case we need to, clean up all calls prior to accepting
      ** this one.  This will also ensure we have resource for this user to
      ** accept this call.
      */
      if ( (uParentGuid != guCeInvalidGuid) &&
           (pCfgUsr != NULL) &&
           bEmergency &&
           pCfgUsr->xtl.usr.net.emgCfg.bEndCallOnInc )
      {
         shutDown ( CeResBase::eCERESTYPE_CALL,
                    uParentGuid );
      }

      /* If the parent was located, continue with call creation and allocation.
      */
      if ( (uResGuid != guCeInvalidGuid) &&
           (uParentGuid != guCeInvalidGuid ) &&
           (allocateRes( CeResBase::eCERESTYPE_CALL,
                         uResGuid,
                         uParentGuid,
                         BOS_FALSE ) != guCeInvalidGuid) )
      {
         locateRes( uResGuid, pRes );

         if ( cfgAddCall( uResGuid, uParentGuid ) == eCFGSTATUS_SUCCESS )
         {
            /* Emergency call.
            **
            ** Check on whether we are dealing with a new incoming emergency call,
            ** if so, we need to mark it right away to ensure all the packets sent
            ** related to this emergency call will have the right settings,
            ** including those generated internally by the stack layers.
            **
            ** Note: It is mandatory to do this step *prior* to creating the CE
            **       resource such that the UASSP resource configuration will be
            **       accurate.
            */
            if ( bEmergency )
            {
               CCTKEMGCALLCFG emCfg;

               /* Mark the call as emergency.
               */
               memset ( &emCfg, 0, sizeof ( CCTKEMGCALLCFG ) ); 
               emCfg.bEmergency = BOS_TRUE;

               tkmSetParm( uResGuid,
                           eCCTKPARM_EC_EMGCALLCFG,
                           reinterpret_cast<const void *> ( &emCfg ) );
            }
#if CCTKCFG_REMOTE_USER_CONTROL
            /* Resource allocation is successful, continue with creation and
            ** set up such that FSM can take over.
            */
            if ( remoteRes ( uParentGuid ))
            {
               pRes->resourcePushFsm( CeFsmBase::eCEFSMTYPE_PROXY_IN );
            }
            else
            {
               pRes->resourcePushFsm( CeFsmBase::eCEFSMTYPE_IN );
            }
#else
            pRes->resourcePushFsm( CeFsmBase::eCEFSMTYPE_IN );
#endif
            pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_CALLIN_IDLE );
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pResCall = reinterpret_cast<CeResCall *>
                  ( pRes->resourceAddress() );
               pResCall->resourceCreate( uResGuid,pCall );
            }
         }
         else
         {
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pResCall = reinterpret_cast<CeResCall *>
                  ( pRes->resourceAddress() );
               pResCall->resourceEnd();
               /* Set the manager such that the call can be rejected as 'busy'
               ** even though we are not associating internal resources.
               */
               pCall->AddIfRef();
               pCall->SetManager( gpCeMain );
               pCall->SetOpaque( reinterpret_cast<mxt_opaque> (guCeBusyMarker) );
            }
         }
      }
      else
      {
         /* Set the manager such that the call can be rejected as 'busy'
         ** even though we are not associating internal resources.
         */
         pCall->AddIfRef();
         pCall->SetManager( gpCeMain );
         pCall->SetOpaque( reinterpret_cast<mxt_opaque> (guCeBusyMarker) );
      }
   }

   if ( uParentGuid == guCeInvalidGuid )
   {
      /* Cannot accept this new incoming call.
      */
      pUserConfig = NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvCalled
**
**  PURPOSE:    Callback from UASSP IUaSspCallMgr object when informing of
**              a new incoming call and the need to deal with it accordingly.
**
**  PARAMETERS:
**              pCall                  - the call reference from UASSP.
**              rFromAddr              - call from.
**              rToAddr                - call to.
**              pAdditionalParameters  - additional information passed.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvCalled( IN MXD_GNS::IUaSspCall* pCall,
                       IN const MXD_GNS::CNameAddr& rFromAddr,
                       IN const MXD_GNS::CNameAddr& rToAddr,
                       IN MXD_GNS::IEComUnknown* pAdditionalParameters )
{
   CeResBase *pRes = NULL;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rToAddr );

   if ( locateCallRes( pCall, pRes ))
   {
      CEEVT eEvt = eCEEVT_INV;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
      {
         CeResCall *pResCall = reinterpret_cast<CeResCall *>
            ( pRes->resourceAddress() );
         pResCall->resourceSetCall( rFromAddr );

         fsmMapEvent( pRes->resourceFsm(),
                      static_cast<BOS_UINT32> ( eCEEVT_CALL_NET_CALLED ),
                      eEvt );

         fsmProcessEvt( pRes->resourceGuid(),
                        eEvt,
                        (pAdditionalParameters ?
                              static_cast<void *> (pAdditionalParameters) : NULL));
      }
   }
   /* If this call cannot be associated with any known call resource, and we
   ** marked it as a busy we just reject it now, it could be a transient call
   ** attempt while we have max'ed out our ability to accept incoming calls.
   */
   else if ( pCall &&
             (reinterpret_cast<BOS_UINT32> (pCall->GetOpaque()) == guCeBusyMarker) )
   {
      pCall->RejectCall();
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvTerminated
**
**  PURPOSE:    Callback from UASSP IUaSspCallMgr object when informing of
**              a call termination.
**
**  PARAMETERS:
**              pCall      - the call reference from UASSP.
**              eReason    - the termination reason.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvTerminated( IN MXD_GNS::IUaSspCall* pCall,
                           IN MXD_GNS::IUaSspCallMgr::ETerminationReason eReason )
{
   CeResBase *pRes = NULL;

   if ( locateCallRes( pCall, pRes ))
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( (
                        eReason == eTERMINATION_USER_TERMINATED ||
                        eReason == eTERMINATION_USER_FORWARDED ||
                        eReason == eTERMINATION_USER_REJECTED ) ?
                              eCEEVT_CALL_NET_TERMUSR :
                              eCEEVT_CALL_NET_TERMNET ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt );
   }
   else if ( locateCallForkRes ( pCall, pRes ) )
   {
      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
      {
         CeResCall *pResCall = reinterpret_cast<CeResCall *>
            ( pRes->resourceAddress() );
         pResCall->resourceForkRem( pCall );
      }
      else
      {

      }
   }
   else if ( (reinterpret_cast<BOS_UINT32> (pCall->GetOpaque()) == guCeBusyMarker) )
   {
      pCall->ReleaseIfRef();
      pCall = NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvFailed
**
**  PURPOSE:    Callback from UASSP IUaSspCallMgr object when informing of
**              an outgoing call failure.
**
**  PARAMETERS:
**              pCall        - the call reference from UASSP.
**              uStatusCode  - SIP response status code.
**              rStrReason   - the reason for failure.
**              uRetryAfter  - an indication of how long to wait before possibly
**                             reattempting the same call again.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvFailed( IN MXD_GNS::IUaSspCall* pCall,
                       IN uint16_t uStatusCode,
                       IN const MXD_GNS::CString& rStrReason,
                       IN unsigned int uRetryAfter )
{
   CeResBase *pRes = NULL;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uRetryAfter );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rStrReason );
   /* Note that we do not use the status code at this time.
   */
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uStatusCode );

   /* There is a Timeout issue when log enable for sending REQUESTs.
   ** When all log enable, SIP stack does not have time to pick up the 
   ** response from its socket before it gets timeout.  Since, there is
   ** a retry, the event only get queue when there is no more retry. */
   
   if ( uStatusCode == MXD_GNS::uREQUEST_TIMEOUT && uRetryAfter == 0 )
   {
      CCTK_TRACED0( "CeMain::EvFailed() due to 'REQUEST_TIMEOUT'.  Try disabling logging.  This issue can be due to logging.  Response handling is delayed by logging, causing an internal timeout." );
   }
   
   if ( locateCallRes( pCall, pRes ))
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eCEEVT_CALL_NET_FAILED ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvAnswered
**
**  PURPOSE:    Callback from UASSP IUaSspCallMgr object when informing of
**              an outgoing call answer.
**
**  PARAMETERS:
**              pCall                     - the call reference from UASSP.
**              pAdditionalParameters     - the Client event data for Callee ID
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvAnswered( IN MXD_GNS::IUaSspCall* pCall,
                         IN MXD_GNS::IEComUnknown* pAdditionalParameters )
{
   CeResBase *pRes = NULL;

   if ( locateCallRes( pCall, pRes ))
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eCEEVT_CALL_NET_ANSWER ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt,
                     ( pAdditionalParameters ?
                           static_cast<void *> ( pAdditionalParameters) : NULL ) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvProgress
**
**  PURPOSE:    Callback from UASSP IUaSspCallMgr object when informing of
**              the progress of an outgoing call.
**
**  PARAMETERS:
**              pCall              - the call reference from UASSP.
**              uStatusCode        - the status code associated with this progress.
**              rstrDisplayString  - the string associated with the received status.
**              pForwardedAddr     - if call has been forwarded (3xx status), this
**                                   is the new remote party indication.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvProgress( IN MXD_GNS::IUaSspCall* pCall,
                         IN uint16_t uStatusCode,
                         IN const MXD_GNS::CString& rstrDisplayString,
                         IN const MXD_GNS::CNameAddr* pForwardedAddr )
{
   CeResBase *pRes = NULL;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rstrDisplayString );

   if ( locateCallRes( pCall, pRes ))
   {
      CEEVT eEvt = eCEEVT_INV;
      CEEVT eEvtIn = eCEEVT_DNP;

      if ( pForwardedAddr &&
           ( uStatusCode & MXD_GNS::eSIP_STATUS_CLASS_REDIRECTION ) )
      {
         CeResCall *pResCall = reinterpret_cast<CeResCall *>
            ( pRes->resourceAddress() );

         pResCall->resourceSetSvc( *pForwardedAddr );
      }

      /* The only status codes that we are interested in are the ones triggering
      ** a real event to the FSM. All other status-codes will be ignored with
      ** the DNP event.
      */
      if (uStatusCode == MXD_GNS::uRINGING)
      {
         eEvtIn = eCEEVT_CALL_NET_RING;
      }
      else if (uStatusCode == MXD_GNS::uSESSION_PROGRESS)
      {
         eEvtIn = eCEEVT_CALL_NET_EARLYMEDIA;
      }
      
      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eEvtIn ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvForkedCall
**
**  PURPOSE:    Callback from UASSP IUaSspCallMgr object when informing of
**              an answer on a forked call.
**
**  PARAMETERS:
**              pCall              - the original call reference from UASSP.
**              pNewCall           - the forked call reference from UASSP.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvForkedCall( IN MXD_GNS::IUaSspCall* pCall,
                           IN MXD_GNS::IUaSspCall* pNewCall )
{
   CeResBase *pRes = NULL;

   /* Locate the call which was forked.  If it can be located, associate the
   ** forked call with it.
   */
   if ( locateCallRes( pCall, pRes ))
   {
      /* Associate the forked call with the actual CE call.  We keep this
      ** association for resource management.
      */
      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
      {
         CeResCall *pResCall = reinterpret_cast<CeResCall *>
            ( pRes->resourceAddress() );

         if ( pResCall->resourceForkAdd( pNewCall ) )
         {
            pNewCall->AddIfRef();
            pNewCall->SetManager( gpCeMain );
         }
      }
      else
      {

      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvReplacing
**
**  PURPOSE:    Callback from UASSP IUaSspCallMgr object when informing that a new
**              incoming call is replacing an existing one.
**
**  PARAMETERS:
**              pNewCall           - the call reference from UASSP.
**              pReplacedCall      - the replaced call reference from UASSP.
**              rRemoteAddr        - the new address of the peer
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvReplacing( IN MXD_GNS::IUaSspCall* pNewCall,
                          IN MXD_GNS::IUaSspCall* pReplacedCall,
                          IN const MXD_GNS::CNameAddr& rRemoteAddr )
{
   CeResBase *pRes = NULL;

   if ( locateCallRes( pNewCall, pRes ))
   {
      CEEVT eEvt = eCEEVT_INV;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
      {
         CeResCall *pResCall = reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );
         pResCall->resourceSetCall( rRemoteAddr );
         pResCall->resourceSetLink( pReplacedCall );

         fsmMapEvent( pRes->resourceFsm(),
                      static_cast<BOS_UINT32> ( eCEEVT_CALL_NET_REPLACE ),
                      eEvt );

         fsmProcessEvt( pRes->resourceGuid(),
                        eEvt );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvTransfereeTransferRequested
**
**  PURPOSE:    Callback from UASSP IUaSspCallMgr object when a transfer has been
**              requested for the current call.
**
**  PARAMETERS:
**              pCall              - the call reference from UASSP.
**              rTarget            - the remote peer to be transfered to.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvTransfereeTransferRequested( IN MXD_GNS::IUaSspCall* pCall,
                                            IN const MXD_GNS::CNameAddr& rTarget )
{
   CeResBase *pRes = NULL;

   if ( locateCallRes( pCall, pRes ))
   {
      CEEVT eEvt = eCEEVT_INV;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
      {
         CeResCall *pResCall = reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );
         pResCall->resourceSetSvc( rTarget );
         pRes->resourcePushFsm( CeFsmBase::eCEFSMTYPE_XFER );
         pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_XFER_IDLE );

         fsmMapEvent( pRes->resourceFsm(),
                      static_cast<BOS_UINT32> ( eCEEVT_XFER_NET_XFER ),
                      eEvt );

         fsmProcessEvt( pRes->resourceGuid(),
                        eEvt );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvTransfereeTransferResult
**
**  PURPOSE:    Callback from UASSP IUaSspCallMgr object when a transfer status is
**              being informed.
**
**  PARAMETERS:
**              pCall            - the call reference from UASSP.
**              eTransferResult  - the transfer result.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvTransfereeTransferResult(
               IN MXD_GNS::IUaSspCall* pCall,
               IN MXD_GNS::IUaSspCallMgr::ETransferReport eTransferResult )
{
   CeResBase *pRes = NULL;

   if ( locateCallRes( pCall, pRes ))
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eTransferResult ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvTransferorTransferResult
**
**  PURPOSE:    Callback from UASSP IUaSspCallMgr object when a transfer status is
**              being informed.
**
**  PARAMETERS:
**              pCall            - the call reference from UASSP.
**              eTransferResult  - the transfer result.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvTransferorTransferResult(
               IN MXD_GNS::IUaSspCall* pCall,
               IN MXD_GNS::IUaSspCallMgr::ETransferReport eTransferResult )
{
   CeResBase *pRes = NULL;

   if ( locateCallRes( pCall, pRes ))
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eTransferResult ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvConnectedIdChange
**
**  PURPOSE:    Callback from UASSP IUaSspCallMgr object when informing of
**              an updated remote or local name address for a dialog.
**
**  PARAMETERS:
**              pCall                  - the call reference from UASSP.
**              rLocalAddr             - Local address.
**              rRemotelAddr            - Remote address.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvConnectedIdChange( IN MXD_GNS::IUaSspCall* pCall,
                                  IN const MXD_GNS::CNameAddr& rLocalAddr,
                                  IN const MXD_GNS::CNameAddr& rRemoteAddr )
{
   CeResBase *pRes = NULL;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rLocalAddr );

   if ( locateCallRes( pCall, pRes ) )
   {
      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
      {
         CeResCall *pResCall = reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );
         pResCall->resourceSetCall( rRemoteAddr );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvOodResponse
**
**  PURPOSE:    Callback from SCE-CORE ISceOodRequestSenderMgr object when the
**              status for an out-of-dialog request is being received.
**
**  PARAMETERS:
**              pComponent      - the network reference from SCE.
**              pUserConfig     - the associated user configuration.
**              opqTransaction  - opaque data the application may have used during
**                                creation of the corresponding request.
**              rResponse       - the SIP response packet.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvOodResponse( IN MXD_GNS::ISceOodRequestSender* pComponent,
                            IN MXD_GNS::ISceUserConfig* pUserConfig,
                            IN mxt_opaque opqTransaction,
                            IN const MXD_GNS::CSipPacket& rResponse )
{
   BOS_UINT32 uIx;
   CeResNot *pRes = NULL;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pUserConfig );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( opqTransaction );

   /* Make sure this is for a known sender and retrieve the information
   ** associated with it.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
   {
      if ( mpResNot[uIx]->resourceOodEqual( pComponent ) )
      {
         pRes = mpResNot[uIx];
         break;
      }
   }

   if ( pRes != NULL )
   {
      CEEVT eEvt = eCEEVT_DNP;

      if ( rResponse.IsResponse() && rResponse.GetStatusLine() )
      {
         BOS_UINT32 uRsp;

         if ( rResponse.GetStatusLine()->GetClass() ==
                                MXD_GNS::eSIP_STATUS_CLASS_INFORMATIONAL )
         {
            /* Ignore the response in this case and wait for the final response.
            */
         }
         else if ( rResponse.GetStatusLine()->GetClass() ==
                                     MXD_GNS::eSIP_STATUS_CLASS_SUCCESS )
         {
            eEvt = eCEEVT_NOT_NET_OK;
         }
         else
         {
            eEvt = eCEEVT_NOT_NET_FAIL;
         }

         uRsp = static_cast<BOS_UINT32> ( rResponse.GetStatusLine()->GetCode() );

         fsmMapEvent( pRes->resourceFsm(),
                      static_cast<BOS_UINT32> ( eEvt ),
                      eEvt );

         fsmProcessEvt( pRes->resourceGuid(),
                        eEvt,
                        static_cast<void *> ( &uRsp ) );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvChallenged
**
**  PURPOSE:    Callback from SCE-CORE ISceUserAuthenticationMgr object when a
**              challenge has been received which realm is not associated with
**              any known data.
**
**  PARAMETERS:
**              pAuth         - the authentication service reference from SCE.
**              rstrRealm     - the real of interest.
**              rstrProtocol  - the protocol of interest.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvChallenged( IN MXD_GNS::ISceUserAuthentication* pAuth,
                           IN const MXD_GNS::CString& rstrRealm,
                           IN const MXD_GNS::CString& rstrProtocol )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;
   MXD_GNS::ISceUserAuthentication *pSceUsrAuth;
   mxt_result res;
   const CFGUSR *pCfgUsr;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rstrProtocol );

   /* Make sure this is for a known authentication and retrieve the corresponding
   ** user resource.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
      if ( (pRes == NULL) && (pCfgUsr != NULL) )
      {
         res = pCfgUsr->in.pSceUsrCfg->QueryIf( &pSceUsrAuth );
         if ( MX_RIS_S( res ) && ( pSceUsrAuth != NULL ) )
         {
            if ( pSceUsrAuth == pAuth )
            {
               pRes = mpResUser[uIx];
            }
            pSceUsrAuth->ReleaseIfRef();
            pSceUsrAuth = NULL;
         }
      }
   }
   /* If we could locate a valid resource for this authentication,
   ** map the event and send it to the FSM.
   */
   if ( pRes != NULL )
   {
      CEEVT eEvt = eCEEVT_INV;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         CeResUser *pUser = reinterpret_cast<CeResUser *>
            ( pRes->resourceAddress() );
         pUser->resourceSetRealm( rstrRealm );

         fsmMapEvent( pRes->resourceFsm(),
                      static_cast<BOS_UINT32> ( eCEEVT_REG_NET_CHALLENGE ),
                      eEvt );

         fsmProcessEvt( pRes->resourceGuid(),
                        eEvt );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvSvrAuthenticationFailed
**
**  PURPOSE:    Callback from SCE-CORE ISceUserAuthenticationMgr object when the
**              server fails to authenticate itself by providing an invalid
**              "response-digest" in a 2xx response.
**
**  PARAMETERS:
**              pAuth         - the authentication service reference from SCE.
**              rstrProtocol  - the protocol of interest.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvSvrAuthenticationFailed( IN MXD_GNS::ISceUserAuthentication* pAuth,
                                        IN const MXD_GNS::CString& rstrProtocol )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;
   MXD_GNS::ISceUserAuthentication *pSceUsrAuth;
   mxt_result res;
   const CFGUSR *pCfgUsr;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rstrProtocol );

   /* Make sure this is for a known authentication and retrieve the corresponding
   ** user resource.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
      if ( (pRes == NULL) && (pCfgUsr != NULL) )
      {
         res = pCfgUsr->in.pSceUsrCfg->QueryIf( &pSceUsrAuth );
         if ( MX_RIS_S( res ) && ( pSceUsrAuth != NULL ) )
         {
            if ( pSceUsrAuth == pAuth )
            {
               pRes = mpResUser[uIx];
            }
            pSceUsrAuth->ReleaseIfRef();
            pSceUsrAuth = NULL;
         }
      }
   }
   
   /* If we could locate a valid resource for this authentication,
   ** map the event and send it to the FSM.
   */
   if ( pRes != NULL )
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eCEEVT_REG_NET_SRV_FAIL ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvAuthenticationSuccess
**
**  PURPOSE:    Callback from SCE-CORE ISceUserAuthenticationMgr object when the
**              application has received a next-nonce in the last valid response
**              to a challenged request.
**
**  PARAMETERS:
**              pAuth         - the authentication service reference from SCE.
**              rstrProtocol  - the protocol of interest.
**              opqChallengeId - the identifier representing the last updated
**                               challenge information based on the next-nonce
**                               value.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvAuthenticationSuccess( IN MXD_GNS::ISceUserAuthentication* pAuth,
                                      IN const MXD_GNS::CString& rstrProtocol,
                                      IN mxt_opaque opqChallengeId )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;
   MXD_GNS::ISceUserAuthentication *pSceUsrAuth;
   mxt_result res;
   const CFGUSR *pCfgUsr;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rstrProtocol );

   /* Make sure this is for a known authentication and retrieve the corresponding
   ** user resource.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
      if ( (pRes == NULL) && (pCfgUsr != NULL) )
      {
         res = pCfgUsr->in.pSceUsrCfg->QueryIf( &pSceUsrAuth );
         if ( MX_RIS_S( res ) && ( pSceUsrAuth != NULL ) )
         {
            if ( pSceUsrAuth == pAuth )
            {
               pRes = mpResUser[uIx];
            }
            pSceUsrAuth->ReleaseIfRef();
            pSceUsrAuth = NULL;
         }
      }
   }
   /* If we could locate a valid resource for this authentication,
   ** map the event and send it to the FSM.
   */
   if ( pRes != NULL )
   {
      CEEVT eEvt = eCEEVT_INV;
      mxt_opaque challengeId;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         CeResUser *pUser = reinterpret_cast<CeResUser *>
            ( pRes->resourceAddress() );

         /* If this is some new challenge information that we should worry about.
         */
         if ( !(pUser->resourceChallengeId ()) ||
              ( (pUser->resourceChallengeId ()) &&
                !pAuth->IsChallengeInfoEqual( opqChallengeId,
                                              pUser->resourceChallengeId ()) ))
         {
            /* Update the nonce information
            */
            MXD_GNS::CString rNonceAuth;
            
            pAuth->GetChallengeNonceInfo( opqChallengeId, rNonceAuth);
            pUser->resourceSetNonce( rNonceAuth );
            
            /* Delete any already saved information.
            */
            if ( pUser->resourceChallengeId () )
            {
               pAuth->DeleteChallengeInfo( pUser->resourceChallengeId () );
            }
            /* Create a copy of the challenge that will survive beyond this callback.
            */
            challengeId = pAuth->CloneChallengeInfo( opqChallengeId );
            fsmMapEvent( pRes->resourceFsm(),
                         static_cast<BOS_UINT32> ( eCEEVT_REG_NET_NEXT_NONCE ),
                         eEvt );

            fsmProcessEvt( pRes->resourceGuid(),
                           eEvt,
                           reinterpret_cast<void *> ( &challengeId ) );
         }
      }
   }
}



/***********************************************************************************
**  FUNCTION:   CeMain::EvNonceEmployed
**
**  PURPOSE:    Callback from SCE-CORE ISceUserAuthenticationMgr object when the
**              application has updated a request with a nonce value
**
**  PARAMETERS:
**              pAuth         - the authentication service reference from SCE.
**              opqChallengeId - the identifier representing the last updated
**                               challenge information based on the next-nonce
**                               value.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvNonceEmployed( IN MXD_GNS::ISceUserAuthentication* pAuth,
                              IN mxt_opaque opqChallengeId )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;
   MXD_GNS::ISceUserAuthentication *pSceUsrAuth;
   mxt_result res;
   const CFGUSR *pCfgUsr;

   /* Make sure this is for a known authentication and retrieve the corresponding
   ** user resource.
   */
   for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
   {
      pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
      if ( (pRes == NULL) && (pCfgUsr != NULL) )
      {
         res = pCfgUsr->in.pSceUsrCfg->QueryIf( &pSceUsrAuth );
         if ( MX_RIS_S( res ) && ( pSceUsrAuth != NULL ) )
         {
            if ( pSceUsrAuth == pAuth )
            {
               pRes = mpResUser[uIx];
            }
            pSceUsrAuth->ReleaseIfRef();
            pSceUsrAuth = NULL;
         }
      }
   }
   
   /* If we could locate a valid resource for this authentication,
   ** map the event and send it to the FSM.
   */
   if ( pRes != NULL )
   {
      MXD_GNS::CString rNonce;
      MXD_GNS::CString rNonceAuth;
      CEEVT eEvt = eCEEVT_INV;
      mxt_opaque challengeId;

      /*Retrieve the nonce from the challenge and the last succesfull nonce*/       
      pAuth->GetChallengeNonceInfo( opqChallengeId, rNonceAuth);

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         CeResUser *pUser = reinterpret_cast<CeResUser *>
            ( pRes->resourceAddress() );

         pUser->resourceGetNonce(rNonce);

         /* If the challenge information has changed then trigger an authentication 
         ** update if we've already seen this nonce before.
         */
         if ( (rNonce == rNonceAuth) && 
              ( (pUser->resourceChallengeId ()) &&
                !pAuth->IsChallengeInfoEqual( opqChallengeId,
                                              pUser->resourceChallengeId ()) ))
         {
            /* Delete any already saved information.
            */
            if ( pUser->resourceChallengeId () )
            {
               pAuth->DeleteChallengeInfo( pUser->resourceChallengeId () );
            }
            /* Create a copy of the challenge that will survive beyond this callback.
            */
            challengeId = pAuth->CloneChallengeInfo( opqChallengeId );

            fsmMapEvent( pRes->resourceFsm(),
                         static_cast<BOS_UINT32> ( eCEEVT_REG_NET_NEXT_NONCE ),
                         eEvt );

            fsmProcessEvt( pRes->resourceGuid(),
                           eEvt,
                           reinterpret_cast<void *> ( &challengeId ) );
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvUnhandledRequest
**
**  PURPOSE:    Callback from SCE-CORE ISceBasicExtensionMgr object when an
**              unhandled request is received and has not been associated with
**              any existing service.
**
**  PARAMETERS:
**              pComponent            - the basic extension service ref. from SCE.
**              pUserConfig           - the user this applies to.
**              pServerEventCtrl      - reference of object to send the response.
**              rRequest              - the actual data received.
**              pAdditionalParameters - additional parameters.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvUnhandledRequest(
                   IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                   IN MXD_GNS::ISceUserConfig* pUserConfig,
                   IN MXD_GNS::ISipServerEventControl* pServerEventCtrl,
                   IN const MXD_GNS::CSipPacket& rRequest,
                   IN MXD_GNS::IEComUnknown* pAdditionalParameters )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pAdditionalParameters );

   /* Locate the component which was registered and which is receiving
   ** this callback.
   **
   ** Note that we may associate a component (would be an 'in-dialog'
   ** request, or a request associated with a component we created) or we
   ** may not (would be an 'out-of-dialog' request, or a request that is
   ** associated with the 'root' component).
   */
   CeResBase *pRes = NULL;
   BOS_UINT32 uGuid = locatePmRes( reinterpret_cast<void *> ( pComponent ),
                                   BOS_FALSE );

   /* Determine the type of request that we are okay to handle, reject
   ** any that we do not want/need.
   */
   switch ( rRequest.GetRequestLine()->GetMethod() )
   {
      /* 'OPTIONS' request lead to synchronous response from Call Engine containing
      ** the identified acceptable feature supported.
      */
      case MXD_GNS::eSIP_METHOD_OPTIONS:
      {
         /* If this is 'out-of-dialog', try to match it to a known user.
         */
         if ( uGuid == guCeInvalidGuid )
         {
            /* If a user configuration was provided already, let's see if we can
            ** find it.
            */
            if ( pUserConfig )
            {
               BOS_UINT32 uIx;
               const CFGUSR *pCfgUsr;
               /* Loop through the known configured user to identify the one
               ** corresponding to this config.
               */
               for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
               {
                  if ( (mpResUser[uIx] != NULL) && (uGuid == guCeInvalidGuid) )
                  {
                     pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
                     if ( (pCfgUsr != NULL) &&
                          (pCfgUsr->in.pSceUsrCfg == pUserConfig) )
                     {
                        uGuid = mpResUser[uIx]->resourceGuid();
                        break;
                     }
                  }
               }
            }
            /* No user configuration provided, try to locate the correct one
            ** from the URI information.
            */
            else
            {
               /* Note: Only check for SIP URI as target user, typically this is
               **       what we would receive for the service of interest.
               */
               BOS_UINT32 uIx;
               const CFGUSR *pCfgUsr;
               const MXD_GNS::CSipUri *pUri =
                                          rRequest.GetRequestLine()->GetSipUri();
               if ( pUri )
               {
                  for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
                  {
                     if ( (mpResUser[uIx] != NULL) && (uGuid == guCeInvalidGuid) )
                     {
                        pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
                        if ( (pCfgUsr != NULL) &&
                             !strcmp(
                                reinterpret_cast<const char *> (
                                          pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                                pUri->GetUser().GetString().CStr() ) )
                        {
                           uGuid = mpResUser[uIx]->resourceGuid();
                           break;
                        }
                     }
                  }
               }
            }
         }
         /* If this is not a user resource, associate the correponding
         ** user resource with it since this latter will take care of processing
         ** the request.
         */
         else if ( cfgGetType( uGuid ) != eCFGTYPE_USR )
         {
            if ( locateRes ( uGuid, pRes ) )
            {
               uGuid = pRes->resourceParent();
            }
         }

         /* If the resource can be located.  Process the request in its FSM.
         */
         if ( (uGuid != guCeInvalidGuid) && locateRes( uGuid, pRes ) )
         {
            CEEVT eMapEvt;

            fsmMapEvent( pRes->resourceFsm(),
                         static_cast<BOS_UINT32> ( eCEEVT_REG_NET_OPTIONS ),
                         eMapEvt );

            /* We only need to pass the server event control to keep track of
            ** where to send the response to.
            */
            fsmProcessEvt( uGuid,
                           eMapEvt,
                           reinterpret_cast<void *> ( pServerEventCtrl ) );
         }
         /* Otherwise, reject the request.
         */
         else
         {
            pComponent->RejectRequest( pUserConfig,
                                       pServerEventCtrl,
                                       rRequest );
         }
      }
      break;

      /* 'INFO' request may be used to implicitely query the status of a dialog.
      ** The Call Engine responds automatically to the INFO request on behalf of
      ** the user.  Note that this is the only usage of the INFO method that we
      ** support at this time.  If further process is necessary, then it would need
      ** to be implemented properly (see the handling of the OPTIONS request as
      ** example).
      */
      case MXD_GNS::eSIP_METHOD_INFO:
      {
         if ( uGuid != guCeInvalidGuid )
         {
            /* This request came on a valid call.  Respond to it with positive
            ** final response.
            */
            if ( cfgGetType( uGuid ) == eCFGTYPE_CALL )
            {
               BOS_BOOL bHandled = BOS_FALSE;
               CEEVT eEvt = eCEEVT_INV;
               CEMAINDATA data;

               memset ( static_cast<void *> (&data),
                        0,
                        sizeof ( CEMAINDATA ) );

               /* If there is a payload associated with this event, pass it up
               ** to the actual handler for payload processing.
               */
               if ( rRequest.GetPayload() && locateRes(uGuid, pRes) )
               {
                  data.uSize = rRequest.GetPayload()->GetSize();
                  data.puData =
                     utilDataDup( rRequest.GetPayload()->GetFirstIndexPtr(),
                                  rRequest.GetPayload()->GetSize() );

                  bHandled = BOS_TRUE;
               }
               else
               {
                  /* If there is an Event header associated with this event , pass it up
                  ** to the actual handler for further processing.
                  */
                  mxt_result res;
                  const MXD_GNS::CSipHeader *pEvt =
                           rRequest.GetHeaderList().Get( MXD_GNS::eHDR_EVENT, res );
                  if ( MX_RIS_S( res ) && ( pEvt != NULL ) )
                  {
                     data.uSize = pEvt->GetEvent().GetString().GetSize();
                     data.puData = 
                        utilDataDup( (BOS_UINT8*)pEvt->GetEvent().GetString().CStr(),
                                     pEvt->GetEvent().GetString().GetSize() );
                     bHandled = BOS_TRUE;
                  }
               }

               if (bHandled)
               {
                  /* If the resource can be located.  Process the request in its FSM.
                  */
                  if ( (uGuid != guCeInvalidGuid) && locateRes( uGuid, pRes ) )
                  {
                     fsmMapEvent( pRes->resourceFsm(),
                                  static_cast<BOS_UINT32> ( eCEEVT_CALL_NET_INFO ),
                                  eEvt );

                     fsmProcessEvt( pRes->resourceGuid(),
                                    eEvt,
                                    static_cast<void *> (&data) );
                  }
               }
               
               /* Always accept this request, even if we end up not acting on the
               ** payload (if any) provided, it is not worth rejecting it since it
               ** is not interfering with the call state anyways.
               */
               pServerEventCtrl->SendResponse( MXD_GNS::uOK,
                                               NULL,
                                               NULL,
                                               NULL  );
            }
            /* This request came on any other resource that is not a valid call.
            ** Reject it.
            */
            else
            {
               pComponent->RejectRequest( pUserConfig,
                                          pServerEventCtrl,
                                          rRequest );
            }
         }
         else
         {
            /* This request came on a resource that cannot be identified, this is
            ** likely not a valid call any longer.  Answer it with an appropriate
            ** final response in this situation.
            */
            pServerEventCtrl->SendResponse(
                     MXD_GNS::uCALL_LEG_TRANSACTION_DOES_NOT_EXIST,
                     NULL,
                     NULL,
                     NULL  );
         }
      }
      break;

      /* 'NOTIFY' requests that are unhandled by the SCE are 'out-of-band'/'blind'
      ** information which may be taken into consideration at this time in some
      ** special scenario.  Note that such notification should not be for
      ** message waiting indication (i.e. blind message waiting) as there is
      ** already a SCE component that would take care of those if configured to
      ** do so, therefore here we expect application specific requests.  At this
      ** time, the application specific notification request that are handled
      ** are: user-agent profile notification - used for call forwarding status.
      */
      case MXD_GNS::eSIP_METHOD_NOTIFY:
      {
         BOS_BOOL bHandled = BOS_FALSE;
         const CFGUSR *pCfgUsr = NULL;

#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
         if ( uGuid == guCeInvalidGuid )
         {
            /* If a user configuration was provided already, let's see if we can
            ** find it.
            */
            if ( pUserConfig )
            {
               BOS_UINT32 uIx;
               
               /* Loop through the known configured user to identify the one
               ** corresponding to this config.
               */
               for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
               {
                  if ( (mpResUser[uIx] != NULL) && (uGuid == guCeInvalidGuid) )
                  {
                     pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
                     if ( (pCfgUsr != NULL) &&
                          (pCfgUsr->in.pSceUsrCfg == pUserConfig) )
                     {
                        uGuid = mpResUser[uIx]->resourceGuid();
                        break;
                     }
                  }
               }
            }
            /* No user configuration provided, try to locate the correct one
            ** from the URI information.
            */
            else
            {
               /* Note: Only check for SIP URI as target user, typically this is
               **       what we would receive for the service of interest.
               */
               BOS_UINT32 uIx;
               const MXD_GNS::CSipUri *pUri =
                                          rRequest.GetRequestLine()->GetSipUri();
               if ( pUri )
               {
                  for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
                  {
                     if ( (mpResUser[uIx] != NULL) && (uGuid == guCeInvalidGuid) )
                     {
                        pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
                        if ( (pCfgUsr != NULL) &&
                             !strcmp(
                                reinterpret_cast<const char *> (
                                          pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                                pUri->GetUser().GetString().CStr() ) )
                        {
                           uGuid = mpResUser[uIx]->resourceGuid();
                           break;
                        }
                     }
                  }
               }
            }
         }
         /* If this is not a user resource, associate the correponding
         ** user resource with it since this latter will take care of processing
         ** the request.
         */
         else if ( cfgGetType( uGuid ) != eCFGTYPE_USR )
         {
            if ( locateRes ( uGuid, pRes ) )
            {
               uGuid = pRes->resourceParent();
               pCfgUsr = CFG_USR( uGuid );
            }
         }

         /* If the resource can be located.
         */
         if ( (uGuid != guCeInvalidGuid) && locateRes( uGuid, pRes ) )
         {
            /* Check to see whether we want to indeed process the blind
            ** notification.
            */
            mxt_result res;
            const MXD_GNS::CSipHeader *pEvt =
                     rRequest.GetHeaderList().Get( MXD_GNS::eHDR_EVENT, res );

            if ( MX_RIS_S( res ) && ( pEvt != NULL ) )
            {
               /* Only accept event for the ua-profile package, and only if
               ** configuration enables it.
               */
               if (  (( pCfgUsr != NULL ) &&
                     ( pCfgUsr->xtl.usr.net.uapEvt.bUnsolNtfy ))&&
                     !strCmpNoCase( pEvt->GetEvent().GetString().CStr(),
                                   PM__SIGNALING_PROF_TYPE_EVENT )  )
               {
                  bHandled = BOS_TRUE;            
               }
            }

            /* If process is accepted, create a temporary subscription resource
            ** which would handle the event and forward the event to it for
            ** further processing.
            */
            if ( bHandled )
            {
               BOS_UINT32 uResGuid = allocateGuid();

               if ( (uResGuid != guCeInvalidGuid) &&
                    (allocateRes( CeResBase::eCERESTYPE_SUB,
                                  uResGuid,
                                  uGuid,
                                  BOS_FALSE ) != guCeInvalidGuid) )
               {
                  if ( cfgAddEvt( uResGuid, uGuid ) == eCFGSTATUS_SUCCESS )
                  {
                     /* Resource creation is successful, set it up accordingly such
                     ** that FSM can take over.
                     */
                     locateRes( uResGuid, pRes );
                     if ( pRes->resourceGender() == CeResBase::eCERESTYPE_SUB )
                     {
                        CeResSub *pSub = reinterpret_cast<CeResSub *>
                           ( pRes->resourceAddress() );

                        pSub->resourceEvtType( eCCTKSUBEVT_UAP );
                        pSub->resourceReleaseImmediate();
                        pRes->resourcePushFsm( CeFsmBase::eCEFSMTYPE_SUB );
                        pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_IDLE );
                     }
                  }
                  else
                  {
                     if ( pRes->resourceGender() == CeResBase::eCERESTYPE_SUB )
                     {
                        CeResSub *pSub = reinterpret_cast<CeResSub *>
                           ( pRes->resourceAddress() );
                        pSub->resourceTerminate();
                        bHandled = BOS_FALSE;
                     }
                  }
               }
               else
               {
                  bHandled = BOS_FALSE;
               }
            }
         }
#endif /* #if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS ) */
         /* Process at the call engine level and respond to the request.
         */
         if ( bHandled )
         {
            CEEVT eEvt = eCEEVT_INV;
            CEMAINDATA data;

            memset ( static_cast<void *> (&data),
                     0,
                     sizeof ( CEMAINDATA ) );

            /* If we handle the event, the resource will be set to the correct
            ** one now, we can simply post the event for the Call Engine to
            ** process it accordingly.
            */
            if ( rRequest.GetPayload() )
            {
               data.uSize = rRequest.GetPayload()->GetSize();
               data.puData =
                  utilDataDup( rRequest.GetPayload()->GetFirstIndexPtr(),
                               rRequest.GetPayload()->GetSize() );
            }

            fsmMapEvent( pRes->resourceFsm(),
                         static_cast<BOS_UINT32> ( eCEEVT_SUB_NET_NOTIFY ),
                         eEvt );

            fsmProcessEvt( pRes->resourceGuid(),
                           eEvt,
                           static_cast<void *> (&data) );

            /* Ensure that we setup the packet observer for this component at this
            ** time.  It is likely that this component was not associated with
            ** our packet observer, in such case we would not be able to apply the
            ** behavior intended described below.
            */
            pComponent->SetSipPacketObserver(
               reinterpret_cast<MXD_GNS::ISceSipPacketObserver *>(
                                                         pmGetPktObs() ) );

            /* We accept this request, but we cannot send a success response at
            ** this time because it would create an implicit dialog per RFC 3265
            ** operations and the SCE framework is not capable of supporting such
            ** behavior.  What we do instead is to reject this request with a
            ** specific status and reason information and when the response is seen
            ** by the observer upon being generated out by the stack, we will at
            ** that time transform this specific negative response into a positive
            ** one.
            ** This behavior is basically a work around for a framework limitation,
            ** it is subject to review and change as the framework evolves in the
            ** future.
            */
            pServerEventCtrl->SendResponse(
                                 MXD_GNS::uBAD_EXTENSION,
                                 PM__SIGNALING_ACCEPT_BLIND_NOT_REASON_INT,
                                 NULL,
                                 NULL  );
         }
         else
         {
            /* Let the SCE framework decide how best to reject this particular
            ** request.
            */
            pComponent->RejectRequest( pUserConfig,
                                       pServerEventCtrl,
                                       rRequest );
         }
      }
      break;
      case MXD_GNS::eSIP_METHOD_REGISTER:
      {
         /* Receiving a REGISTER request for a remote user.
         **
         ** We need to apply the following:
         **
         **   - Extract the user identification from the Contact header
         **     of the request.
         **   - Check whether this user is already known to us or not.
         **   - See if we can accept the request (for a new user, that means
         **     we are able to create a user resource to associate it with)
         **   - Respond to the registration request (either accept it or not).
         */
         BOS_BOOL bAccepted = BOS_FALSE;
         BOS_BOOL bAuthFailure = BOS_FALSE;
         BOS_BOOL b403Response = BOS_FALSE;  /* given credential is invalid */
         MXD_GNS::CSipHeader *pAuthHeader = NULL;
         BOS_BOOL bRemoteUser = BOS_FALSE;
         BOS_UINT32 uIx;
         BOS_UINT32 uGuid = guCeInvalidGuid;

         mxt_result res;
         const MXD_GNS::CSipHeader *pContact = NULL;
         const MXD_GNS::CSipHeader *pFrom = NULL;
         const BOS_UINT8 *puDisplayName = NULL;
         const BOS_UINT8 *puUserName = NULL;
         const BOS_UINT8 *puHost = NULL;
         BOS_UINT16 uPort = 0;
         BOS_UINT32 uExpires = (BOS_UINT32)(-1);

#if CCTKCFG_REMOTE_USER_CONTROL
         if ( CFM_CFG.pSys->xtl.remUsr.gen.bRemUsrSupport )
         {
            pContact = rRequest.GetHeaderList().Get( MXD_GNS::eHDR_CONTACT, res );
            pFrom = rRequest.GetHeaderList().Get( MXD_GNS::eHDR_FROM, res );
            
            /* Get 'DisplayName', 'UserName', 'Host', 'Port' and 'Expires' 
            ** from the request */
            if ( ( pContact != NULL ) && ( pFrom != NULL) )
            {
               const MXD_GNS::CNameAddr contactNameAddr = pContact->GetContact();
               const MXD_GNS::CNameAddr fromNameAddr = pFrom->GetFrom();
               puDisplayName = reinterpret_cast<const BOS_UINT8 *> (
                                  fromNameAddr.GetDisplayName().GetString().CStr() );

               const MXD_GNS::CSipUri *pUri =
                  reinterpret_cast<const MXD_GNS::CSipUri *> ( contactNameAddr.GetUri() );
               if ( pUri )
               {
                  puUserName = 
                     reinterpret_cast<const BOS_UINT8 *> (
                                    pUri->GetUser().GetString().CStr() );

                  puHost = 
                     reinterpret_cast<const BOS_UINT8 *> (
                                    pUri->GetHostPort().GetHost().CStr() );
                  uPort = pUri->GetHostPort().GetPort();
               }
               
               /* look for 'expires' value in CONTACT header */
               const MXD_GNS::CToken *pExpiresToken = pContact->GetParam( 
                  MXD_GNS::szHDRPARAM_EXPIRES );
               if ( pExpiresToken != NULL )
               {
                  pExpiresToken->GetUint32( 
                     reinterpret_cast<uint32_t &> ( uExpires ) );
               }
               else 
               {
                  /* no "expired" in contact, check if there is a Expires header */
                  const MXD_GNS::CSipHeader *pExpireHdr =
                     rRequest.GetHeaderList().Get( MXD_GNS::eHDR_EXPIRES, res );                  
                  if ( pExpireHdr )
                  {
                     (pExpireHdr->GetExpires()).GetUint32(
                        reinterpret_cast<uint32_t &> ( uExpires ) );
                  }
               }
            }
            
            /* Locate any current user with the same user-name credential.
            **
            ** Note that at this time, we do not support remote user roaming
            ** per say which would be the case if the host contact of the
            ** remote user has changed but not the actual user name.
            **
            ** Note: Remote user roaming would not be difficult to do, it would
            **       simply be matter of updating the current user credentials
            **       that we keep track of.
            */
            for ( uIx = 0; uIx < guCeMainMaxUser; uIx++ )
            {
               if ( (mpResUser[uIx] != NULL) && (uGuid == guCeInvalidGuid) )
               {
                  const CFGUSR *pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
                  if ( (pCfgUsr != NULL) &&
                       !strcmp(
                          reinterpret_cast<const char *> (
                                    pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                          reinterpret_cast<const char *> (
                                    puUserName) ) )
                  {
                     uGuid = mpResUser[uIx]->resourceGuid();
                     bAccepted = BOS_TRUE;
                     bRemoteUser = mpResUser[uIx]->resourceIsRemUsr();
                     break;
                  }
               }
            }
            
            CCTK_TRACED1( "EvUnhandledRequest() - handling REGISTER for user (%x)", uGuid );               
            /* Check if this is a 'UN-REGISTER' request */
            if ( uExpires == 0 )
            {
               if ( bAccepted &&  bRemoteUser && uGuid != guCeInvalidGuid )
               {
                  CCTK_TRACEI1( "EvUnhandledRequest() - UN-REGISTER uExpires = 0 (%x).", uGuid );
                  /* Issue 'REMOVE' event to the FSM.
                  */
                  ceNotify ( uGuid,
                             eCEEVT_REG_NET_USR_REMOVE,
                             NULL );
               }
            }
            else
            {
               /* This is a 'REGISTER' request */
            
               /* If no current user share the same credential, this would
               ** be a new user registration possibly.  
               ** Check if there is any free user block to accept.
               */
               if ( !bAccepted )
               {
                  for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
                  {
                     if ( (mpResUser[uIx] != NULL) &&
                          /* Any free user block would do.
                          */
                          (mpResUser[uIx]->resourceGuid() == guCeInvalidGuid) )
                     {
                        bAccepted = BOS_TRUE;
                        break;
                     }
                  }
               }

               /* Check if new SIP client meets the current admission mode */
               if ( bAccepted )
               {
                  switch ( CFM_CFG.pSys->xtl.remUsr.gen.eAdmMode )
                  {
                     case eCCTKREMUSRADMMODE_ALL:
                     break;
                     case eCCTKREMUSRADMMODE_LANUSR:
                     /* Check if the SIP client is coming from LAN. */
                     {
                        BOS_IP_ADDRESS usrAddr;
                        bosIpAddrCreateFromStr(
                           reinterpret_cast<const char *>(puHost), &usrAddr );
                        if ( CFM_CFG.pSys->xtl.sec.nat.rg.ctl.pNatIsLanAddrFunc && 
                             ( CFM_CFG.pSys->xtl.sec.nat.rg.ctl.pNatIsLanAddrFunc ( 
                                 &usrAddr ) == BOS_FALSE )
                           )
                        {
                           bAccepted = BOS_FALSE;                   
                        }
                     }
                     break;
                     case eCCTKREMUSRADMMODE_PRECONFIG:
                     /* Check if the SIP client is a pre-Configured user. */
                     /* A CCTK user is already created for pre-Configured user
                     ** during startup and the uGuid should be found. */
                     {
                        if ( uGuid == guCeInvalidGuid )
                        {
                           bAccepted = BOS_FALSE; 
                        }
                     }
                     break;
                     case eCCTKREMUSRADMMODE_NONE:
                     default:
                     /* Simply reject all registration */
                     {
                        bAccepted = BOS_FALSE; 
                     }
                     break;
                  }
               }

               /*  Check Authoriziation */
               if ( bAccepted == BOS_TRUE )
               {
                  MXD_GNS::CString strUserName;
                  MXD_GNS::CString strPassword;
                  BOS_BOOL bCheckCredential = BOS_TRUE;
                  BOS_BOOL bDftCredential = BOS_TRUE;

                  /* Get the username and password for this user */
                  if ( (uGuid != guCeInvalidGuid) && bRemoteUser )
                  {
                     CFGUSR *pCfgUsr = const_cast <CFGUSR *> ( CFG_USR( uGuid ) );
                     if ( pCfgUsr->xtl.remUsr.cfg.uIMPIIndexRef != 0 )
                     {
                        bDftCredential = BOS_FALSE;
                        strUserName = reinterpret_cast<char *>( pCfgUsr->xtl.remUsr.cfg.pcIMPIUserId );
                        strPassword = reinterpret_cast<char *>( pCfgUsr->xtl.remUsr.cfg.pcIMPIPassword );
                     }
                  }
                  
                  /* use default credential */
                  if ( bDftCredential )
                  {
                     if ( CFM_CFG.pSys->xtl.remUsr.gen.uDftIMPIIndexRef == 0 )
                     {
                        bCheckCredential = BOS_FALSE;
                     }
                     else
                     {
                        strUserName = reinterpret_cast<char *>( CFM_CFG.pSys->xtl.remUsr.gen.pcDftIMPIUserId );
                        strPassword = reinterpret_cast<char *>( CFM_CFG.pSys->xtl.remUsr.gen.pcDftIMPIUserPassword );
                     }
                  }
                  CCTK_TRACED4( "EvUnhandledRequest() - Authentication check - doCheck(%d) IsDftCred(%d) '%s:%s'",
                     bCheckCredential, bDftCredential, strUserName.CStr(), strPassword.CStr() );                  
                  if ( bCheckCredential )
                  {
                     MXD_GNS::ISipContext* pContext = NULL;
                     MXD_GNS::ISipStatelessDigestServerAuthSvc *pAuthSvc = NULL;
                     pComponent->GetSipContext( &pContext );
                     if ( pContext )
                     {
                        mxt_result res;                        
                        /* Look for Server Authentication Service */
                        res = pContext->QueryIf(&pAuthSvc);
                        if ( pAuthSvc == NULL )
                        {
                           /* Attach Server Authentication Service if there is none */
                           res = pContext->AttachService( MXD_GNS::CLSID_CSipStatelessDigestServerAuthSvc );
                           if ( res == resFE_FAIL )
                           {
                              CCTK_TRACEI0( "CeMain::EvUnhandledRequest - Fail to attach Server Authentication Service!" );
                           }
                           res = pContext->QueryIf(&pAuthSvc);
                        }
                        if ( MX_RIS_S( res ) && ( pAuthSvc != NULL ) )                        
                        {
                           MXD_GNS::CString strReqUserName;
                           
                           /* Configure the Server Authentication Service */
                           pAuthSvc->SetSupportedRealm( gpIMGRealm );
                           pAuthSvc->SetRequestedQualityOfProtection( MXD_GNS::ISipStatelessDigestServerAuthSvc::eAuth );
                           
                           /* check if there is a credential */
                           res = pAuthSvc->ContainsCredentials( rRequest, strReqUserName ); 
                           if ( MX_RIS_F( res ) )
                           {
                              /* Send 401 if there is not 'Authorization' Header or if nonce is stale */
                              bool bIsStale = ( res == MXD_GNS::resFE_SIPCORESVC_STALE_NONCE );

                              /* Required by EvUnhandledRequest() to send response thru
                               * ISipServerEventControl.  (Otherwise, result in leaking interface)
                               *
                               * A new API is added to generate a Challenge Header from the
                               * Stateless Authentication Service, so the response can be 
                               * send thru ISipServerEventControl. */              
                              pAuthSvc->CreateChallengeHeader( rRequest,
                                 MXD_GNS::eHDR_WWW_AUTHENTICATE,
                                 MXD_GNS::ISipStatelessDigestServerAuthSvc::eMD5,
                                 bIsStale,
                                 &pAuthHeader );      

                              bAccepted = BOS_FALSE;
                           }
                           else if ( strUserName != strReqUserName )
                           {
                              b403Response = BOS_TRUE;
                              bAccepted = BOS_FALSE;
                           }
                           else 
                           {
                              /* Verify the credentail */
                              MXD_GNS::CString strHashA1;
                              pAuthSvc->GetMd5AlgoHash( strUserName, strPassword, strHashA1 ); 
                              res = pAuthSvc->VerifyAuthentication( rRequest, strHashA1 ); 
                              if ( res != resS_OK )
                              {
                                 b403Response = BOS_TRUE;
                                 bAccepted = BOS_FALSE;
                              }
                           }
                           pAuthSvc->ReleaseIfRef();
                           pAuthSvc = NULL;
                        }
                        else
                        {
                           CCTK_TRACEI0( "CeMain::EvUnhandledRequest - Server Authentication Service not available!" );
                        }
                        pContext->ReleaseIfRef();
                        pContext = NULL;
                        
                        /* Not accepting, set the bAuthFailure flag */
                        bAuthFailure = !bAccepted;
                     }
                  }
               }
               /* This is an accepted 'REGISTER' with an existing user.
               ** This can be a REGISTER refresh or the first REGISTER for
               ** pre-Configured user. */
               if ( bAccepted && bRemoteUser && (uGuid != guCeInvalidGuid) )
               {
                  /* check if AOR changes */
                  CFGUSR *pCfgUsr = const_cast <CFGUSR *> ( CFG_USR( uGuid ) );
                  if ( ( pCfgUsr->xtl.reg.gen.aor.uPort != uPort ) || 
                       ( strncmp( reinterpret_cast<const char *> (puHost),
                           reinterpret_cast<const char *> (pCfgUsr->xtl.reg.gen.aor.pcAddr),
                           strlen( reinterpret_cast<const char *> (puHost) ) ) ) )
                  {
                     CCTK_TRACEI1( "CeMain::EvUnhandledRequest - User (%x) AOR change detected!", uGuid );
                     /* AOR has changed, notify the upper application to update
                     ** the current user. */
                     pCfgUsr->xtl.reg.gen.aor.uPort = uPort;
                     BRCM_DELETE_ARRAY (pCfgUsr->xtl.reg.gen.aor.pcAddr);
                     pCfgUsr->xtl.reg.gen.aor.pcAddr = utilStrDup ( puHost );
                     
                     /* Since we are the registrar for this user, mark it as
                     ** registered always.
                     */
                     locateRes( uGuid, pRes );                  
                     pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGED );

                     /* Issue 'Update' event to the FSM.
                     */
                     ceNotify ( uGuid,
                                eCEEVT_REG_NET_USR_ADD,
                                NULL );
                  }
               }

               /* This is an accepted 'REGISTER' with no current user.
               **  Need to create the support for this user at the CCTK (and
               ** upper application) level.
               */
               if ( bAccepted && (uGuid == guCeInvalidGuid) )
               {
                  CCTK_TRACEI1( "CeMain::EvUnhandledRequest - Register new remote user (%s)!", puUserName );
                  BOS_UINT32 uNewGuid = ceGuid();
                  CCTKUSRCFG *pCctkUsrCfg;
                  CFGSTATUS eCfgStatus;

                  pCctkUsrCfg = BRCM_NEW( CCTKUSRCFG );
                  memset ( pCctkUsrCfg,
                           0,
                           sizeof ( CCTKUSRCFG ) );

                  /* Save the user name of the registered user.
                  */
                  pCctkUsrCfg->usr.net.sipName.pcUsrName = utilStrDup ( puUserName );
                  pCctkUsrCfg->usr.net.sipName.pcDisplay = utilStrDup ( puDisplayName );
                  /* Save the address of record of the registered user in order
                  ** to be able to target requests to it directly.
                  */
                  pCctkUsrCfg->reg.gen.aor.pcAddr = utilStrDup ( puHost );
                  pCctkUsrCfg->reg.gen.aor.uPort = uPort;
                  pCctkUsrCfg->remUsr.bRemUsr = BOS_TRUE;
                  
                  eCfgStatus = cfgAddUsr ( uNewGuid, pCctkUsrCfg );
                  if ( CFGSTATUS_IS_SUCCESS( eCfgStatus ) )
                  {
                     if ( allocateRes( CeResBase::eCERESTYPE_USR,
                                       uNewGuid,
                                       guCeInvalidGuid ) != guCeInvalidGuid )
                     {
                        /* Resource creation is successful, set it up accordingly
                        ** such that FSM can take over.
                        */
                        locateRes( uNewGuid, pRes );
       
                        /* Complete the user addition from the resource management
                        ** point of view.  This is needed to properly trigger the
                        ** context creation at the UASSP layer.
                        */
                        ceNotify ( pRes->resourceGuid(),
                                   eCEEVT_REG_USR_ADD_OK,
                                   NULL );

                        /* Since we are the registrar for this user, mark it as
                        ** registered always.
                        */
                        pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_REG_REGED );

                        /* Issue event to the FSM such that the upper application can
                        ** be updated as well.
                        */
                        ceNotify ( pRes->resourceGuid(),
                                   eCEEVT_REG_NET_USR_ADD,
                                   NULL );
                     }
                     else
                     {
                        /* Reject registration for failure to allocate the
                        ** CCTK user resource to handle such.
                        */
                        bAccepted = BOS_FALSE;
                     }
                  }
                  else
                  {
                     /* Reject registration for failure to allocate the
                     ** CCTK user configuration to handle such.
                     */
                     bAccepted = BOS_FALSE;
                  }
               }
            }
         }
#endif /* #if CCTKCFG_REMOTE_USER_CONTROL */ 

         /* Send final response.
         */
         if ( bAccepted )
         {
            /* Pass the Contact information back.
            */
            MXD_GNS::CHeaderList *pHdrLst = BRCM_NEW( MXD_GNS::CHeaderList );
            MXD_GNS::CSipHeader *pHdrCtc =
               BRCM_NEW( MXD_GNS::CSipHeader ) ( MXD_GNS::eHDR_CONTACT );
            
            /* Add an "expires" parameter on the response. 
            ** If there is no expires on the request use an default of 60 */
            
            MXD_GNS::CToken token(MXD_GNS::CToken::eCS_SIPHEADER_PARAM);
            token = (int)( (uExpires != (BOS_UINT32)(-1))? 
                              uExpires : CE_DEF_REMOTE_USR_REG_EXPIRE_VAL);
            
            pHdrCtc->GetContact() = pContact->GetContact();
            pHdrCtc->SetParam( MXD_GNS::szHDRPARAM_EXPIRES, token );
            pHdrLst->Append( pHdrCtc );

            pServerEventCtrl->SendResponse( MXD_GNS::uOK,
                                            NULL,
                                            pHdrLst,
                                            NULL  );
         }
         else
         {  
            /* Let the SCE framework decide how best to reject this particular
            ** request.
            */
            /* Response has been sent when there is an authentication failure */
            if ( bAuthFailure )
            {
               CCTK_TRACEI0( "CeMain::EvUnhandledRequest - Authentication failured, sending 401/403 response!" );               
               if ( b403Response )
               {
                  pServerEventCtrl->SendResponse( MXD_GNS::uFORBIDDEN,
                                                  "Authentication Failure!",
                                                  NULL,
                                                  NULL );
               }
               else
               {
                  /* General 401 response to request another REGISTER */
                  MXD_GNS::CHeaderList *pHdrLst = BRCM_NEW( MXD_GNS::CHeaderList );
                  pHdrLst->Append( pAuthHeader );
                  pServerEventCtrl->SendResponse( MXD_GNS::uUNAUTHORIZED,
                                            NULL,
                                            pHdrLst,
                                            NULL );
               }
            }
            else
            {
               CCTK_TRACEI0( "CeMain::EvUnhandledRequest - REGISTER rejected!" );                              
               pComponent->RejectRequest( pUserConfig,
                                       pServerEventCtrl,
                                       rRequest );
            }
         }
      }
      break;
      case MXD_GNS::eSIP_METHOD_MESSAGE:
      {
         /* 'MESSAGE' request can be accepted as pass through data carrier for the
         ** application to use.
         */      
         BOS_BOOL bHandled = BOS_TRUE;
#if CCTKCFG_MESSAGE_PASS_THROUGH
         /* If this is 'out-of-dialog', try to match it to a known user.
         */
         if ( uGuid == guCeInvalidGuid )
         {
            /* If a user configuration was provided already, let's see if we can
            ** find it.
            */
            if ( pUserConfig )
            {
               BOS_UINT32 uIx;
               const CFGUSR *pCfgUsr;
               /* Loop through the known configured user to identify the one
               ** corresponding to this config.
               */
               for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
               {
                  if ( (mpResUser[uIx] != NULL) && (uGuid == guCeInvalidGuid) )
                  {
                     pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
                     if ( (pCfgUsr != NULL) &&
                          (pCfgUsr->in.pSceUsrCfg == pUserConfig) )
                     {
                        uGuid = mpResUser[uIx]->resourceGuid();
                        break;
                     }
                  }
               }
            }
            /* No user configuration provided, try to locate the correct one
            ** from the URI information.
            */
            else
            {
               /* Note: Only check for SIP URI as target user, typically this is
               **       what we would receive for the service of interest.
               */
               BOS_UINT32 uIx;
               const CFGUSR *pCfgUsr;
               const MXD_GNS::CSipUri *pUri =
                                          rRequest.GetRequestLine()->GetSipUri();
               if ( pUri )
               {
                  for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
                  {
                     if ( (mpResUser[uIx] != NULL) && (uGuid == guCeInvalidGuid) )
                     {
                        pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
                        if ( (pCfgUsr != NULL) &&
                             !strcmp(
                                reinterpret_cast<const char *> (
                                          pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                                pUri->GetUser().GetString().CStr() ) )
                        {
                           uGuid = mpResUser[uIx]->resourceGuid();
                           break;
                        }
                     }
                  }
               }
            }
         }
         /* If this is not a user resource, associate the correponding
         ** user resource with it.
         */
         else if ( cfgGetType( uGuid ) != eCFGTYPE_USR )
         {
            if ( locateRes ( uGuid, pRes ) )
            {
               uGuid = pRes->resourceParent();
            }
         }

         /* If the user resource to be associated with this request can be located,
         ** continue with its processing.
         */
         if ( (uGuid != guCeInvalidGuid) && locateRes( uGuid, pRes ) )
         {
            BOS_UINT32 uResGuid = allocateGuid();

            if ( (uResGuid != guCeInvalidGuid) &&
                 (allocateRes( CeResBase::eCERESTYPE_NOT,
                               uResGuid,
                               uGuid,
                               BOS_FALSE,
                               eCCTKPUBLISH_PASSTHRUMSG ) != guCeInvalidGuid) )
            {
               if ( cfgAddEvt( uResGuid, uGuid ) == eCFGSTATUS_SUCCESS )
               {  
                  /* Resource creation is successful, set it up accordingly such
                  ** that FSM can take over.
                  */
                  locateRes( uResGuid, pRes );
                  pRes->resourcePushFsm( CeFsmBase::eCEFSMTYPE_PUB );
                  pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_PUB_IDLE );
               }
               else
               {
                  if ( pRes->resourceGender() == CeResBase::eCERESTYPE_NOT )
                  {
                     CeResNot *pNot = reinterpret_cast<CeResNot *>
                        ( pRes->resourceAddress() );
                     pNot->resourceTerminate();
                     bHandled = BOS_FALSE;
                  }
               }
            }
            else
            {
               bHandled = BOS_FALSE;
            }
         }  

         if ( !bHandled )
         {
            pComponent->RejectRequest( pUserConfig,
                                       pServerEventCtrl,
                                       rRequest );
         }
         else    
         
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */
         {
            /* Process at the call engine level and respond to the request.
            */
            CEEVT eEvt = eCEEVT_INV;
            CEMAINDATA data;

            memset ( static_cast<void *> (&data),
                     0,
                     sizeof ( CEMAINDATA ) );

            /* If we handle the event, the resource will be set to the correct
            ** one now, we can simply post the event for the Call Engine to
            ** process it accordingly.
            */
            if ( rRequest.GetPayload() )
            {
               data.uSize = rRequest.GetPayload()->GetSize();
               data.puData =
               utilDataDup( rRequest.GetPayload()->GetFirstIndexPtr(),
                            rRequest.GetPayload()->GetSize() );
            }
            data.pAdditionalNotInfo = reinterpret_cast<void *> ( pServerEventCtrl );

            fsmMapEvent( pRes->resourceFsm(),
                         static_cast<BOS_UINT32> ( eCEEVT_NOT_NET_MSG ),
                         eEvt );

            fsmProcessEvt( pRes->resourceGuid(),
                           eEvt,
                           static_cast<void *> (&data) );

            /* Ensure that we setup the packet observer for this component at this
            ** time.
            */
            pComponent->SetSipPacketObserver(
            reinterpret_cast<MXD_GNS::ISceSipPacketObserver *>(
                                                  pmGetPktObs() ) );
         }
      }
      break;


      case MXD_GNS::eSIP_METHOD_ACK:
      case MXD_GNS::eSIP_METHOD_BYE:
      case MXD_GNS::eSIP_METHOD_CANCEL:
      case MXD_GNS::eSIP_METHOD_INVITE:
      case MXD_GNS::eSIP_METHOD_PING:
      case MXD_GNS::eSIP_METHOD_PRACK:
      case MXD_GNS::eSIP_METHOD_PUBLISH:
      case MXD_GNS::eSIP_METHOD_REFER:
      case MXD_GNS::eSIP_METHOD_SERVICE:
      case MXD_GNS::eSIP_METHOD_SUBSCRIBE:
      case MXD_GNS::eSIP_METHOD_UPDATE:
      default:
      {
         /* Reject this request.  Note that some of those should never be
         ** applicable anyways.
         */
         pComponent->RejectRequest( pUserConfig,
                                    pServerEventCtrl,
                                    rRequest );
      }
      break;
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvResponse
**
**  PURPOSE:    Callback from SCE-CORE ISceBasicExtensionMgr object when a
**              response for a generic request is received.
**
**  PARAMETERS:
**              pComponent      - the network reference from SCE.
**              pUserConfig     - the associated user configuration.
**              opqTransaction  - opaque data the application may have used during
**                                creation of the corresponding request.
**              rResponse       - the SIP response packet.
**
**  RETURNS:    None
**
**  NOTES:      We do not process this callback because we do not care about
**              the result of a request that CE sends that lead to this callback
**              being invoked.
**
**              The current list of actions that would lead to a callback on this
**              method is:
**
**                    - Sending in-dialog DTMF information event via INFO
**                      message.  The outcome of sending such message is not
**                      currently monitored and/or acted upon.  This is a blind
**                      request sending.
***********************************************************************************/
void CeMain::EvResponse( IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                         IN MXD_GNS::ISceUserConfig* pUserConfig,
                         IN mxt_opaque opqTransaction,
                         IN const MXD_GNS::CSipPacket& rResponse )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pComponent );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pUserConfig );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( opqTransaction );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rResponse );
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvChallenged
**
**  PURPOSE:    Callback from SCE-CORE ISceBasicExtensionMgr object when a
**              challenge for a request that it cannot handle is received.
**
**  PARAMETERS:
**              pComponent      - the network reference from SCE.
**              rChallenge      - the actual SIP packet (401/407) containing all
**                                the necessary information for the challenge.
**              opqChallenge    - opaque data to be passed back to the SCE.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvChallenged( IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                           IN const MXD_GNS::CSipPacket& rChallenge,
                           IN mxt_opaque opqChallenge )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pComponent );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rChallenge );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( opqChallenge );
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvSipContextCreated
**
**  PURPOSE:    Callback from SCE-CORE ISceBasicExtensionMgr object when a
**              SIP context is created.
**
**  PARAMETERS:
**              pComponent      - the component reference from SCE.
**              pCreatedContext - the SIP context reference.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvSipContextCreated( IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                                  IN MXD_GNS::ISipContext* pCreatedContext )
{
   /* Locate the component which was registered and which is receiving
   ** this callback.
   */
   BOS_UINT32 uGuid = locatePmRes( reinterpret_cast<void *> ( pComponent ) );
   BOS_UINT32 uUsrGuid = guCeInvalidGuid;
   CeResBase *pRes = NULL;
   BOS_BOOL bRes = locateRes( uGuid, pRes );
   BOS_BOOL bSkipNsBr = BOS_FALSE;
   BOS_BOOL bRetryAttach = BOS_FALSE; 

   if ( bRes )
   {
      switch ( pRes->resourceGender() )
      {
         case CeResBase::eCERESTYPE_USR:
         {
            CeResUser *pUser = reinterpret_cast<CeResUser *>
               ( pRes->resourceAddress() );

            /* If this is a new reg-event subscription then we need to clean up local knowledge
            ** of the previous subscription.
            */
            if ( pUser->resourceSubRegGuid() != guCeInvalidGuid && 
                 pUser->resourceSubRegGuid() == uGuid )
            {
               bRetryAttach = BOS_TRUE;
            }
            
            uUsrGuid = pRes->resourceGuid();
         }
         break;

         case CeResBase::eCERESTYPE_CALL:
         {
            uUsrGuid = pRes->resourceParent();
         }
         break;
         
         case CeResBase::eCERESTYPE_SUB:
         {
            /* Need to allow backup and recovery cleanup and retry in case of failure
            ** as subscriber component can replace the existing sip context.
            */
            bRetryAttach = BOS_TRUE;
            
            uUsrGuid = pRes->resourceParent();
         }
         break;

         case CeResBase::eCERESTYPE_NOT:
         {
            CeResNot *pResNot = NULL;

            uUsrGuid = pRes->resourceParent();

            /* If the created context is for Call Metrics publishing, we will
            ** need to bypass the nsbr processing as backup and recovery
            ** processing is not required.
            */
            pResNot = reinterpret_cast<CeResNot *> ( pRes->resourceAddress() );
            if ( pResNot->resourceType() == eCCTKPUBLISH_CALLMETRICS )
            {
               bSkipNsBr = BOS_TRUE;
            }
         }
         break;

         default:
         break;
      }
   }

   if ( uUsrGuid != guCeInvalidGuid )
   {
#if CCTKCFG_REMOTE_USER_CONTROL 
      /* If this is a call to the SIP remote user for an incoming call, set 
      ** the address spec of the call to the address spec of the incoming call */
      if ( pRes && pRes->resourceGender() == CeResBase::eCERESTYPE_CALL &&
           pRes->resourceFsm() == CeFsmBase::eCEFSMTYPE_PROXY_OUT )
      {
         CeResBase *pBaseUsr = NULL;
         /* Make sure this is a call for the SIP remote user */
         locateRes( uUsrGuid, pBaseUsr );
         if ( pBaseUsr && pBaseUsr->resourceGender() ==
              CeResBase::eCERESTYPE_USR )
         {
            CeResUser *pUser = reinterpret_cast<CeResUser *>
               ( pBaseUsr->resourceAddress() );

            if ( pUser->resourceIsRemUsr() )
            {
               /* Get the Name Spec, which as the display name and the URI
               **  from the incoming call.
               */
               MXD_GNS::CNameAddr proxyNameAddr;
               CeResCall *pCall = reinterpret_cast<CeResCall *>
                  ( pRes->resourceAddress() );
               
               CeResBase *pBaseProxyCall = NULL;
               locateRes( pCall->resourceProxyGuid(), pBaseProxyCall );
               if ( pBaseProxyCall && pBaseProxyCall->resourceGender() ==
                    CeResBase::eCERESTYPE_CALL )
               {
                  CeResCall *pProxyCall = reinterpret_cast<CeResCall *>
                     ( pBaseProxyCall->resourceAddress() );
                  pProxyCall->resourceGetCall ( proxyNameAddr );
               }
    
               /* Update the Local Address, the 'From' header */
               MXD_GNS::ISipUserAgentSvc* pUsSvc = NULL;      
               pCreatedContext->QueryIf( &pUsSvc );
               if ( pUsSvc )
               {
                  pUsSvc->SetLocalAddr(proxyNameAddr);
                  pUsSvc->ReleaseIfRef();
                  pUsSvc = NULL;
               }

               /* Update Preferred Identity, the 'P-Prefferd-Identity' header */
               MXD_GNS::ISipUaAssertedIdentitySvc *pAssertedIdSvc = NULL;                   
               pCreatedContext->QueryIf( &pAssertedIdSvc );
               if ( pAssertedIdSvc )
               {
                  MXD_GNS::CNameAddr *pPI = BRCM_NEW( MXD_GNS::CNameAddr )(
                     proxyNameAddr );
                  pAssertedIdSvc->SetPreferredIdentities(pPI, NULL);
                  pAssertedIdSvc->ReleaseIfRef();
                  pAssertedIdSvc = NULL;
               }
            }
         }
      }
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

      /* Set the opaque TOS for this newly created context.
      */
      BOS_UINT8 uTos = CeMain::getResTos( pRes->resourceGuid() );
      pCreatedContext->SetTosOpaque( reinterpret_cast<mxt_opaque> ( uTos ) );

      /* Set the opaque GUID for this context.
      */
      pCreatedContext->SetGuidOpaque( reinterpret_cast<mxt_opaque> ( uGuid ) );

      /* Check whether we need to associate some service with this context
      ** that has just been created.
      */
      if ( !bSkipNsBr )
      {
         const CFGUSR *pCfgUsr = CFG_USR( uUsrGuid );

         if ( pCfgUsr &&
              ( pCfgUsr->xtl.usr.net.brCfg.eMode == eCCTKBRSVCMODE_DLM ) )
         { 
            if ( !nsBrAttach( uUsrGuid,
                                 uGuid,
                                 reinterpret_cast<void *> ( pCreatedContext ) ) )
            {
               CCTK_TRACEI2( "CeMain::EvSipContextCreated - fail nsBrAttach (0x%lX) - %s",
                                uGuid, bRetryAttach ? "retry" : "no retry" );
                                
               /* The failure could be caused by a duplicated resource. Retry if allowed.
               */
               if( bRetryAttach )
               {
                  nsBrDetach( uGuid );
                  if ( !nsBrAttach( uUsrGuid,
                                    uGuid,
                                    reinterpret_cast<void *> ( pCreatedContext ) ) )
                  {
                        CCTK_TRACED1( "CeMain::EvSipContextCreated - fail nsBrAttach (0x%lX) - finite!",
                               uGuid );
                  }
               }
            }
         }
      }

      /* Check whether we need to reset the CSeq for this context, this has the
      ** effect of initiating the CSeq to 1 for each new transaction associated
      ** with this context.
      */
      if ( UTIL_BMP_IS_ENABLED_ALL(
               CFG_SYS->xtl.net.prot.gen.sigBehave.uTx,
               CCTK_SIGBEHVTX_RESETCSEQ ) )
      {
         MXD_GNS::ISipUserAgentSvc *pLocalUaSvc = NULL;
         
         pCreatedContext->QueryIf( &pLocalUaSvc );
         if ( pLocalUaSvc != NULL )
         {
            pLocalUaSvc->SetLocalSequenceNumber (
                              static_cast<uint32_t> ( guCeSequenceInit ));

            pLocalUaSvc->ReleaseIfRef();
            pLocalUaSvc = NULL;
         }
      }

      /* Check whether we need to add a challenge (Authentication) information to
      ** the component context.
      */
      if ( UTIL_BMP_IS_ENABLED_ALL(
               CFG_SYS->xtl.net.prot.gen.sigBehave.uTx,
               CCTK_SIGBEHVTX_INSCACHEAUTH ) )
      {
         
         CeResBase::CERESTYPE cachedType = pRes->resourceGender();

         locateRes( uUsrGuid, pRes );
         if ( pRes && pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
         {
            CeResUser *pUser = reinterpret_cast<CeResUser *>
               ( pRes->resourceAddress() );

            if ( pUser->resourceChallengeId() )
            {
               pComponent->AddChallengeInfo(
                  pUser->resourceChallengeId() );
               pComponent->SetChallengeType(
                  pUser->resourceChallengeId(),
                  static_cast<unsigned int> (
                        (cachedType == CeResBase::eCERESTYPE_USR) ?
                        MXD_GNS::uUNAUTHORIZED :
                        MXD_GNS::uPROXY_AUTHENTICATION_REQUIRED) );
            }
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvSubscriptionStatus
**
**  PURPOSE:    Callback from SCE-CORE ISceSubscriberMgr object when a
**              subscription status information is received.
**
**  PARAMETERS:
**              pSubscriber     - the subscriber reference from SCE.
**              eStatus         - the status.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvSubscriptionStatus( IN MXD_GNS::ISceSubscriber* pSubscriber,
                                   IN ESubscriptionStatus eStatus )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;

   /* Make sure this is for a known subscription and retrieve the corresponding
   ** resource.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx]->resourceSubEqual( pSubscriber ))
      {
         pRes = mpResSub[uIx];
         break;
      }
   }
   /* If we could not locate a valid resource for this authentication,
   ** terminate the authentication to prevent any mis-behavior.
   */
   if ( pRes == NULL )
   {
      pSubscriber->ReleaseIfRef();
      pSubscriber = NULL;
   }
   /* ...otherwise, map the event and send it to the FSM.
   */
   else
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eStatus ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvResourceStateUpdate
**
**  PURPOSE:    Callback from SCE-CORE ISceSubscriberMgr object when the
**              state of a subscribed object was changed (i.e. typically a
**              notification was received for this object).
**
**  PARAMETERS:
**              pSubscriber     - the subscriber reference from SCE.
**              pNotifyServerEventCtrl - the server event control to handle the
**                                       notification.
**              rPayload        - the payload received containing the new state.
**              bReceivedFinalNotify - TRUE if the final notification has been
**                                     received for the component, FALSE otherwise.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvResourceStateUpdate(
                        IN MXD_GNS::ISceSubscriber* pSubscriber,
                        IN MXD_GNS::ISipServerEventControl* pNotifyServerEventCtrl,
                        IN const MXD_GNS::CSipMessageBody& rPayload,
                        IN bool bReceivedFinalNotify )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( bReceivedFinalNotify );

   /* Make sure this is for a known subscription and retrieve the corresponding
   ** resource.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx]->resourceSubEqual( pSubscriber ))
      {
         pRes = mpResSub[uIx];
         break;
      }
   }
   /* If we could not locate a valid resource for this authentication,
   ** terminate the authentication to prevent any mis-behavior.
   */
   if ( pRes == NULL )
   {
      pSubscriber->ReleaseIfRef();
      pSubscriber = NULL;
   }
   /* ...otherwise, map the event and send it to the FSM.
   */
   else
   {
      CEEVT eEvt = eCEEVT_INV;

      if ( rPayload.GetBlobBody() )
      {
         CEMAINDATA data;
      
         memset ( static_cast<void *> (&data),
                  0,
                  sizeof( CEMAINDATA ) );

         data.uSize = rPayload.GetBlobBody()->GetSize();
         data.puData =
            utilDataDup( rPayload.GetBlobBody()->GetFirstIndexPtr(),
                         rPayload.GetBlobBody()->GetSize() );
         data.pAdditionalNotInfo = static_cast<void *> ( pNotifyServerEventCtrl );

         fsmMapEvent( pRes->resourceFsm(),
                      static_cast<BOS_UINT32> ( eCEEVT_SUB_NET_NOTIFY ),
                      eEvt );

         fsmProcessEvt( pRes->resourceGuid(),
                        eEvt,
                        static_cast<void *> (&data) );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvWatcherInfo
**
**  PURPOSE:    Callback from SCE-CORE ISceSubscriberMgr object when watcher
**              information has been updated.
**
**  PARAMETERS:
**              pSubscriber            - the subscriber reference from SCE.
**              rWatcherInfo           - the reference to the watcher information.
**              bFlushWatcherInfoTable - true if the input table contains full
**                                       watcher info.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvWatcherInfo(
                 IN MXD_GNS::ISceSubscriber* pSubscriber,
                 IN const MXD_GNS::ISceSubscriberMgr::SWatcherInfo& rWatcherInfo,
                 IN bool bFlushWatcherInfoTable )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pSubscriber );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rWatcherInfo );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( bFlushWatcherInfoTable );
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvWatcherInfoUpdateDone
**
**  PURPOSE:    Callback from SCE-CORE ISceSubscriberMgr object when no more items
**              to report the watcher information.
**
**  PARAMETERS: None.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvWatcherInfoUpdateDone()
{
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvResourceListUpdate
**
**  PURPOSE:    Callback from SCE-CORE ISceSubscriberMgr object when the
**              items of a resource list have their state updated.
**
**  PARAMETERS:
**              pszResourceListUri     - the resource list URI.
**              rvecpListDisplayName   - the display name associated with the list.
**              bFullState             - true if the list contains full state info.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvResourceListUpdate(
               IN const char* pszResourceListUri,
               IN MXD_GNS::CVector< MXD_GNS::CXmlElement* >& rvecpListDisplayName,
               IN bool bFullState)
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pszResourceListUri );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rvecpListDisplayName );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( bFullState );
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvResourceListUpdateDone
**
**  PURPOSE:    Callback from SCE-CORE ISceSubscriberMgr object when no more items
**              to report for the resource list.
**
**  PARAMETERS: None.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvResourceListUpdateDone()
{
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvResourceListItemUpdate
**
**  PURPOSE:    Callback from SCE-CORE ISceSubscriberMgr object when the state
**              of a resource found on a resource list was changed.
**
**  PARAMETERS:
**              pSubscriber     - the subscriber reference from SCE.
**              pResourceInfo   - the resource information.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvResourceListItemUpdate(
                  IN MXD_GNS::ISceSubscriber* pSubscriber,
                  IN const MXD_GNS::ISceSubscriberMgr::SResourceListResourceInfo*
                        pResourceInfo)
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pSubscriber );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pResourceInfo );
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvSubscriptionStatus
**
**  PURPOSE:    Callback from UASSP IUaSspMwiMgr object when a
**              subscription status information is received.
**
**  PARAMETERS:
**              pMwi     - the subscriber reference from UASSP.
**              eStatus  - the status.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvSubscriptionStatus(
                   IN MXD_GNS::IUaSspMwi* pMwi,
                   IN MXD_GNS::ISceSubscriberMgr::ESubscriptionStatus eStatus )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;

   /* Make sure this is for a known subscription and retrieve the corresponding
   ** resource.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx]->resourceMwiEqual( pMwi ))
      {
         pRes = mpResSub[uIx];
         break;
      }
   }
   /* If we could not locate a valid resource for this authentication,
   ** terminate the authentication to prevent any mis-behavior.
   */
   if ( pRes == NULL )
   {
      pMwi->ReleaseIfRef();
      pMwi = NULL;
   }
   /* ...otherwise, map the event and send it to the FSM.
   */
   else
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eStatus ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvMailboxStatus
**
**  PURPOSE:    Callback from UASSP IUaSspMwiMgr object when a
**              mailbox event information is received.
**
**  PARAMETERS:
**              pMwi      - the subscriber reference from UASSP.
**              rSummary  - the information summary.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvMailboxStatus( IN MXD_GNS::IUaSspMwi* pMwi,
                              IN const MXD_GNS::CMessageSummary& rSummary )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;

   /* Make sure this is for a known subscription and retrieve the corresponding
   ** resource.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx]->resourceMwiEqual( pMwi ))
      {
         pRes = mpResSub[uIx];
         break;
      }
   }
   /* If this is not a valid subscription, then this is an invalid callback.
   */
   if ( pRes == NULL )
   {
      pMwi->ReleaseIfRef();
      pMwi = NULL;
   }
   /* ...otherwise, map the event and send it to the FSM.
   */
   else
   {
      CCTKMWIINFO mwiInfo;
      BOS_UINT32 uIx;
      MXD_GNS::CMessageSummary::SSummaryLine sumLine;
      CEEVT eEvt = eCEEVT_INV;

      memset( static_cast<void *> (&mwiInfo),
              0,
              sizeof( CCTKMWIINFO ));

      /* Get the message summary data of interest.
      */
      mwiInfo.bMsgStatus =
               static_cast<BOS_BOOL> (rSummary.GetMsgStatus());
      for ( uIx = 0; uIx < rSummary.GetMsgSummaryLines().GetSize() ; uIx++ )
      {
         sumLine = rSummary.GetMsgSummaryLines().GetAt( uIx );
         if ( !sumLine.m_strMessageContextClass.CaseInsCmp(
               MXD_GNS::CMessageSummary::ms_szMWI_MSG_CONTEXT_CLASS_VOICE))
         {
            mwiInfo.voiceMsg.newMsg += sumLine.m_uNewMsgs;
            mwiInfo.voiceMsg.oldMsg += sumLine.m_uOldMsgs;
            mwiInfo.voiceMsg.newUrgMsg += sumLine.m_uNewUrgentMsgs;
            mwiInfo.voiceMsg.oldUrgMsg += sumLine.m_uOldUrgentMsgs;
         }
      }

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eCEEVT_SUB_NET_NOTIFY ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt,
                     static_cast<void *> (&mwiInfo) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::HandleRequest
**
**  PURPOSE:    Callback from SCE-CORE ISceRequestHandler object when a
**              request is received and the CCTK must decide how to handle it.
**
**  PARAMETERS:
**              rRequest      - the request received.
**              pUserConfig   - the user configuration associated with this
**                              request.
**
**  RETURNS:    resFE_NOT_IMPLEMENTED - if we do not handle this request.
**              resS_OK               - if we handled the request somehow.
**
**  NOTES:      This handler is invoked by the SCE component upon reception of
**              any request from the network and it gives the CCTK the chance to
**              intercept and process the request outside of the SCE framework.
**
**              Doing so implies however that the CCTK shall be fully responsible
**              for managing the request, including if applicable managing the
**              dialog associated with the request and managing the response to a
**              request.
**
**              For those reason, it is anticipated that such handler will only
**              be used for requests not managed by the SCE otherwise since it is
**              not the intention of the design to re-invent the wheel when SCE
**              already provides it.
**
**              For special handling of a request that cannot be processed by the
**              SCE even though a component exists for it, look at the following
**              API instead: 'CeMain::EvUnhandledRequest'
***********************************************************************************/
mxt_result CeMain::HandleRequest( IN const MXD_GNS::CSipPacket& rRequest,
                                  IN MXD_GNS::IEComUnknown* pUserConfig )
{
   mxt_result eRes = resFE_NOT_IMPLEMENTED;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pUserConfig );

   if ( rRequest.IsRequest() )
   {
      switch ( rRequest.GetRequestLine()->GetMethod() )
      {
         /* Those requests are the most 'natural' ones that the CCTK
         ** may want to process on its own.
         */
         case MXD_GNS::eSIP_METHOD_INFO:
         case MXD_GNS::eSIP_METHOD_OPTIONS:
         case MXD_GNS::eSIP_METHOD_PING:
         /* Those requests should be left with SCE to manage.
         **
         ** However, should an application decide to intercept those at this time,
         ** one should set the eRes variable to 'resS_OK' to indicate ownership of
         ** the request has been taken.
         **
         ** Furthermore, the CE framework should be invoked accordingly to handle
         ** the request, which includes either updating or creating proper FSM
         ** support for it.
         */
         case MXD_GNS::eSIP_METHOD_ACK:
         case MXD_GNS::eSIP_METHOD_BYE:
         case MXD_GNS::eSIP_METHOD_CANCEL:
         case MXD_GNS::eSIP_METHOD_INVITE:
         case MXD_GNS::eSIP_METHOD_MESSAGE:
         case MXD_GNS::eSIP_METHOD_NOTIFY:
         case MXD_GNS::eSIP_METHOD_PRACK:
         case MXD_GNS::eSIP_METHOD_PUBLISH:
         case MXD_GNS::eSIP_METHOD_REFER:
         case MXD_GNS::eSIP_METHOD_REGISTER:
         case MXD_GNS::eSIP_METHOD_SERVICE:
         case MXD_GNS::eSIP_METHOD_SUBSCRIBE:
         case MXD_GNS::eSIP_METHOD_UPDATE:
         default:
         break;
      }
   }

   return eRes;
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvNewNotifierObject
**
**  PURPOSE:    Callback from SCE-CORE ISceNewNotifierMgr object when a
**              new subscription has been received.
**
**  PARAMETERS:
**              pNotifier         - the newly created component.
**              pUserConfig       - the user configuration associated with this
**                                  component.  May be NULL.
**              rSubscribeRequest - the request that triggered this callback.
**
**  RETURNS:    resFE_NOT_IMPLEMENTED - if we do not handle this request.
**              resS_OK               - if we handled the request somehow.
**
**  NOTES:      If we accept handling the request at this time, we need to make
**              sure that all necessary resources to handle the request will be
**              in place.
***********************************************************************************/
void CeMain::EvNewNotifierObject( IN MXD_GNS::ISceNotifier* pNotifier,
                                  IN MXD_GNS::ISceUserConfig* pUserConfig,
                                  IN const MXD_GNS::CSipPacket& rSubscribeRequest )
{
   BOS_UINT32 uParentGuid = guCeInvalidGuid;
   CeResBase *pRes = NULL;
   BOS_UINT32 uIx;
   const CFGUSR *pCfgUsr;

   /* This could happen if the subscription is coming directly from a peer which
   ** does not know our registration contact matching.  This would mean that
   ** the call is not coming through a proxy with which we have registered.
   **
   ** We could reject this scenario, but in an effort to accept peer-to-peer
   ** scenario, we would attempt to match the SIP URI to a known configured user.
   */
   if ( pUserConfig == NULL )
   {
      const MXD_GNS::CSipUri *pUri =
                        rSubscribeRequest.GetRequestLine()->GetSipUri();
      if ( pUri )
      {
         for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
         {
            if ( (mpResUser[uIx] != NULL) && (pUserConfig == NULL) )
            {
               pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
               if ( (pCfgUsr != NULL) &&
                    !strcmp(
                       reinterpret_cast<const char *> (
                                    pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                       pUri->GetUser().GetString().CStr() ) )
               {
                  pUserConfig = pCfgUsr->in.pSceUsrCfg;
                  pNotifier->SetConfiguration( pUserConfig );
                  break;
               }
            }
         }
      }
   }

   /* Make sure the subscription is for a registered/configured user.  We do not
   ** accept subscriptions for unknown target user(s).
   */
   if ( pUserConfig != NULL )
   {
      /* Locate the user resource for this subscription and create a resource
      ** to handle it.
      */
      BOS_UINT32 uResGuid = allocateGuid();

      /* Loop through the known configured user to identify the one corresponding
      ** to this config.
      */
      for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
      {
         if ( (mpResUser[uIx] != NULL) && (uParentGuid == guCeInvalidGuid) )
         {
            pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
            if ( (pCfgUsr != NULL) && (pCfgUsr->in.pSceUsrCfg == pUserConfig) )
            {
               uParentGuid = mpResUser[uIx]->resourceGuid();
               break;
            }
         }
      }

      /* If the parent was located, continue with subscription creation and
      ** allocation.
      */
      if ( (uResGuid != guCeInvalidGuid) &&
           (uParentGuid != guCeInvalidGuid ) &&
           (allocateRes( CeResBase::eCERESTYPE_SUB,
                         uResGuid,
                         uParentGuid,
                         BOS_FALSE ) != guCeInvalidGuid) )
      {
         if ( cfgAddEvt( uResGuid, uParentGuid ) == eCFGSTATUS_SUCCESS )
         {
            /* Resource creation is successful, set it up accordingly such that
            ** FSM can take over.
            */
            locateRes( uResGuid, pRes );
            pRes->resourcePushFsm( CeFsmBase::eCEFSMTYPE_SUB );
            pRes->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_IDLE );
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_SUB )
            {
               CeResSub *pSub = reinterpret_cast<CeResSub *>
                  ( pRes->resourceAddress() );
               pSub->resourceCreate( uResGuid, eCCTKSUBEVT_MAX, pNotifier );
            }
         }
         else
         {
            locateRes(uResGuid, pRes);
            if ( pRes && pRes->resourceGender() == CeResBase::eCERESTYPE_SUB )
            {
               CeResSub *pSub = reinterpret_cast<CeResSub *>
                  ( pRes->resourceAddress() );
               pSub->resourceTerminate();
            }
            uParentGuid = guCeInvalidGuid;
         }
      }
      else
      {
         uParentGuid = guCeInvalidGuid;
      }
   }

   if ( uParentGuid == guCeInvalidGuid )
   {
      /* Cannot accept this new incoming subscription.  Terminate it.
      */
      pUserConfig = NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvNewSubscription
**
**  PURPOSE:    Callback from SCE ISceNotifierMgr when a new subscription has been
**              received and needs to be handled.
**
**  PARAMETERS:
**              pNotifier               - the notifier reference.
**              rFromAddr               - where it came from.
**              rToAddr                 - who this is for.
**              eEventType              - the subscription type.
**              uRequestedExpirationSec - the requested TTL for this subscription.
**              pBody                   - the body of the subscription.
**              pAdditionalParameter    - additional parameters.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvNewSubscription( IN MXD_GNS::ISceNotifier* pNotifier,
                                IN const MXD_GNS::CNameAddr& rFromAddr,
                                IN const MXD_GNS::CNameAddr& rToAddr,
                                IN MXD_GNS::ESipEventType eEventType,
                                IN uint32_t uRequestedExpirationSec,
                                IN const MXD_GNS::CSipMessageBody* pBody,
                                IN MXD_GNS::IEComUnknown* pAdditionalParameter )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rFromAddr );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rToAddr );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pBody );

   /* Make sure this is for a known subscription and retrieve the corresponding
   ** resource.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx]->resourceNotEqual( pNotifier ))
      {
         pRes = mpResSub[uIx];
         break;
      }
   }
   /* If we could not locate a valid resource for this subscription,
   ** terminate the subscription to prevent any mis-behavior.
   */
   if ( pRes == NULL )
   {
      pNotifier->ReleaseIfRef();
      pNotifier = NULL;
   }
   else
   {
      CEMAINSUB subInfo;
      CEEVT eEvt = eCEEVT_INV;

      subInfo.uTimeOut = static_cast<BOS_UINT32> (uRequestedExpirationSec);
      subInfo.uType = static_cast<BOS_UINT32> (eEventType);
      subInfo.pAdditionalSubInfo =
         (pAdditionalParameter ?
            static_cast<void *> (pAdditionalParameter) : NULL);

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eCEEVT_SUB_NET_REQ ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt,
                     static_cast<void *> (&subInfo) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvSubscriptionTerminated
**
**  PURPOSE:    Callback from SCE ISceNotifierMgr when a subscription was
**              terminated by peer.
**
**  PARAMETERS:
**              pNotifier    - the notifier reference.
**              eReason      - the reason for termination.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvSubscriptionTerminated(
                         IN MXD_GNS::ISceNotifier* pNotifier,
                         IN MXD_GNS::ISceNotifierMgr::ETerminationReason eReason )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( eReason );

   /* Make sure this is for a known subscription and retrieve the corresponding
   ** resource.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx]->resourceNotEqual( pNotifier ))
      {
         pRes = mpResSub[uIx];
         break;
      }
   }
   /* If we could not locate a valid resource for this authentication,
   ** terminate the authentication to prevent any mis-behavior.
   */
   if ( pRes == NULL )
   {
      pNotifier->ReleaseIfRef();
      pNotifier = NULL;
   }
   else
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eCEEVT_SUB_NET_REQEND ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt,
                     NULL );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvNotifierTerminated
**
**  PURPOSE:    Callback from SCE ISceNotifierMgr when a subscription has been
**              terminated and notifier can be released as well, there will be
**              no longer events for this subscription.
**
**  PARAMETERS:
**              pNotifier    - the notifier reference.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvNotifierTerminated( IN MXD_GNS::ISceNotifier* pNotifier )
{
   BOS_UINT32 uIx;
   CeResBase *pRes = NULL;

   /* Make sure this is for a known subscription and retrieve the corresponding
   ** resource.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_EVT ; uIx++ )
   {
      if ( mpResSub[uIx]->resourceNotEqual( pNotifier ))
      {
         pRes = mpResSub[uIx];
         break;
      }
   }
   /* If we could not locate a valid resource for this authentication,
   ** terminate the authentication to prevent any mis-behavior.
   */
   if ( pRes == NULL )
   {
      pNotifier->ReleaseIfRef();
      pNotifier = NULL;
   }
   else
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eCEEVT_SUB_NET_ENDED ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt,
                     NULL );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvRefer
**
**  PURPOSE:    Callback from SCE ISceReferrerMgr when a REFER (call transfer) has
**              been received.
**
**  PARAMETERS:
**              pReferrerComponent    - the referrer reference.
**              pReferSource          - the AOR of who initiated the transfer.
**              pTargetDialog         - the target dialog associated with this
**                                      REFER (may be NULL).
**              rReferTo              - the target to refer to.
**              pAdditionalParameters - additional parameters.
**
**  RETURNS:    None
**
**  NOTES:      If the target dialog is NULL, we are not going to accept this
**              action.  The target dialog should be valid and correspond to an
**              existing UASSP call (dialog).
***********************************************************************************/
void CeMain::EvRefer( IN MXD_GNS::ISceReferrer* pReferrerComponent,
                      IN const MXD_GNS::CNameAddr& pReferSource,
                      IN MXD_GNS::IEComUnknown* pTargetDialog,
                      IN const MXD_GNS::CNameAddr& rReferTo,
                      IN MXD_GNS::IEComUnknown* pAdditionalParameters )
{
   CeResBase *pRes = NULL;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pReferSource );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pAdditionalParameters );

   if ( locateReferRes( pReferrerComponent,
                        pRes ) )
   {
      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         CEEVT eEvt = eCEEVT_INV;

         CeResUser *pUser = reinterpret_cast<CeResUser *>
            ( pRes->resourceAddress() );

         pUser->resourceSetReferTo( rReferTo );
         pUser->resourceSetTgtDlg( pTargetDialog );

         fsmMapEvent( pRes->resourceFsm(),
                      static_cast<BOS_UINT32> ( eCEEVT_REG_NET_REFER ),
                      eEvt );

         fsmProcessEvt( pRes->resourceGuid(),
                        eEvt,
                        static_cast<void *> (pReferrerComponent) );
      }
   }
   /* If we could not locate a valid resource for this referrer,
   ** release the referrer reference to prevent any misbehavior.
   */
   else if ( pRes == NULL )
   {
      pReferrerComponent->ReleaseIfRef();
      pReferrerComponent = NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvReferProgress
**
**  PURPOSE:    Callback from SCE ISceReferrerMgr when the progress of a referrer
**              action is being reported.  This callback may be invoked several
**              times throughout the lifetime of the referrer, the status code is
**              the actual SIP status code.
**
**  PARAMETERS:
**              pReferrerComponent    - the referrer reference.
**              uStatusCode           - the status being reported.
**
**  RETURNS:    None
**
**  NOTES:      To be noted that at this time, the referrer ownership is most
**              likely with the call resource, which is where it would matter to
**              be processed.  If the referrer ownership is with the user
**              resource, we will simply ignore those events.
***********************************************************************************/
void CeMain::EvReferProgress( IN MXD_GNS::ISceReferrer* pReferrerComponent,
                              IN uint16_t uStatusCode )
{
   CeResBase *pRes = NULL;

   if ( locateReferRes( pReferrerComponent,
                        pRes ) )
   {
      CEEVT eEvt = eCEEVT_INV;
      MXD_GNS::IUaSspCallMgr::ETransferReport eReport;

      switch ( MXD_GNS::MxGetSipStatusClass( uStatusCode ) )
      {
         case MXD_GNS::eSIP_STATUS_CLASS_SUCCESS:
         {
            eReport = eTRANSFER_RESULT_SUCCESS;
         }
         break;

         case MXD_GNS::eSIP_STATUS_CLASS_SERVER_ERROR:
         case MXD_GNS::eSIP_STATUS_CLASS_GLOBAL_FAILURE:
         case MXD_GNS::eSIP_STATUS_CLASS_CLIENT_ERROR:
         {
            /* Treat authentication challenge as 'in-progress' rather than
            ** a failure.
            */
            if ( ( uStatusCode == MXD_GNS::uUNAUTHORIZED ) ||
                 ( uStatusCode == MXD_GNS::uPROXY_AUTHENTICATION_REQUIRED ) )
            {
               eReport = eTRANSFER_RESULT_IN_PROGRESS;
            }
            else
            {
               eReport = eTRANSFER_RESULT_FAILURE;
            }
         }
         break;

         case MXD_GNS::eSIP_STATUS_CLASS_UNKNOWN:
         {
            eReport = eTRANSFER_RESULT_PROGRESSING;
         }
         break;

         case MXD_GNS::eSIP_STATUS_CLASS_INFORMATIONAL:
         case MXD_GNS::eSIP_STATUS_CLASS_REDIRECTION:
         default:
         {
            eReport = eTRANSFER_RESULT_IN_PROGRESS;
         }
         break;
      }

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eReport ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt,
                     NULL );
   }
   /* If we could not locate a valid resource for this referrer,
   ** release the referrer reference to prevent any misbehavior.
   */
   else if ( pRes == NULL )
   {
      pReferrerComponent->ReleaseIfRef();
      pReferrerComponent = NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvReferStatus
**
**  PURPOSE:    Callback from SCE ISceReferrerMgr when the status to a referrer
**              attempt is being reported.  This is the status report to the
**              initial attempt to 'REFER'.  It is not the final status of the
**              transfer.  In particular, if the REFER was accepted, the actual
**              service status will come through EvReferProgress.
**
**  PARAMETERS:
**              pReferrerComponent    - the referrer reference.
**              eStatus               - the status being reported.
**
**  RETURNS:    None
**
**  NOTES:      To be noted that at this time, the referrer ownership is with
**              the call resource.
***********************************************************************************/
void CeMain::EvReferStatus( IN MXD_GNS::ISceReferrer* pReferrerComponent,
                            IN MXD_GNS::ISceReferrerMgr::EReferStatus eStatus )
{
   CeResBase *pRes = NULL;

   if ( locateReferRes( pReferrerComponent,
                        pRes ) )
   {
      CEEVT eEvt = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   ( ( eStatus == eSTATUS_ACCEPTED ) ?
                        static_cast<BOS_UINT32> ( eCEEVT_XFER_NET_ACCEPT ) :
                        static_cast<BOS_UINT32> ( eCEEVT_XFER_NET_FAIL ) ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt,
                     NULL );
   }
   /* If we could not locate a valid resource for this referrer,
   ** release the referrer reference to prevent any misbehavior.
   */
   else if ( pRes == NULL )
   {
      pReferrerComponent->ReleaseIfRef();
      pReferrerComponent = NULL;
   }
   
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvTerminated
**
**  PURPOSE:    Callback from SCE ISceReferrerMgr when a referrer has been
**              terminated.
**
**  PARAMETERS:
**              pReferrerComponent    - the referrer reference.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvTerminated( IN MXD_GNS::ISceReferrer* pReferrerComponent )
{
   CeResBase *pRes = NULL;
   
   /* If the referrer resource is located then pass on the event
   ** to the resource so that it can clean up as needed. Otherwise
   ** do nothing as the resource has already released the referrer.
   */
   if ( locateReferRes( pReferrerComponent,
                        pRes ) )
   {
      CEEVT eEvt = eCEEVT_INV;
      CEEVT eEvtIn =
         ((pRes->resourceGender() == CeResBase::eCERESTYPE_CALL) ?
            eCEEVT_XFER_NET_REFENDED : eCEEVT_REG_NET_REFENDED );

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eEvtIn ),
                   eEvt );

      fsmProcessEvt( pRes->resourceGuid(),
                     eEvt,
                     static_cast<void *> (pReferrerComponent) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvNewReferrerObject
**
**  PURPOSE:    Callback from SCE ISceNewReferrerMgr when a new referrer has been
**              created in order to handle a new REFER (transfer service).
**
**  PARAMETERS:
**              pReferrer    - the referrer reference.
**              pUserConfig  - the user configuration associated with this
**                             referrer.
**              rPacket      - the packet which triggered this event.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvNewReferrerObject( IN MXD_GNS::ISceReferrer* pReferrer,
                                  IN MXD_GNS::ISceUserConfig* pUserConfig,
                                  IN const MXD_GNS::CSipPacket& rPacket )
{
   BOS_UINT32 uParentGuid = guCeInvalidGuid;
   BOS_UINT32 uIx;
   const CFGUSR *pCfgUsr;

   if ( pUserConfig == NULL )
   {
      const MXD_GNS::CSipUri *pUri = rPacket.GetRequestLine()->GetSipUri();
      if ( pUri )
      {
         MXD_GNS::CSipUri *pTmpUri = BRCM_NEW( MXD_GNS::CSipUri ) ( *pUri );
         MXD_GNS::CNameAddr addr;

         addr.Reset();
         addr.SetUri( pTmpUri );

         for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
         {
            if ( (mpResUser[uIx] != NULL) && (pUserConfig == NULL) )
            {
               pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
               if ( (pCfgUsr != NULL) &&
                      /* Check if this is a known user.
                      */
                    ( !strcmp(
                       reinterpret_cast<const char *> (
                                       pCfgUsr->xtl.usr.net.sipName.pcUsrName),
                         pUri->GetUser().GetString().CStr() ) ||
                      /* Check if this is an assumed identity for a known
                      ** user.
                      */
                      mpResUser[uIx]->resourceIsAssumedId( addr ) ) )
               {
                  pUserConfig = pCfgUsr->in.pSceUsrCfg;
                  pReferrer->SetConfiguration( pUserConfig );
                  break;
               }
            }
         }
      }
   }

   /* Make sure the referrer is for a registered/configured user.  We do not
   ** accept referrer for unknown target user(s).
   */
   if ( pUserConfig != NULL )
   {
      /* Locate the user resource for this referrer and associate it with the
      ** user resource to handle it.  Note that the user resource will subsequently
      ** pass on the ownership of this referrer to the correct call resource.
      */
      for ( uIx = 0 ; uIx < guCeMainMaxUser ; uIx++ )
      {
         if ( (mpResUser[uIx] != NULL) && (uParentGuid == guCeInvalidGuid) )
         {
            pCfgUsr = CFG_USR( mpResUser[uIx]->resourceGuid() );
            if ( (pCfgUsr != NULL) && (pCfgUsr->in.pSceUsrCfg == pUserConfig) )
            {
               uParentGuid = mpResUser[uIx]->resourceGuid();
               break;
            }
         }
      }

      /* If the parent was located, continue with call creation and allocation.
      */
      if ( (uParentGuid != guCeInvalidGuid ) &&
           allocateReferrer( uParentGuid,
                             pReferrer ) )
      {
         pReferrer->SetManager( gpCeMain );

         /* Set the manager and packet obeserver for this referrer.
         */
         MXD_GNS::ISceBasicExtensionControl *pSceBasicExtCtl = NULL;
         mxt_result res = pReferrer->QueryIf( &pSceBasicExtCtl );
         if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
         {
            pSceBasicExtCtl->SetBasicExtensionManager( gpCeMain );
            pSceBasicExtCtl->SetSipPacketObserver(
               reinterpret_cast<MXD_GNS::ISceSipPacketObserver *>(
                                                         pmGetPktObs() ) );
            pSceBasicExtCtl->ReleaseIfRef();
            pSceBasicExtCtl = NULL;
         }
      }
   }

   if ( uParentGuid == guCeInvalidGuid )
   {
      /* Cannot accept this new referrer.
      */
      pUserConfig = NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::ceMseEvent
**
**  PURPOSE:    Callback from MSE layer when a media associated event is being
**              reported.
**
**  PARAMETERS:
**              uMseId    - the MSE reference.
**              eEvt      - the event.
**
**  RETURNS:    None
**
**  NOTES:      The MSE handle passed in this callback corresponds to the GUID
**              of the CE call resource it is associated with since there is a one
**              to one association between a CE-Call and a MSE.
***********************************************************************************/
void CeMain::ceMseEvent( BOS_UINT32 uMseId, CCTKEVT eEvt )
{
   CeResBase *pRes = NULL;

   if ( locateRes( uMseId, pRes ))
   {
      CEEVT eMapped = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eEvt ),
                   eMapped );

      fsmProcessEvt( pRes->resourceGuid(),
                     eMapped );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::EvXferNotificationTimeout
**
**  PURPOSE:    Callback from framework (timer) when a transfer notification timer
**              has expired.
**
**  PARAMETERS:
**              uId       - the timer reference.
**              pData     - the data passed when timer was started (will contain
**                          the GUID of the call which started the timer).
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::EvXferNotificationTimeout( BOS_UINT32 uId, void *pData )
{
   CeResBase *pRes = NULL;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pData );

   if ( locateTmrRes( uId, pRes ))
   {
      CEEVT eMapped = eCEEVT_INV;

      fsmMapEvent( pRes->resourceFsm(),
                   static_cast<BOS_UINT32> ( eCEEVT_XFER_USR_NTTO ),
                   eMapped );

      fsmProcessEvt( pRes->resourceGuid(),
                     eMapped );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::CeResetTimeout
**
**  PURPOSE:    Callback from framework (timer) when a reset timeout timer
**              has expired.
**
**  PARAMETERS:
**              uId       - the timer reference.
**              pData     - the data passed when timer was started.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::CeResetTimeout( BOS_UINT32 uId, void *pData )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pData );

   if ( uId == muTmrRst )
   {
      muTmrRst = guCeInvalidGuid;
   
      /* Take down any of the active stack component.
      **
      ** As a component gets terminated, it will signal the
      ** termination to CE which will in turn clean up its 
      ** associated resources for the component.
      */
      shutDown ();
      
      /* Done with reset now that the shutdown is completed.
      */
      mbReset = BOS_FALSE;


      /* Notify the framework that it can continue proceeding with
      ** the reset action that was suspended until now.
      */
      tkmResetCont();
   }
   else
   {
      CCTK_TRACEI1( "CeMain::CeResetTimeout - Invalid timer (0x%lX)!", uId );
   }
}


/***********************************************************************************
**  FUNCTION:   CeMain::CeBackupRecoveryTimeout
**
**  PURPOSE:    Callback from framework (timer) when a backup and recovery timeout
**              timer has expired.
**
**  PARAMETERS:
**              uId       - the timer reference.
**              pData     - the data passed when timer was started.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void CeMain::CeBackupRecoveryTimeout( BOS_UINT32 uId, void *pData )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pData );
   CeResBase *pRes = NULL;

   if ( locateBrRes( uId, pRes ) )
   {
      CeResUser *pResUser = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         const CFGUSR *pCfgUsr = NULL;
         mxt_result res;
         
         pResUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );
         pCfgUsr = CFG_USR( pResUser->resourceGuid());
         
         CCTK_TRACEI1( "CeBackupRecoveryTimeout - (0x%X)", pResUser->resourceGuid() );

         /* Clean up timer knowledge.
         */
         pResUser->resourceSetTmrBkupReco( guCeInvalidGuid );
         
         /* Clean up reg cycle success status.
         */
         pResUser->resourceSuccessfulRegCycle( BOS_FALSE );
         
         /* Cleanup backoff delay knowledge.
         */
         pResUser->resourceApplyBkoffDelay( BOS_FALSE );
         
         /* If all targets have failed clean up the backup and recovery data set.
         */
         if ( nsBrStatus( pResUser->resourceGuid()) == eNSBRSTA_FAILED )
         {
            if ( nsBrRestart( pResUser->resourceGuid() ) == eNSBRSTA_IDLE )
            {
               /* In external list mode we need to update the outbound proxy
               ** as the backup and recovery service will not be
               ** responsible for choosing the destination
               */
               if ( pCfgUsr &&
                   ( pCfgUsr->xtl.usr.net.brCfg.eMode == eCCTKBRSVCMODE_EXTLIST ) )
               {
                  pResUser->resourceSelectNewObProxy();
                  pResUser->resourceNetHdl()->UpdateRegistrar();
               }
            }
         }

         /* Reset the UASSP reg retry-time as this was previoulsy updated
         ** to postpone the failed registration retry as per the calculated 
         ** backoff delay.
         */
         if ( pCfgUsr )
         {
            MXD_GNS::ISceUserConfig *pUsrCfg = pCfgUsr->in.pSceUsrCfg;
            if ( pUsrCfg != NULL )
            {
               MXD_GNS::IUaSspRegistrationConfig *pRegCfg = NULL;
               res = pUsrCfg->QueryIf( &pRegCfg );
               if ( MX_RIS_S( res ) && ( pRegCfg != NULL ) )
               {
                  pRegCfg->SetRegistrationRetryDelay(
                              pCfgUsr->xtl.reg.gen.tmr.uRegRetryTmr );

                  pRegCfg->ReleaseIfRef();
                  pRegCfg = NULL;
               }
            }
         }
         
         /* Issue the register.
         */         
         ceNotify ( pResUser->resourceGuid(),
                          eCEEVT_REG_USR_REG,
                          NULL );
      }
      else
      {
         CCTK_TRACEI2(
               "CeBackupRecoveryTimeout - Invalid user 0x%lX for tmr 0x%lX!",
               pRes->resourceGuid(), uId );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeKeepAliveRecoveryTimeout
**
**  PURPOSE:    Callback from framework (timer) when a keep alive recovery timeout
**              timer has expired.
**
**  PARAMETERS:
**              uId       - the timer reference.
**              pData     - the data passed when timer was started.
**
**  RETURNS:    None
**
**  NOTES:
***********************************************************************************/
void CeMain::CeKeepAliveRecoveryTimeout( BOS_UINT32 uId, void *pData )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pData );
   CeResBase *pRes = NULL;

   if ( locateKaRes( uId, pRes ) )
   {
      CeResUser *pResUser = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         pResUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );

         /* Clean up timer knowledge.
         */
         pResUser->resourceSetTmrKeepAliveReco( guCeInvalidGuid );

         ceNotify ( pResUser->resourceGuid(),
                    eCEEVT_REG_USR_KA_RECOVERY,
                    NULL );
      }
      else
      {
         CCTK_TRACEI2(
               "CeKeepAliveRecoveryTimeout - Invalid user 0x%lX for tmr 0x%lX!",
               pRes->resourceGuid(), uId );
      }
   }
}

/***********************************************************************************
**  FUNCTION:   CeBackToPrimaryRecoveryTimeout
**
**  PURPOSE:    Callback from framework (timer) when a back to primary recovery 
**              timeout timer has expired.
**
**  PARAMETERS:
**              uId       - the timer reference.
**              pData     - the data passed when timer was started.
**
**  RETURNS:    None
**
**  NOTES:
***********************************************************************************/
void CeMain::CeBackToPrimaryRecoveryTimeout( BOS_UINT32 uId, void *pData )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pData );
   CeResBase *pRes = NULL;

   if ( locateFailBkRes( uId, pRes ) )
   {
      CeResUser *pResUser = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_USR )
      {
         pResUser = reinterpret_cast<CeResUser *> ( pRes->resourceAddress() );

         /* Clean up timer knowledge.
         */
         pResUser->resourceSetTmrFailbkPrimReco( guCeInvalidGuid );
         
         /* Dispatch the temporary user to try registering with the primary.
         */
         const CFGUSR *pCfgUsr = CFG_USR( pResUser->resourceGuid() );
         if ( ( pCfgUsr != NULL ) &&
              pCfgUsr->xtl.usr.net.brCfg.eBackToPrimMode != eCCTKBRBACKTOPRIMMODE_DISABLED )
         {
            CeDispatchRestrictedUser( pResUser->resourceGuid(), BOS_TRUE );
         }
      }
      else
      {
         CCTK_TRACEI2(
               "CeBackToPrimaryRecoveryTimeout - Invalid user 0x%lX for tmr 0x%lX!",
               pRes->resourceGuid(), uId );
      }
   }
}

/***********************************************************************************
**  FUNCTION:   CeMain::setPublishAddr
**
**  PURPOSE:    Set the IP address to which PUBLISH messages are being transmitted
**              to
**
**  PARAMETERS:
**              uResGuid  - the resource that the PUBLISH is associated with
**              pPubAddr  - the IP address to which PUBLISH messages are being
**                          transmitted to
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void CeMain::setPublishAddr( BOS_UINT32 uResGuid, MX_NS CSocketAddr *pPubAddr )
{
   BOS_UINT32     uIx;
   BOS_IP_ADDRESS pubIpAddr;

   if ( uResGuid != guCeInvalidGuid )
   {
      /* Make sure we match a matching resource before informing the upper layer
      ** application */
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_NOT ; uIx++ )
      {
         if ( mpResNot[uIx] &&
              ( mpResNot[uIx]->resourceGuid() == uResGuid ) )
         {
            bosIpAddrCreateFromStr ( pPubAddr->GetAddress().CStr(),
                                     &pubIpAddr );

            tkmEvent( mpResNot[uIx]->resourceParent(), 
                      guCeInvalidGuid,
                      eCCTKEVT_PUBLISH_TARGET, 
                      &pubIpAddr );
            break;
         }
      }
   }
}

/***********************************************************************************
**  FUNCTION:   CeMain::setPrivInfo
**
**  PURPOSE:    Set the privacy info for the resource.
**
**  PARAMETERS:
**              uResGuid  - the resource that privacy info is associated to
**              pPrivCfg - the IP address to which PUBLISH messages are being
**                          transmitted to
**
**  RETURNS:    BOS_TRUE if able to pass on the privacy info to the call resource.
**              BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeMain::setPrivInfo( BOS_UINT32 uResGuid, CCTKPRIVCFG *pPrivCfg )
{
   CeResBase *pRes = NULL;

   if ( locateRes( uResGuid, pRes ) )
   {
      CeResCall *pResCall = NULL;

      if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
      {
         pResCall = reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );

         pResCall->resourceSetPrivCfgInfo( pPrivCfg );
         
         return BOS_TRUE;
      }
      else
      {
         CCTK_TRACEI1( "setPrivInfo - Invalid call 0x%lX!", uResGuid );
      }
   }

   return BOS_FALSE;
}


