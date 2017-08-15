/**********************************************************************************
** Copyright © 2009 - 2010 Broadcom Corporation
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
**      This module implements mapping function of any kind necessary for usage
**      of features within the TCAM.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <tcamInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define    TCAM__INVALID           "INVALID"

/* ---- Private Variables ------------------------------------------------------- */

/* This maps the CMGR signals (which are targetted to the endpoint interface) into
** the corresponding event that is passed to the outside application.  As such,
** some of the events from CMGR are ignored or mapped to the proper external event
** as expected by the outside world application.
*/
TCAMMAPID tcamMapCmgrEptSig2OutEvt[] =
{
   { eEPTSIG_BUSY,                 eTCAM_DISCONNECT },
   { eEPTSIG_CONF,                 eTCAM_INVALID    },
   { eEPTSIG_DIAL,                 eTCAM_INVALID    },
   { eEPTSIG_OHWARN,               eTCAM_DISCONNECT },
   { eEPTSIG_RINGING,              eTCAM_NEWCALL    },
   { eEPTSIG_RING0,                eTCAM_NEWCALL    },
   { eEPTSIG_RING1,                eTCAM_NEWCALL    },
   { eEPTSIG_RING2,                eTCAM_NEWCALL    },
   { eEPTSIG_RING3,                eTCAM_NEWCALL    },
   { eEPTSIG_RING4,                eTCAM_NEWCALL    },
   { eEPTSIG_RING5,                eTCAM_NEWCALL    },
   { eEPTSIG_RING6,                eTCAM_NEWCALL    },
   { eEPTSIG_RING7,                eTCAM_NEWCALL    },
   { eEPTSIG_CALLID_RINGING,       eTCAM_NEWCALL    },
   { eEPTSIG_CALLID_RING0,         eTCAM_NEWCALL    },
   { eEPTSIG_CALLID_RING1,         eTCAM_NEWCALL    },
   { eEPTSIG_CALLID_RING2,         eTCAM_NEWCALL    },
   { eEPTSIG_CALLID_RING3,         eTCAM_NEWCALL    },
   { eEPTSIG_CALLID_RING4,         eTCAM_NEWCALL    },
   { eEPTSIG_CALLID_RING5,         eTCAM_NEWCALL    },
   { eEPTSIG_CALLID_RING6,         eTCAM_NEWCALL    },
   { eEPTSIG_CALLID_RING7,         eTCAM_NEWCALL    },
   { eEPTSIG_REORDER,              eTCAM_DISCONNECT },
   { eEPTSIG_SPLASH,               eTCAM_INVALID    },
   { eEPTSIG_RINGBACK,             eTCAM_ALERT      },
   { eEPTSIG_STUTTER,              eTCAM_INVALID    },
   { eEPTSIG_CONF_STUTTER,         eTCAM_INVALID    },
   { eEPTSIG_CALLW1,               eTCAM_NEWCALL    },
   { eEPTSIG_CALLW2,               eTCAM_NEWCALL    },
   { eEPTSIG_CALLW3,               eTCAM_NEWCALL    },
   { eEPTSIG_CALLW4,               eTCAM_NEWCALL    },
   { eEPTSIG_SAS_CAS1,             eTCAM_NEWCALL    },
   { eEPTSIG_SAS_CAS2,             eTCAM_NEWCALL    },
   { eEPTSIG_SAS_CAS3,             eTCAM_NEWCALL    },
   { eEPTSIG_SAS_CAS4,             eTCAM_NEWCALL    },

   { TCAM_INVALID_HANDLE,          eTCAM_INVALID    },
};


