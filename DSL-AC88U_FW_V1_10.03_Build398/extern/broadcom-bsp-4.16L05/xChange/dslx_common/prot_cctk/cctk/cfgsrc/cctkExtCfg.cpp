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
**      This file implements the CCTK extended configuration.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <cctkExtCfg.h>
#include <stdlib.h>

/* This includes the custom overrides for the specific application configuration.
*/
#include <cctkExtCfgCustom.cpp>


/* ---- Public Variables -------------------------------------------------------- */
/* Default supported subscription events */
#ifndef CCTKINIT_EC_SUB_EVT
const BOS_UINT32 guCctkDftEcSubEvt = CCTK_SUBEVT_NONE;
#endif

/* Default generic header configuration */
#ifndef CCTKINIT_EC_GEN_HDR_CFG
const CCTKGENHDRCFG gCctkDftEcGenHdrCfg =
{
   0,                                  /* uNum */
                                       /* hdr[CCTKCFG_MAX_GENHDRLST] */
};
#endif

/* Default user local port configuration */
#ifndef CCTKINIT_EC_USR_PORT_CFG
const CCTKUSRPORTCFG gCctkDftEcUsrPortCfg =
{
   {
      0,                               /* tosCfg.uDefault */
      0,                               /* tosCfg.uEmgCall */
   }
};
#endif

/* Default user emergency call configuration */
#ifndef CCTKINIT_EC_USR_EMG_CFG
const CCTKUSREMGCFG gCctkDftEcUsrEmgCfg =
{
   BOS_FALSE,                          /* bEndCallOnInc */
};
#endif

/* Default user backup and recovery configuration */
#ifndef CCTKINIT_EC_USR_BR_CFG
const CCTKUSRBRCFG gCctkDftEcUsrBrCfg =
{
   eCCTKBRSVCMODE_DISABLED,           /* eMode */
   BOS_FALSE,                         /* bFailover */
   BOS_FALSE,                         /* bEnableInbdReg */
   eCCTKBRBACKTOPRIMMODE_DISABLED,    /* eBackToPrimMode */
   eCCTKBRBKOFFMODE_ANYFAIL,          /* eBackoffMode (cert val) */
   BOS_TRUE,                          /* bOutboundBackoff */
   30,                                /* uBackoffMinTmr */
   1800,                              /* uBackoffMaxTmr */
   0,                                 /* uBackToPrimMinTmr */
   0,                                 /* uBackToPrimMaxTmr */
   {
      0,                              /* uNum */
                                      /* ipport[CCTKCFG_MAX_ADDRLST] */
   }
};
#endif

/* Default user backup and recovery configuration */
#ifndef CCTKINIT_EC_SVC_ROUTE
const CCTKSVCROUTE gCctkDftEcSvcRoute =
{
#if 0
   {
      {
         {
            NULL,                      /* route[0].addr.hostPort.pcAddr */
            0,                         /* route[0].addr.hostPort.uPort */
         },
         BOS_FALSE,                    /* route[0].addr.bSec */
      },
      BOS_FALSE,                       /* route[0].bLr */
   },
                                       /* route[CCTKCFG_MAX_OBPROXY] */
#endif
};
#endif

/* Default user contact configuration */
#ifndef CCTKINIT_EC_USR_CONTACT
const CCTKCONTACT gCctkDftEcUsrContact =
{
   {
      NULL,                            /* hdrOverride.pcAddr */
      NULL,                            /* hdrOverride.pcHdrParm */
   },
   NULL,                               /* pcUsrParm */
};
#endif

/* Default dialog event configuration */
#ifndef CCTKINIT_EC_DIAG_EVT_CFG
const CCTKDIAGEVTCFG gCctkDftEcDiagEvtCfg =
{
   NULL,                               /* puSubscriber */
   0,                                  /* uSuberMax */
   0                                   /* uSubeeMax */
};
#endif

/* Default user-agent profile event configuration */
#ifndef CCTKINIT_EC_UAP_EVT_CFG
const CCTKUAPEVTCFG gCctkDftEcUapEvtCfg =
{
   0,                                  /* uSubExpTmr */
   BOS_TRUE,                           /* bUnsolNtfy */
   0,                                  /* uNum */
                                       /* pcProfile[CCTKCFG_MAX_PROFCFGLST] */
};
#endif

