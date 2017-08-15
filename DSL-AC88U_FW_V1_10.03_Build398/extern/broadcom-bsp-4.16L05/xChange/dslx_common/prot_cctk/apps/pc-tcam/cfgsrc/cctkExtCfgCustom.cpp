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
**      This file implements the CCTK custom extended configuration.
**
**      This module is being included as part of the cctkExtCfg.cpp module and
**      therefore is not required to be explicitely listed as part of the build
**      process.
**
***********************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif
/* This handler is defined in implemented in the call client, this is where we
** receive and handle the logs coming from the CCTK and SIP stack modules.
*/
extern void cctkTraceHandler( BOS_UINT16 uLevel,
                              BOS_UINT16 uGroup,
                              const BOS_UINT8 *pcTraceStr );
#ifdef __cplusplus
}
#endif


/* Override the default configuration for those elements of interest.
*/
#define CCTKINIT_EC_TRACE
const CCTKTRACE gCctkDftEcTrace =
{
   &cctkTraceHandler,
   CCTK_TRACELVL_ALL,
   CCTK_TRACEGRP_CCTK,
};

#define CCTKINIT_EC_PKT_LOG
const CCTKPKTLOG gCctkDftEcPktLog =
{
   {
#if CCTKCFG_IPV6
      {
         BOS_IPADDRESS_TYPE_V4,        /* ipPort.addr.ipType */
         {
            127,                       /* ipPort.addr.u.ipv4Address.s4Addr[0] */
            0,                         /* ipPort.addr.u.ipv4Address.s4Addr[1] */
            0,                         /* ipPort.addr.u.ipv4Address.s4Addr[2] */
            1,                         /* ipPort.addr.u.ipv4Address.s4Addr[3] */
         },
      },
#else
      {
         127,                          /* ipPort.addr.s4Addr[0] */
         0,                            /* ipPort.addr.s4Addr[1] */
         0,                            /* ipPort.addr.s4Addr[2] */
         1,                            /* ipPort.addr.s4Addr[3] */
      },
#endif
      55555,                           /* ipPort.uPort */
   },
   eCCTKPKTLOGMODE_SIP,                /* eMode */
};


#define CCTKINIT_EC_CALL_NAI
const CCTKNAICFG gCctkDftEcCallNai =
{
   {
      eCCTKNAICLTMODE_NET,             /* cli.eMode */
      BOS_FALSE,                       /* cli.bSec */
      NULL,                            /* cli.pPreferredId */
   },
   {
      eCCTKNAISVRMODE_ANY,             /* svr.eMode */
   }
};


#define CCTKINIT_EC_CALL_PRIV
const CCTKPRIVCFG gCctkDftEcCallPriv =
{
   eCCTKPRIVLVL_NONE,                  /* eLevel */
   BOS_FALSE,                          /* bCritical */
   BOS_FALSE,                          /* bRetry */
   BOS_FALSE,                          /* bRejectAnon */
};


#define CCTKINIT_EC_REG_TMR
const CCTKREGTMR gCctkDftEcRegTmr =
{
   600000,                             /* uRegExpTmr */
   600000,                             /* uSubExpTmr */
   60,                                 /* uThreshold */
   10,                                 /* uRegRetryTmr */
};


#define CCTKINIT_EC_PRACK
const CCTKPRACK geCctkDftEcPrack = eCCTKPRACK_SUPPORTED;

#define CCTKINIT_EC_CONNECTEDID
const CCTKCONNECTEDID geCctkDftEcConnectedId = eCCTKCONNECTEDID_SUPPORTED;

#define CCTKINIT_EC_MED_NEG_CFG
const CCTKMEDNEGCFG gCctkDftEcMedNegCfg =
{
   BOS_FALSE,                          /* bUseUpdate */
   BOS_FALSE,                          /* bUseMptime */
   BOS_TRUE,                           /* bSymmetricPtime */
   BOS_FALSE,                          /* bUseBandwidth */
   BOS_TRUE,                           /* bDisableRTCP */
   BOS_FALSE,                          /* bSdpAnswerIn180 */
   BOS_FALSE,                          /* bFirstSdpIsFinal */
   BOS_FALSE,                          /* bLbSupport */
   {                                   /* uRtpCode (based on CCTKCODEC) */
      0,                               /* CCTKCODEC_PCMU */
      8,                               /* CCTKCODEC_PCMA */
      98,                              /* CCTKCODEC_G729E */
      99,                              /* CCTKCODEC_ILBC */
      96,                              /* CCTKCODEC_BV16 */
      97,                              /* CCTKCODEC_BV32 */
      100,                             /* CCTKCODEC_AMRWB */
#if CCTKCFG_VIDEO
      104,                             /* eCCTKCODEC_H263 */
      105,                             /* eCCTKCODEC_H264 */
      106,                             /* eCCTKCODEC_MPEG4 */
#endif
   },
   101,                                /* uTelEvtRtpCode */
   104,                                /* uEncapRtpCode */
   103,                                /* uRtpLbRtpCode */
   13                                  /* uCnRtpCode */
};

#define CCTKINIT_EC_CALL_HOLD
const CCTKHOLDCFG gCctkDftEcCallHold =
{
   BOS_TRUE,
   BOS_TRUE,
   BOS_FALSE,
   BOS_FALSE,
};

#define CCTKINIT_EC_TRANSPORT
const CCTKSIGTRANS gCctkDftEcTransport =
{
#if CCTKCFG_TLS
   CCTK_TRANSPORT_UDP | CCTK_TRANSPORT_TCP | CCTK_TRANSPORT_TLS,
#else
   CCTK_TRANSPORT_UDP | CCTK_TRANSPORT_TCP,
#endif
                                             /* uSigTrans */
   CCTK_TRANSPORT_TCP_FALLBACK_MTU,          /* uTcpFallbackThresh */
   BOS_FALSE,                                /* bKeepAlive */
};

#define CCTKINIT_EC_REG_EVT_CFG
const CCTKREGEVTCFG gCctkDftEcRegEvtCfg =
{
   BOS_FALSE,                                 /* bRegEvtSupport */
};

#define CCTKINIT_EC_USR_BR_CFG
const CCTKUSRBRCFG gCctkDftEcUsrBrCfg =
{
   BOS_TRUE,                          /* bEnabled */
   BOS_TRUE,                          /* bFailover */
   BOS_FALSE,                         /* bBackToPrimary */
   BOS_FALSE,                         /* bEnableInbdReg */
   eCCTKBRBKOFFMODE_EXP_ALLFAILED,    /* eBackoffMode  */
   60,                                /* uBackoffMinTmr */
   300,                               /* uBackoffMaxTmr */
   0,                                 /* uBackToPrimMinTmr */
   0,                                 /* uBackToPrimMaxTmr */
};

#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
#define CCTKINIT_EC_IMS_CFG
const CCTKIMSCFG gCctkDftEcImsCfg =
{
   BOS_FALSE,
   BOS_TRUE,
   BOS_TRUE,
   BOS_FALSE,
   BOS_FALSE,
};
#endif