/* This maps the CMGR events (which are information about either registration
** or call progress) into the corresponding event that is passed to the outside
** application.  As such, some of the events from CMGR are ignored or mapped
** to the proper external event as expected by the outside world application.
*/
TCAMMAPID tcamMapCmgrEvt2OutEvt[] =
{
   { eCMGREVT_OPER_OK,             eTCAM_SVCRDY     },
   { eCMGREVT_OPER_NOK,            eTCAM_SVCNRDY    },
   { eCMGREVT_MWI_ON,              eTCAM_MWION      }, 
   { eCMGREVT_MWI_OFF,             eTCAM_MWIOFF     },
   { eCMGREVT_CALL_CONNECT,        eTCAM_ACTIVE     },
   { eCMGREVT_CALL_ENDED,          eTCAM_DISCONNECT },
   { eCMGREVT_CALL_XFER,           eTCAM_CALL_XFER  },
   { eCMGREVT_SVC_APPLY,           eTCAM_SVCOK      },
   { eCMGREVT_SVC_DENY,            eTCAM_SVCFAIL    },
   { eCMGREVT_CONF_ESTAB,          eTCAM_CONFEST    },

   { eCMGREVT_MESSAGE,             eTCAM_MESSAGE    },
   { eCMGREVT_MSG_HDL,             eTCAM_MSG_HDL    },
   { eCMGREVT_MSG_RSP,             eTCAM_MSG_RSP    },

   { TCAM_INVALID_HANDLE,          eTCAM_INVALID    },
};


/* Maps the CMGR service to an outside of the world event.
*/
TCAMMAPID tcamMapCmgrSvc2OutEvt[] =
{
   { eCMGRSVC_LOCAL_HOLD,          eTCAM_HOLD          },
   { eCMGRSVC_LOCAL_UNHOLD,        eTCAM_UNHOLD        },
   { eCMGRSVC_REMOTE_HOLD,         eTCAM_REMOTE_HOLD   },
   { eCMGRSVC_REMOTE_UNHOLD,       eTCAM_REMOTE_UNHOLD },
   { eCMGRSVC_CONF,                eTCAM_CONF          },
   { eCMGRSVC_BXFER,               eTCAM_BXFER         },
   { eCMGRSVC_CXFER,               eTCAM_CXFER         },

   { TCAM_INVALID_HANDLE,          eTCAM_INVALID       },
};


/* Mapping the outside protocol action names.
*/
TCAMMAPSTR tcamMapOutEvtStr[] =
{
   /* Incoming command from the outside workd into TCAM.
   */
   { eTCAM_CFG,               (BOS_UINT8 *)"CONFIG" },
   { eTCAM_USRCFG,            (BOS_UINT8 *)"USRCFG" },
   { eTCAM_USRDEL,            (BOS_UINT8 *)"USRDEL" },
   { eTCAM_DIAL,              (BOS_UINT8 *)"DIAL" },
   { eTCAM_HANGUP,            (BOS_UINT8 *)"HANGUP" },
   { eTCAM_ANSWER,            (BOS_UINT8 *)"ANSWER" },
   { eTCAM_HANDLE,            (BOS_UINT8 *)"SETHANDLE" },
   { eTCAM_RESTART,           (BOS_UINT8 *)"RESTART" },

   /* Incoming or outgoing command/event.
   */
   { eTCAM_ALERT,             (BOS_UINT8 *)"ALERT" },
   { eTCAM_MESSAGE,           (BOS_UINT8 *)"MESSAGE" },
   { eTCAM_HOLD,              (BOS_UINT8 *)"HOLD" },
   { eTCAM_UNHOLD,            (BOS_UINT8 *)"UNHOLD" },
   { eTCAM_CONF,              (BOS_UINT8 *)"CONF" },
   { eTCAM_CONFEND,           (BOS_UINT8 *)"CONFEND" },
   { eTCAM_BXFER,             (BOS_UINT8 *)"BXFER" },
   { eTCAM_CXFER,             (BOS_UINT8 *)"CXFER" },
   { eTCAM_XFEREND,           (BOS_UINT8 *)"XFEREND" },
   { eTCAM_MWION,             (BOS_UINT8 *)"MWI" },
   { eTCAM_MWIOFF,            (BOS_UINT8 *)"MWI" },
   { eTCAM_DTMF,              (BOS_UINT8 *)"DTMF" },

   /* Outgoing event from TCAM to the outside world.
   */
   { eTCAM_SVCRDY,            (BOS_UINT8 *)"SVC_RDY" },
   { eTCAM_SVCNRDY,           (BOS_UINT8 *)"SVC_NRDY" },
   { eTCAM_NEWCALL,           (BOS_UINT8 *)"NEWCALL" },
   { eTCAM_ACTIVE,            (BOS_UINT8 *)"ACTIVE" },
   { eTCAM_DISCONNECT,        (BOS_UINT8 *)"DISCONNECT" },
   { eTCAM_CALL_XFER,         (BOS_UINT8 *)"CALLXFER" },
   { eTCAM_REMOTE_HOLD,       (BOS_UINT8 *)"REMOTEHOLD" },
   { eTCAM_REMOTE_UNHOLD,     (BOS_UINT8 *)"REMOTEUNHOLD" },

   /* For internal use only (TCAM <-> ISA) */
   { eTCAM_DIRINC,            (BOS_UINT8 *)"INT-DIRINC" },
   { eTCAM_DIROUT,            (BOS_UINT8 *)"INT-DIROUT" },
   { eTCAM_ANSWERED,          (BOS_UINT8 *)"INT-ANS" },
   { eTCAM_DISC,              (BOS_UINT8 *)"INT-DISC" },
   { eTCAM_DTMFPLAY,          (BOS_UINT8 *)"INT-DTMF" },
   { eTCAM_CONFEST,           (BOS_UINT8 *)"INT-CONF" },

   { eTCAM_INVALID,           (BOS_UINT8 *)"INVALID" },
   { TCAM_INVALID_HANDLE,     (BOS_UINT8 *)"INVALID" },
};


