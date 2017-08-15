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

#define CCTKINIT_EC_GEN_HDR_CFG
const CCTKGENHDRCFG gCctkDftEcGenHdrCfg =
{
   1,                                  /* uNum */
   {
      eCCTKHDRTYPE_PRIVATE,            /* hdr[0].hdr.eType */
                                       /* hdr[0].hdr.pcName */
      reinterpret_cast< BOS_UINT8* >( "P-Test-Header" ),
                                       /* hdr[0].hdr.pcValue */
      reinterpret_cast< BOS_UINT8* >( "test value 123" ),
      CCTK_METHOD_GRP_CALL,            /* hdr[0].uMethod */
      eCCTKHDRMAP_ALL,                 /* hdr[0].eMap */
      eCCTKHDRMODE_APPEND,             /* hdr[0].eMode */
   },
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
   3600,                               /* uRegExpTmr */
   600000,                             /* uSubExpTmr */
   2000,                               /* uThreshold */
   10,                                 /* uRegRetryTmr */
   10,                                 /* uSubRetryTmr */
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
   BOS_TRUE,                           /* bUseBandwidth */
   BOS_TRUE,                           /* bDisableRTCP */
   BOS_FALSE,                          /* bSdpAnswerIn180 */
   BOS_FALSE,                          /* bFirstSdpIsFinal */
   BOS_TRUE,                           /* bLbSupport */
   {                                   /* uRtpCode (based on CCTKCODEC) */
      0,                               /* eCCTKCODEC_PCMU */
      8,                               /* eCCTKCODEC_PCMA */
      4,                               /* eCCTKCODEC_G7231 */
      107,                             /* eCCTKCODEC_G726_16 */
      108,                             /* eCCTKCODEC_G726_24 */
      109,                             /* eCCTKCODEC_G726_32 */
      110,                             /* eCCTKCODEC_G726_40 */
      15,                              /* eCCTKCODEC_G728 */
      18,                              /* eCCTKCODEC_G729 */
      98,                              /* eCCTKCODEC_G729E */
      99,                              /* eCCTKCODEC_ILBC */
      96,                              /* eCCTKCODEC_BV16 */
      97,                              /* eCCTKCODEC_BV32 */
      9,                               /* eCCTKCODEC_G722 */
      111,                             /* eCCTKCODEC_G7221 */
#if CCTKCFG_RED
      102,
#endif
      100,                             /* eCCTKCODEC_AMRWB */
      112,                             /* eCCTKCODEC_LPCMWB */
#if CCTKCFG_VIDEO
      34,                              /* eCCTKCODEC_H263 */
      105,                             /* eCCTKCODEC_H264 */
      106,                             /* eCCTKCODEC_MPEG4 */
#endif
   },
   101,                                /* uTelEvtRtpCode */
   104,                                /* uEncapRtpCode */
   103,                                /* uRtpLbRtpCode */
   13,                                 /* uCnRtpCode */
   BOS_TRUE                            /* bSdpNoStaticRtpmap */
};

#define CCTKINIT_EC_CALL_HOLD
const CCTKHOLDCFG gCctkDftEcCallHold =
{
   BOS_TRUE,
   BOS_TRUE,
   BOS_FALSE,
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
#ifdef CCTKCFG_USE_CABLE_SIMULATOR
   BOS_TRUE,                                 /* bKeepAlive */
#else
   BOS_FALSE,                                /* bKeepAlive */
#endif
};

#define CCTKINIT_EC_REG_EVT_CFG
const CCTKREGEVTCFG gCctkDftEcRegEvtCfg =
{
   BOS_FALSE,                                 /* bRegEvtSupport */
   BOS_FALSE,                                 /* bMapRejToDeact */
};


#define CCTKINIT_EC_USR_BR_CFG
const CCTKUSRBRCFG gCctkDftEcUsrBrCfg =
{
   BOS_FALSE,                         /* bEnabled */
   BOS_FALSE,                         /* bFailover */
   BOS_FALSE,                         /* bBackToPrimary */
   BOS_FALSE,                         /* bEnableInbdReg */
   eCCTKBRBKOFFMODE_EXP_ALLFAILED,    /* eBackoffMode  */
   60,                                /* uBackoffMinTmr */
   300,                               /* uBackoffMaxTmr */
   0,                                 /* uBackToPrimMinTmr */
   0,                                 /* uBackToPrimMaxTmr */
};


#define CCTKINIT_EC_PAU_CFG
const CCTKPAUCFG gCctkDftEcPauCfg =
{
   BOS_FALSE,                          /* bNoPauUnreg */
   BOS_TRUE,                           /* bPauNoValid */
   BOS_FALSE,                          /* bPauUpdCtc */
   BOS_TRUE,                           /* bPauUsrSwap */
};


#define CCTKINIT_EC_INTEROP
const BOS_UINT32 guCctkDftEcInterop = CCTK_INTEROP_MSGSUMCRLF | CCTK_INTEROP_REVALRTNFO;


#define CCTKINIT_EC_SIG_BEHAVE
const CCTKSIGBEHAVE gCctkDftEcSigBehave =
{
   CCTK_SIGBEHVTX_INSCACHEAUTH,                /* uTx */
   CCTK_SIGBEHVRX_NONE,                /* uRx */
};

#define UA_STR "PIERRE-TEST-UA"
#define SRV_STR "PIERRE-TEST-SRV"

#define CCTKINIT_EC_UA_ID_HDR
const CCTKUAIDHDR gCctkDftEcUaIdHdr =
{
  (BOS_UINT8 *)UA_STR,                   /* pcUaHdr */
  eCCTKUAHDRFMT_APPONLY,              /* eUaHdrFmt */
  (BOS_UINT8 *)SRV_STR,                  /* pcSvrHdr */
  NULL,                               /* pcSipInstId */
};