/* Default white-list */
#ifndef CCTKINIT_EC_WHT_LST
const CCTKWHTLSTCFG gCctkDftEcWhtLst =
{
   {
      0,                               /* lst.uNum */
                                       /* lst.addr[CCTKCFG_MAX_ADDRLST] */
   }
                                       /* bOnlyFromLst */
};
#endif

/* Default black-list */
#ifndef CCTKINIT_EC_BLK_LST
const CCTKIPADDRLST gCctkDftEcBlkLst =
{
   0,                                  /* uNum */
                                       /* addr[CCTKCFG_MAX_ADDRLST] */
};
#endif

/* Default registration timer configuration */
#ifndef CCTKINIT_EC_REG_TMR
const CCTKREGTMR gCctkDftEcRegTmr =
{
   3600,                               /* uRegExpTmr */
   0,                                  /* uSubExpTmr */
   10,                                 /* uThreshold */
   10,                                 /* uRegRetryTmr */
   10,                                 /* uSubRetryTmr */
};
#endif

/* Default registration event configuration */
#ifndef CCTKINIT_EC_REG_EVT_CFG
const CCTKREGEVTCFG gCctkDftEcRegEvtCfg =
{
   BOS_FALSE,                          /* bRegEvtSupport */
   BOS_FALSE,                          /* bMapRejToDeact */
};
#endif

/* Default MWI configuration */
#ifndef CCTKINIT_EC_MWI_CFG
const CCTKMWICFG gCctkDftEcMwiCfg =
{
   {
      eCCTKMWIEVT_NOTIFY_ALWAYS,       /* mode.eEvtPolicy */
   },
};
#endif

/* Default MWI timer configuration */
#ifndef CCTKINIT_EC_MWI_TMR
const CCTKMWITMR gCctkDftEcMwiTmr =
{
   0,                                  /* uExpTmr */
   10,                                 /* uRetryTmr */
};
#endif

#ifndef CCTKINIT_EC_EMG_CALL_CFG
const CCTKEMGCALLCFG gCctkDftEcEmgCallCfg =
{
   BOS_FALSE,                          /* bEmergency */
   BOS_FALSE,                          /* bAddLocInfo */
};
#endif

#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
/* Default telephone call configuration */
#  ifndef CCTKINIT_EC_TEL_CALL
const CCTKTELCALL gCctkDftEcTelCall =
{
   BOS_FALSE,                          /* bNoEnum */
   BOS_FALSE,                          /* bDenyInCall */
};
#  endif
#endif

/* Default PRACK support configuration */
#ifndef CCTKINIT_EC_PRACK
const CCTKPRACK geCctkDftEcPrack = eCCTKPRACK_SUPPORTED;
#endif

/* Default Conneected ID support configuration */
#ifndef CCTKINIT_EC_CONNECTEDID
const CCTKCONNECTEDID geCctkDftEcConnectedId = eCCTKCONNECTEDID_DISABLED;
#endif

/* Default session timer configuration */
#ifndef CCTKINIT_EC_SES_TMR
const CCTKSESTMR gCctkDftEcSesTmr =
{
   1800,                               /* uTmr */
   0,                                  /* uMinSe */
   eCCTKSESTMRMODE_UPDATE,             /* eRefresh */
};
#endif

/* Default privacy configuration for call */
#ifndef CCTKINIT_EC_CALL_PRIV
const CCTKPRIVCFG gCctkDftEcCallPriv =
{
   eCCTKPRIVLVL_NONE,                  /* eLevel */
   BOS_FALSE,                          /* bCritical */
   BOS_FALSE,                          /* bRetry */
   BOS_FALSE,                          /* bRejectAnon */
};
#endif

/* Default network asserted identity configuration for call */
#ifndef CCTKINIT_EC_CALL_NAI
const CCTKNAICFG gCctkDftEcCallNai =
{
   {
      eCCTKNAICLTMODE_NET,             /* cli.eMode */
      BOS_FALSE,                       /* cli.bSec */
      NULL,                            /* cli.pPreferredId */
   },
   {
      eCCTKNAISVRMODE_IGNORED,         /* svr.eMode */
   }
};
#endif