/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   tcamMapGetId
**
** PURPOSE:    Maps an identified resource to another one in an ID based map.
**
** PARAMETERS: pMap - the map to lookup.
**             uId - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if no valid
**             mapping can be obtained.
***********************************************************************************/
BOS_UINT32 tcamMapGetId ( TCAMMAPID *pMap, BOS_UINT32 uId )
{
   BOS_UINT32 uRet = TCAM_INVALID_HANDLE;

   if ( pMap )
   {
      while ( (pMap->uId != TCAM_INVALID_HANDLE) &&
              (pMap->uId != uId) )
      {
         pMap++;
      }
      uRet = pMap->uMap;
   }

   return uRet;
}


/***********************************************************************************
** FUNCTION:   tcamMapGetStr
**
** PURPOSE:    Maps an identified resource to its human readable expression.
**
** PARAMETERS: pMap - the map to lookup.
**             uId - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, NULL pointer if no valid
**             mapping can be obtained.
***********************************************************************************/
BOS_UINT8 *tcamMapGetStr ( TCAMMAPSTR *pMap, BOS_UINT32 uId )
{
   BOS_UINT8 *pRet = (BOS_UINT8 *)TCAM__INVALID;

   if ( pMap )
   {
      while ( (pMap->uId != TCAM_INVALID_HANDLE) &&
              (pMap->uId != uId) )
      {
         pMap++;
      }
      pRet = pMap->pMap;
   }

   return pRet;
}


/***********************************************************************************
** FUNCTION:   tcamMapGetIdFromStr
**
** PURPOSE:    Maps a human readable resource to its internal machine equivalent.
**
** PARAMETERS: pMap - the map to lookup.
**             puStr - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if the
**             mapping can not be obtained.
***********************************************************************************/
BOS_UINT32 tcamMapGetIdFromStr ( TCAMMAPSTR *pMap, BOS_UINT8 *puStr )
{
   BOS_UINT32 uRet = TCAM_INVALID_HANDLE;

   if ( pMap && puStr )
   {
      while ( (pMap->uId != TCAM_INVALID_HANDLE) &&
              strCmpNoCase( (const char *)pMap->pMap, (const char *)puStr ) )
      {
         pMap++;
      }
      uRet = pMap->uId;
   }

   return uRet;
}