/* Default call hold configuration */
#ifndef CCTKINIT_EC_CALL_HOLD
const CCTKHOLDCFG gCctkDftEcCallHold =
{
   BOS_FALSE,                          /* bNoQuadZeros */
   BOS_FALSE,                          /* bUseInactive */
   BOS_FALSE,                          /* bUseUpdate */
   BOS_FALSE,                          /* bLocalHold */
   BOS_FALSE,                          /* bBypassHoldRspChk */
};
#endif

/* Default call transfer configuration */
#ifndef CCTKINIT_EC_CALL_XFER
const CCTKXFERCFG gCctkDftEcCallXfer =
{
   {
      0,                               /* tfererCfg.uNotifyWaitTime */
      BOS_FALSE,                       /* tfererCfg.bEndOnNotify */
      BOS_FALSE,                       /* tfererCfg.bUseOodRefer */
      BOS_TRUE,                        /* tfererCfg.bOodRefFallBack */
      BOS_FALSE                        /* tfererCfg.bCleanOnFail */
   },
   CCTKXFEREE_ACCEPT,                  /* eTferee */
   CCTKXFERTGET_ACCEPT,                /* eTarget */
};
#endif

/* Default media negotiation configuration */
#ifndef CCTKINIT_EC_MED_NEG_CFG
const CCTKMEDNEGCFG gCctkDftEcMedNegCfg =
{
   BOS_FALSE,                          /* bUseUpdate */
   BOS_FALSE,                          /* bUseMptime */
   BOS_FALSE,                          /* bSymmetricPtime */
   BOS_FALSE,                          /* bUseBandwidth */
   BOS_FALSE,                          /* bDisableRTCP */
   BOS_FALSE,                          /* bSdpAnswerIn180 */
   BOS_FALSE,                          /* bFirstSdpIsFinal */
   BOS_FALSE,                          /* bLbSupport */
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
      118,                             /* eCCTKCODEC_GSMAMR */
      119,                             /* eCCTKCODEC_GSMEFR */

      97,                              /* eCCTKCODEC_BV32 */
      9,                               /* G.722 - 64 kbps (static payload type) */
      113,                             /* G.722 - 48 kbps */
      114,                             /* G.722 - 56 kbps */
      115,                             /* G.722 - 64 kbps */
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
   116,                                /* uTelEvtWbRtpCode */
   104,                                /* uEncapRtpCode */
   103,                                /* uRtpLbRtpCode */
   13,                                 /* uCnRtpCode */
   117,                                /* uCnWbRtpCode */
   BOS_FALSE                           /* bSdpNoStaticRtpmap */
};
#endif

/* Default timer configuration for event subscription */
#ifndef CCTKINIT_EC_EVT_TMR
const CCTKEVTTMR gCctkDftEcEvtTmr =
{
   10,                                 /* uRetryTmr */
   BOS_FALSE,                          /* bMap500To408 */
};
#endif

/* Default privacy configuration for event subscription */
#ifndef CCTKINIT_EC_EVT_PRIV
const CCTKPRIVCFG gCctkDftEcEvtPriv =
{
   eCCTKPRIVLVL_NONE,                  /* eLevel */
   BOS_FALSE,                          /* bCritical */
   BOS_FALSE,                          /* bRetry */
   BOS_FALSE,                          /* bRejectAnon */
};
#endif

/* Default network asserted identity configuration for event subscription */
#ifndef CCTKINIT_EC_EVT_NAI
const CCTKNAICFG gCctkDftEcEvtNai =
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
#endif

/* Default local port configuration */
#ifndef CCTKINIT_EC_LOC_PORT
const CCTKLOCPORT gCctkDftEcLocPort =
{
   0,                                  /* uSipPort */
   0,                                  /* uSipsPort */
   BOS_FALSE,                          /* bNoSymUdp */
};
#endif

/* Default transport configuration */
#ifndef CCTKINIT_EC_TRANSPORT
const CCTKSIGTRANS gCctkDftEcTransport =
{
   CCTK_TRANSPORT_UDP | CCTK_TRANSPORT_TCP,  /* uSigTrans */
   CCTK_TRANSPORT_TCP_FALLBACK_MTU,          /* uTcpFallbackThresh */
   BOS_FALSE,                                /* bKeepAlive */
};
#endif

#if CCTKCFG_TLS
/* Default TLS configuration */
#  ifndef CCTKINIT_EC_TLS_CFG
const CCTKTLSCFG gCctkDftEcTlsCfg =
{
   NULL,                               /* ppcCertChain */
   NULL,                               /* ppcTrustedCerts */
   NULL,                               /* pcPrivKey */
   NULL,                               /* pcCipher */
   BOS_FALSE,                          /* bClientAuth */
   BOS_FALSE,                          /* bServerAuth */
   {
      NULL,                            /* preGenDhPrime.puPrime */
      0,                               /* preGenDhPrime.uLen */
   }
};
#  endif
#endif

/* Default connection activity configuration */
#ifndef CCTKINIT_EC_CNX_ACTIVITY
const CCTKCNXCFG gCctkDftEcCnxActivity =
{
   CCTKCFG_DEFAULT_CNX_CFG,         /* uMinCnx */
   CCTKCFG_DEFAULT_CNX_CFG,         /* uMaxCnx */
   CCTKCFG_DEFAULT_CNX_CFG,         /* uMaxIdleTime */
};
#endif

/* Default user agent identity header configuration */
#ifndef CCTKINIT_EC_UA_ID_HDR
const CCTKUAIDHDR gCctkDftEcUaIdHdr =
{
   NULL,                               /* pcUaHdr */
   eCCTKUAHDRFMT_FULL,                 /* eUaHdrFmt */
   NULL,                               /* pcSvrHdr */
   NULL,                               /* pcSipInstId */
};
#endif

/* Default routing-related header configuration */
#ifndef CCTKINIT_EC_ROUTING_HDR
const CCTKROUTINGHDR gCctkDftEcRoutingHdr =
{
   0,                                  /* uMaxFwds */
};
#endif

/* Default DNS query policy */
#ifndef CCTKINIT_EC_DNS_MODE
const CCTKDNSMODE geCctkDftEcDnsMode = eCCTKDNSMODE_ALL;
#endif

/* Default SIP signaling behavial configuration */
#ifndef CCTKINIT_EC_SIG_BEHAVE
const CCTKSIGBEHAVE gCctkDftEcSigBehave =
{
   CCTK_SIGBEHVTX_NONE,                /* uTx */
   CCTK_SIGBEHVRX_NONE,                /* uRx */
};
#endif

/* Default SIP inter-operability configuration */
#ifndef CCTKINIT_EC_INTEROP
const BOS_UINT32 guCctkDftEcInterop = CCTK_INTEROP_NONE;
#endif

#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
/* Default IMS-specific configuration */
#  ifndef CCTKINIT_EC_IMS_CFG
const CCTKIMSCFG gCctkDftEcImsCfg =
{
   BOS_FALSE,                          /* bNoAuthHdrReg */
   BOS_TRUE,                           /* bClidInfoFrom */
   BOS_FALSE,                          /* bClidInfoPaiSip */
   BOS_FALSE,                          /* bCnamePaiSip */
   BOS_FALSE,                          /* bIgnoreInvalidPai */
   BOS_FALSE                           /* bNoClidInfoPai */
};
#  endif
#endif

/* Default GRUU configuration */
#ifndef CCTKINIT_EC_GRUU_CFG
const CCTKGRUUCFG gCctkDftEcGruuCfg =
{
#if CCTKCFG_GRUU_SUPPORT
   eCCTKGRUUMODE_REG,                  /* enable if GRUU is supported */
#else
   eCCTKGRUUMODE_DISABLED,             /* disabled by default*/
#endif
};
#endif

/* Default transaction timer configuration */
#ifndef CCTKINIT_EC_TRANS_TMR
const CCTKTRANSTMRCFG gCctkDftEcTransTmr =
{
   0,                                  /* uT1 */
   0,                                  /* uT2 */
   0,                                  /* uTimeout */
   0,                                  /* uT4 */
   0,                                  /* uB */
   0,                                  /* uD */
   0,                                  /* uF */
   0,                                  /* uH */
   0,                                  /* uJ */
};
#endif

/* Default packet logger configuration */
#ifndef CCTKINIT_EC_PKT_LOG
const CCTKPKTLOG gCctkDftEcPktLog =
{
   {
#if CCTKCFG_IPV6
      {
         BOS_IPADDRESS_TYPE_V4,        /* ipPort.addr.ipType */
         {
            {
               {
                  0,                   /* ipPort.addr.u.ipv4Address.u.u8[0] */
                  0,                   /* ipPort.addr.u.ipv4Address.u.u8[1] */
                  0,                   /* ipPort.addr.u.ipv4Address.u.u8[2] */
                  0,                   /* ipPort.addr.u.ipv4Address.u.u8[3] */
               },
            },
         },
      },
#else
      {
         {
            {
               0,                      /* ipPort.addr.u.u8[0] */
               0,                      /* ipPort.addr.u.u8[1] */
               0,                      /* ipPort.addr.u.u8[2] */
               0,                      /* ipPort.addr.u.u8[3] */
            },
         },
      },
#endif
      0,                               /* ipPort.uPort */
   },
   eCCTKPKTLOGMODE_SIP,                /* eMode */
};
#endif

#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
/* Default tracing configuration */
#  ifndef CCTKINIT_EC_TRACE
const CCTKTRACE gCctkDftEcTrace =
{
   NULL,                               /* pHandler */
   CCTK_TRACELVL_DEFAULT,              /* uLevel */
   CCTK_TRACEGRP_DEFAULT,              /* uGroup */
};
#  endif
#endif

/* Default general NAT configuration */
#ifndef CCTKINIT_EC_NAT_GEN_CFG
const CCTKNATGENCFG gCctkDftEcNatGenCfg =
{
   BOS_FALSE,                          /* bNoRport */
};
#endif

#if CCTKCFG_MEMORY_MONITORING
/* Default memory protection configuration */
#  ifndef CCTKINIT_EC_MEM_PROTECT
const CCTKMEMPROTCFG gCctkDftEcMemProtect =
{
   0,                                  /* uThreshold */
   eCCTKMEMPROT_NOTIFY,                /* ePolicy */
};
#  endif
#endif

/* Default P-Associcated-URI configuration */
#ifndef CCTKINIT_EC_PAU_CFG
const CCTKPAUCFG gCctkDftEcPauCfg =
{
   BOS_FALSE,                          /* bNoPauUnreg */
   BOS_FALSE,                          /* bPauNoValid */
   BOS_FALSE,                          /* bPauUpdCtc */
   BOS_FALSE,                          /* bPauUsrSwap */
};
#endif

/* Default P-Access-Network-Info configuration */
#ifndef CCTKINIT_EC_PANI_CFG
const CCTKPANICFG gCctkDftEcPaniCfg =
{
   BOS_FALSE,                          /* bIncludeNetworkInfo */
   NULL                                /* pcNetworkInfo */
};
#endif

/* -- Default function for RG NAT control --
** implement your own version fitting your application RG NAT control functions.
** and override the default funciton.
*/
#ifndef CCTKINIT_EC_RG_NAT_CTL_CFG
const CCTKRGNATCTL gCctkDftEcRgNatCtl = 
{
   NULL,   /* pNatIsLanAddrFunc */
   NULL,   /* pNatSesAddFunc */
   NULL,   /* pNatSesDelFunc */
};
#endif

#ifndef CCTKINIT_EC_AUTH_CFG
const CCTKAUTHCFG gCctkDftEcAuthCfg = 
{
   10,                                 /* uloopMax */
};
#endif

#if CCTKCFG_REMOTE_USER_CONTROL
/* Default SIP remote user system configuration */
#  ifndef CCTKINIT_EC_REM_USR_SYS_CFG
const CCTKREMUSRSYS gCctkDftEcRemUsrSysCfg =
{
   BOS_TRUE,                               /* bRemUsrSupport */
   eCCTKREMUSRADMMODE_ALL,                 /* eAdmMode */
   0,                                      /* uDftAssIMPUIndexRef */
   NULL,                                   /* pcDftAssIMPUUserId */
   0,                                      /* uDftIMPIIndexRef - no authorization*/
   NULL,                                   /* pcDftIMPIUserId */
   NULL,                                   /* pcDftIMPIUserPassword */
};
#  endif

#  ifndef CCTKINIT_EC_REM_USR_CFG
const CCTKREMUSRCFG gCctkDftEcRemUsrCfg =
{
   0,                                      /* uIMPIIndexRef */
   NULL,                                   /* pcIMPIUserId */
   NULL,                                   /* pcIMPIPassword */
};
#  endif

#endif

/* -- Default function for ICE control --
** implement your own version ICE connection manager control functions.
** and override the default funciton.
*/
#if CCTKCFG_ICE_LITE_SUPPORT
#ifndef CCTKINIT_EC_ICE_CTL_CFG
#error "Application must implement its ICE connection manager."
#error "cctkIce callback functions must be defined."
const CCTKICECTLCFG gCctkDftEcIceCtlCfg = 
{
   NULL,      /* pIceOpenCnxFunc */
   NULL,      /* pIceCloseCnxFunc */
   NULL,      /* pIceStunSendFunc */
   NULL,      /* pIceLogFunc */
   NULL,      /* pIceCrcFunc */
   NULL,      /* pIceHmacFunc */
   BOS_FALSE, /* bIPv6PrefMedia */
};
#endif

/* Default ICE Address list */
#ifndef CCTKINIT_EC_ICE_LST
const CCTKIPADDRLST gCctkDftEcIceLst =
{
   0,                                  /* uNum */
                                       /* addr[CCTKCFG_MAX_ADDRLST] */
};
#endif 

#endif /* CCTKCFG_ICE_LITE_SUPPORT */


CCTKEXTCFGMAP gCctkDftExtUsrCfg[] =
{
   /* -------------- USER CONFIG -------------- */
   /* -- User.Local -- */
   { eCCTKPARM_EC_SUBEVT,              &guCctkDftEcSubEvt },
   { eCCTKPARM_EC_GENHDRCFG,           &gCctkDftEcGenHdrCfg },

   /* -- User.Network -- */
   { eCCTKPARM_EC_USRPORTCFG,          &gCctkDftEcUsrPortCfg },
   { eCCTKPARM_EC_DIAGEVTCFG,          &gCctkDftEcDiagEvtCfg },
   { eCCTKPARM_EC_UAPEVTCFG,           &gCctkDftEcUapEvtCfg },
   { eCCTKPARM_EC_USREMGCFG,           &gCctkDftEcUsrEmgCfg },
   { eCCTKPARM_EC_USRBRCFG,            &gCctkDftEcUsrBrCfg },
   { eCCTKPARM_EC_SVCROUTE,            &gCctkDftEcSvcRoute },
   { eCCTKPARM_EC_USRCONTACT,          &gCctkDftEcUsrContact },

   /* -- User.Security.Authentication -- */
   { eCCTKPARM_EC_AUTHCFG,             &gCctkDftEcAuthCfg },

   /* -- User.Security.DoS -- */
   { eCCTKPARM_EC_WHTLST,              &gCctkDftEcWhtLst },
   { eCCTKPARM_EC_BLKLST,              &gCctkDftEcBlkLst },

   /* -- Registration.General -- */
   { eCCTKPARM_EC_REGTMR,              &gCctkDftEcRegTmr},
   { eCCTKPARM_EC_REGEVTCFG,           &gCctkDftEcRegEvtCfg },

   /* -- Registration.Advanced -- */

   /* -- MWI.General -- */
   { eCCTKPARM_EC_MWICFG,              &gCctkDftEcMwiCfg },
   { eCCTKPARM_EC_MWITMR,              &gCctkDftEcMwiTmr },

   /* -- Default.Call.Basic.General -- */
   { eCCTKPARM_EC_EMGCALLCFG,          &gCctkDftEcEmgCallCfg },
#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
   { eCCTKPARM_EC_TELCALL,             &gCctkDftEcTelCall },
#endif

   /* -- Default.Call.Basic.Advanced -- */
   { eCCTKPARM_EC_PRACK,               &geCctkDftEcPrack },
   { eCCTKPARM_EC_SESTMR,              &gCctkDftEcSesTmr },
   { eCCTKPARM_EC_CALLPRIV,            &gCctkDftEcCallPriv },
   { eCCTKPARM_EC_CALLNAI,             &gCctkDftEcCallNai },
   { eCCTKPARM_EC_CONNECTEDID,         &geCctkDftEcConnectedId },

   /* -- Default.Call.Supplementary -- */
   { eCCTKPARM_EC_CALLHOLD,            &gCctkDftEcCallHold },
   { eCCTKPARM_EC_CALLXFER,            &gCctkDftEcCallXfer },

   /* -- Default.Call.Media -- */
   { eCCTKPARM_EC_MEDNEGCFG,           &gCctkDftEcMedNegCfg },

   /* -- Default.Event.General -- */
   { eCCTKPARM_EC_EVTTMR,              &gCctkDftEcEvtTmr },

   /* -- Default.Event.Advanced -- */
   { eCCTKPARM_EC_EVTPRIV,             &gCctkDftEcEvtPriv },
   { eCCTKPARM_EC_EVTNAI,              &gCctkDftEcEvtNai },

   /* -- RemoteUsr.Configuration -- */
#if CCTKCFG_REMOTE_USER_CONTROL
   { eCCTKPARM_EC_REMUSRCFG,           &gCctkDftEcRemUsrCfg },
#endif /* CCTKCFG_REMOTE_USER_CONTROL */   

   /* Must be last - always */
   { eCCTKPARM_MAX,                    NULL },
};

CCTKEXTCFGMAP gCctkDftExtSysCfg[] =
{
   /* -------------- SYSTEM CONFIG -------------- */
   /* -- Network.General -- */
   { eCCTKPARM_EC_LOCPORT,             &gCctkDftEcLocPort },
   { eCCTKPARM_EC_TRANSPORT,           &gCctkDftEcTransport },
#if CCTKCFG_TLS
   { eCCTKPARM_EC_TLSCFG,              &gCctkDftEcTlsCfg },
#endif
   { eCCTKPARM_EC_CNXACTIVITY,         &gCctkDftEcCnxActivity },

   /* -- Network.Protocol.General -- */
   { eCCTKPARM_EC_UAIDHDR,             &gCctkDftEcUaIdHdr },
   { eCCTKPARM_EC_ROUTINGHDR,          &gCctkDftEcRoutingHdr },
   { eCCTKPARM_EC_DNSMODE,             &geCctkDftEcDnsMode },
   { eCCTKPARM_EC_SIGBEHAVE,           &gCctkDftEcSigBehave },
   { eCCTKPARM_EC_INTEROP,             &guCctkDftEcInterop },
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
   { eCCTKPARM_EC_IMSCFG,              &gCctkDftEcImsCfg },
#endif
   { eCCTKPARM_EC_GRUUCFG,             &gCctkDftEcGruuCfg },
   { eCCTKPARM_EC_PAUCFG,              &gCctkDftEcPauCfg },
   { eCCTKPARM_EC_PANICFG,             &gCctkDftEcPaniCfg },
#if CCTKCFG_ICE_LITE_SUPPORT
   { eCCTKPARM_EC_ICECTLCFG,           &gCctkDftEcIceCtlCfg },
   { eCCTKPARM_EC_ICEIPLSTCFG,         &gCctkDftEcIceLst },
#endif /* CCTKCFG_ICE_LITE_SUPPORT */   

   /* -- Network.Protocol.Timer -- */
   { eCCTKPARM_EC_TRANSTMR,            &gCctkDftEcTransTmr },

   /* -- Local.General -- */

   /* -- Local.Debug -- */
   { eCCTKPARM_EC_PKTLOG,              &gCctkDftEcPktLog },
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
   { eCCTKPARM_EC_TRACE,               &gCctkDftEcTrace },
#endif

   /* -- Security.General -- */

   /* -- Security.NAT.General -- */
   { eCCTKPARM_EC_NATGENCFG,           &gCctkDftEcNatGenCfg },
   /* -- Security.NAT.RG -- */
   { eCCTKPARM_EC_NATRGCTL,            &gCctkDftEcRgNatCtl    },

   /* -- Security.NAT.STUN -- */

   /* -- Security.DoS -- */
#if CCTKCFG_MEMORY_MONITORING
   { eCCTKPARM_EC_MEMPROTECT,          &gCctkDftEcMemProtect },
#endif
   /* -- RemoteUsr.General -- */
#if CCTKCFG_REMOTE_USER_CONTROL
   { eCCTKPARM_EC_REMUSRSYS,           &gCctkDftEcRemUsrSysCfg },
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
   
   /* Must be last - always */
   { eCCTKPARM_MAX,                    NULL },
};



/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */


