/**********************************************************************************
** Copyright (c) 2007-2013 Broadcom
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
**      This file contains the CCTK API.
**
***********************************************************************************/

#ifndef __CCTK_API__H__INCLUDED__
#define __CCTK_API__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <cctkCfg.h>
#include <cctkMainCfg.h>
#include <cctkExtCfg.h>
#include <bosTypes.h>
#include <bosIpAddr.h>
#include <bosTime.h>
#include <str.h>

/* ---- Constants and Types ----------------------------------------------------- */

/* Define this macro which can be used outside of the CCTK application.  The goal
** here is to share the definition of what an invalid handle is such that it is
** uniquely identified within a system and accross the CCTK API.
*/
#define CCTK_INVALID_HANDLE                0xCAFEBEEF

/* CCTK CLID length used for Caller Identification purposes.
*/
#define CCTK_CLID_LEN                  31

/* CCTK string length used for different purposes accross the CCTK interface.
*/
#define CCTK_STR_LEN                   127

/* length for the eueMgmtUsrIMPUPAUriList mib holding the P-Associated-URI header 
*/
#define CCTK_ASSUMED_ID_LEN            256 

/* CCTK function event base ID */
#define CCTKEVT_FN_BASE                10000

/* Return status macros */
#define CCTKSTATUS_IS_SUCCESS( eStatus )                                           \
                                    ( eStatus >= eCCTKSTATUS_SUCCESS_START &&      \
                                      eStatus <= eCCTKSTATUS_SUCCESS_END )
#define CCTKSTATUS_IS_FAIL( eStatus )                                              \
                                    ( eStatus >= eCCTKSTATUS_FAIL_START &&         \
                                      eStatus <= eCCTKSTATUS_FAIL_END )

/* Return status */
typedef enum CCTKSTATUS
{
   /* Successful return status */
   eCCTKSTATUS_SUCCESS_START,
                                       /* Success */
   eCCTKSTATUS_SUCCESS = eCCTKSTATUS_SUCCESS_START,
   eCCTKSTATUS_NOOP,                   /* Success - no operation needed */
   eCCTKSTATUS_SUCCESS_END = eCCTKSTATUS_NOOP,

   /* Failure return status */
   eCCTKSTATUS_FAIL_START,
                                       /* Bad parameter(s) */
   eCCTKSTATUS_BADPARM = eCCTKSTATUS_FAIL_START,
   eCCTKSTATUS_BADSTATE,               /* Bad state */
   eCCTKSTATUS_BADCNX,                 /* Bad connection */
   eCCTKSTATUS_CNXNOTREADY,            /* Connection not ready yet */
   eCCTKSTATUS_NOTINIT,                /* CCTK not initialized */
   eCCTKSTATUS_NORSC,                  /* Not enough resource */
   eCCTKSTATUS_RSCLEAK,                /* Resource leak */
   eCCTKSTATUS_RESETREQ,               /* Reset required */
   eCCTKSTATUS_INTERR,                 /* Internal error */
   eCCTKSTATUS_FAIL_END = eCCTKSTATUS_INTERR,

   eCCTKSTATUS_MAX                     /* Status count (not used) */
} CCTKSTATUS;

/* CCTK events */
typedef enum CCTKEVT
{
   eCCTKEVT_NEWCALL,                   /* New call (create resource) */
   eCCTKEVT_ENDCALL,                   /* Call ended (free resource) */
   eCCTKEVT_ALERT,                     /* Alert user of incoming call */
   eCCTKEVT_XFERCALL,                  /* Incoming transferred call */
   eCCTKEVT_ALERTED,                   /* Remote end is ringing */
   eCCTKEVT_ANSWER,                    /* Call answered by remote */
   eCCTKEVT_DISCONNECT,                /* Remote initiated release */
   eCCTKEVT_BUSY,                      /* Remote end is busy */
   eCCTKEVT_XFER,                      /* Remote initiated transfer request */
   eCCTKEVT_HOLD,                      /* Remote hold */
   eCCTKEVT_UNHOLD,                    /* Remote unhold */
   eCCTKEVT_LHOLD,                     /* Local hold accepted */
   eCCTKEVT_LHOLD_FAIL,                /* Local hold failed */
   eCCTKEVT_LUNHOLD,                   /* Local unhold accepted */
   eCCTKEVT_LUNHOLD_FAIL,              /* Local unhold failed */
   eCCTKEVT_MEDIA,                     /* Media change request */
   eCCTKEVT_MEDIA_FAIL,                /* (Local) media change failure */
   eCCTKEVT_MEDIA_XFERING,             /* Media update while transfering request */
   eCCTKEVT_MWINOTIFY,                 /* MWI notify */
   eCCTKEVT_USR_IS,                    /* User in service */
   eCCTKEVT_USR_OOS,                   /* User out of service */
   eCCTKEVT_CONFSVC,                   /* Confirmation of service */
   eCCTKEVT_ENDSVC,                    /* End of service */
   eCCTKEVT_RESET_COMPLETE,            /* Reset complete. */
   eCCTKEVT_MEM_OVERTHRESHOLD,         /* Memory allocation above threshold */
   eCCTKEVT_MEM_UNDERTHRESHOLD,        /* Memory allocation below threshold */
   eCCTKEVT_DLGNOTIFY,                 /* Dialog notify. */
   eCCTKEVT_FWDCALLNOTIFY,             /* Notification of forwarded call */
   eCCTKEVT_XFERACCEPT,                /* Notification of call transfer accepted */
   eCCTKEVT_ERROR_INFO,                /* Error information about attempted call */
   eCCTKEVT_USR_UPDATE,                /* User information update */
   eCCTKEVT_PAU_UPDATE,                /* P-Associated-URI update */
   eCCTKEVT_TARGETS_UPDATE,            /* Proxy target information update */
   eCCTKEVT_OOB_DTMF,                  /* Out-of-band DTMF information */
   eCCTKEVT_CWT_INFO,                  /* Call Waiting Tone information */
   eCCTKEVT_CDR_INFO,                  /* Call data record information */
   eCCTKEVT_PUBLISH_TARGET,            /* Publish target information update */
   eCCTKEVT_SUB_STATUS,                /* Subscription status information*/
   /* Below are events which are only defined if the CCTK framework
   ** supports remote user control.
   */
#if CCTKCFG_REMOTE_USER_CONTROL
   eCCTKEVT_REM_USR_ADD,               /* Remote user added (registered) */
   eCCTKEVT_REM_USR_REMOVE,            /* Remote user removed (unregistered) */
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
#if CCTKCFG_MESSAGE_PASS_THROUGH
   eCCTKEVT_MSG_PASS_THRU,             /* Pass thru message received. */
   eCCTKEVT_MSG_PASS_THRU_RSP,         /* Pass thru message sent - final resp. */
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */
   eCCTKEVT_MAX,                       /* Event count (not used) */

   /* CCTK API function events */
                                       /* cctkReset */
   eCCTKEVT_FN_RESET_SUCCESS = CCTKEVT_FN_BASE,
   eCCTKEVT_FN_RESET_FAIL,
   eCCTKEVT_FN_CONFIG_SUCCESS,         /* cctkConfig */
   eCCTKEVT_FN_CONFIG_FAIL,
   eCCTKEVT_FN_SETPARM_SUCCESS,        /* cctkSetParm */
   eCCTKEVT_FN_SETPARM_FAIL,
   eCCTKEVT_FN_USERADD_SUCCESS,        /* cctkUserAdd */
   eCCTKEVT_FN_USERADD_FAIL,
   eCCTKEVT_FN_USERREGISTER_SUCCESS,   /* cctkUserRegister */
   eCCTKEVT_FN_USERREGISTER_FAIL,
   eCCTKEVT_FN_USERUNREGISTER_SUCCESS, /* cctkUserUnregister */
   eCCTKEVT_FN_USERUNREGISTER_FAIL,
   eCCTKEVT_FN_USERREMOVE_SUCCESS,     /* cctkUserRemove */
   eCCTKEVT_FN_USERREMOVE_FAIL,
   eCCTKEVT_FN_EVENTCREATE_SUCCESS,    /* cctkEventCreate */
   eCCTKEVT_FN_EVENTCREATE_FAIL,
   eCCTKEVT_FN_EVENTSUBSCRIBE_SUCCESS, /* cctkEventSubscribe */
   eCCTKEVT_FN_EVENTSUBSCRIBE_FAIL,
                                       /* cctkEventUnsubscribe */
   eCCTKEVT_FN_EVENTUNSUBSCRIBE_SUCCESS,
   eCCTKEVT_FN_EVENTUNSUBSCRIBE_FAIL,
   eCCTKEVT_FN_EVENTDELETE_SUCCESS,    /* cctkEventDelete */
   eCCTKEVT_FN_EVENTDELETE_FAIL,
   eCCTKEVT_FN_CALLCREATE_SUCCESS,     /* cctkCallCreate */
   eCCTKEVT_FN_CALLCREATE_FAIL,
   eCCTKEVT_FN_CALLMAKE_SUCCESS,       /* cctkCallMake */
   eCCTKEVT_FN_CALLMAKE_FAIL,
   eCCTKEVT_FN_CALLANSWER_SUCCESS,     /* cctkCallAnswer */
   eCCTKEVT_FN_CALLANSWER_FAIL,
   eCCTKEVT_FN_CALLFORWARD_SUCCESS,    /* cctkCallForward */
   eCCTKEVT_FN_CALLFORWARD_FAIL,
   eCCTKEVT_FN_CALLHOLD_SUCCESS,       /* cctkCallHold */
   eCCTKEVT_FN_CALLHOLD_FAIL,
   eCCTKEVT_FN_CALLUNHOLD_SUCCESS,     /* cctkCallUnhold */
   eCCTKEVT_FN_CALLUNHOLD_FAIL,
   eCCTKEVT_FN_CALLTRANSFER_SUCCESS,   /* cctkCallTransfer */
   eCCTKEVT_FN_CALLTRANSFER_FAIL,
                                       /* cctkCallTransferBlind */
   eCCTKEVT_FN_CALLTRANSFERBLIND_SUCCESS,
   eCCTKEVT_FN_CALLTRANSFERBLIND_FAIL,
                                       /* cctkCallMediaNotify */
   eCCTKEVT_FN_CALLMEDIANOTIFY_SUCCESS,
   eCCTKEVT_FN_CALLMEDIANOTIFY_FAIL,
   eCCTKEVT_FN_CALLDELETE_SUCCESS,     /* cctkCallDelete */
   eCCTKEVT_FN_CALLDELETE_FAIL,
   eCCTKEVT_FN_CALLDTMFINFO_SUCCESS,   /* cctkCallDtmfInfo */
   eCCTKEVT_FN_CALLDTMFINFO_FAIL,
   eCCTKEVT_FN_CALLFLASHINFO_SUCCESS,   /* cctkCallFlashInfo */
   eCCTKEVT_FN_CALLFLASHINFO_FAIL,
   eCCTKEVT_FN_CALLPRIVINFO_SUCCESS,   /* cctkCallPrivInfo */
   eCCTKEVT_FN_CALLPRIVINFO_FAIL,
   eCCTKEVT_FN_PUBLISH_SUCCESS,        /* cctkPublish */
   eCCTKEVT_FN_PUBLISH_FAIL,
   eCCTKEVT_FN_SVCCTRL_SUCCESS,        /* cctkServiceControl */
   eCCTKEVT_FN_SVCCTRL_FAIL,
#if CCTKCFG_REMOTE_USER_CONTROL
   eCCTKEVT_FN_CALLPROXY_SUCCESS,      /* cctkCallProxy */
   eCCTKEVT_FN_CALLPROXY_FAIL,
#endif /* CCTKCFG_REMOTE_USER_CONTROL */ 
#if CCTKCFG_MESSAGE_PASS_THROUGH
   eCCTKEVT_FN_MSGPASSTHRU_SUCCESS,    /* cctkPassThruMsg */
   eCCTKEVT_FN_MSGPASSTHRU_FAIL,
   eCCTKEVT_FN_MSGPASSTHRURSP_SUCCESS, /* cctkPassThruRsp */
   eCCTKEVT_FN_MSGPASSTHRURSP_FAIL,
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */ 

   eCCTKEVT_FN_MAX
} CCTKEVT;

/* CCTK subscription events */
typedef enum CCTKSUBEVT
{
   eCCTKSUBEVT_MWI,                    /* MWI event package */
   eCCTKSUBEVT_BMWI,                   /* Blind MWI event package */
   eCCTKSUBEVT_REG,                    /* Registration event package */
   eCCTKSUBEVT_DLG,                    /* Dialog info event package */
   eCCTKSUBEVT_UAP,                    /* User-Agent profile event package */
   eCCTKSUBEVT_MAX                     /* Subscription event count (not used) */
} CCTKSUBEVT;

/* CCTK Response codes */
typedef enum CCTKRESPCODE
{
   eCCTKRESPCODE_FORBIDDEN,               /* Forbidden response code */
   eCCTKRESPCODE_TIMEOUT,                 /* Timeout response code */
   eCCTKRESPCODE_ALLREGTARGETSFAILED,     /* The target list is either empty or all targets have failed */
   eCCTKRESPCODE_MAX                     /* response count (not used) */
} CCTKRESPCODE;

/* CCTK publication type */
typedef enum CCTKPUBLISH
{
   eCCTKPUBLISH_CALLMETRICS,           /* Call Metrics (VoIP metrics) */
   eCCTKPUBLISH_PASSTHRUMSG,           /* Pass Thru Message */
   eCCTKPUBLISH_MAX                    /* Publication count (not used) */

} CCTKPUBLISH;

/* CCTK reset mode */
typedef enum CCTKRSTMODE
{
   eCCTKRSTMODE_IMMEDIATE,            /* Reset is immediate, there is
                                      ** no specific clean termination of the
                                      ** resources that may be in use at the
                                      ** time of the action.
                                      */
   eCCTKRSTMODE_IMMEDIATENOPHY,       /* Reset is immediate, there is
                                      ** no specific clean termination of the
                                      ** resources that may be in use at the
                                      ** time of the action, furthermore there
                                      ** is no specific termination of the
                                      ** physical resources associated with the
                                      ** application.
                                      */
   eCCTKRSTMODE_GRACEFULNONBLOCKING,  /* Reset is graceful but non blocking,
                                      ** meaning network resources are being
                                      ** made aware once of their clearing using
                                      ** appropriate signaling, but there is no
                                      ** wait on the local resources which are
                                      ** terminated right away.
                                      */
   eCCTKRSTMODE_GRACEFULBLOCKING,     /* Reset is graceful and blocking,
                                      ** meaning proper termination of network
                                      ** resources including indication of
                                      ** termination and wait for acknowledgment
                                      ** prior to cleaning the local resources.
                                      **
                                      ** *** NOT IMPLEMENTED CURRENTLY ***
                                      */

} CCTKRSTMODE;


/***********************************************************************************
**  FUNCTION:   cctkEvtCb
**
**  PURPOSE:    Applcation Event Callback prototype.
**
**  PARAMETERS:
**              eEvent - The event.
**              uid - User identifier.
**              cid - Call reference identifier or registration ref. id.
**              pData - Event data.
**
**  RETURNS:    Nothing.
**
**  NOTES:      uid/cid is not used when not available.
***********************************************************************************/
typedef void ( *CCTKEVTCB )( CCTKEVT eEvent,
                             BOS_UINT32 uid,
                             BOS_UINT32 cid,
                             void *pData );

/* Media update reason */
typedef enum CCTKMUPDRSN
{
   eCCTKMUDPRSN_FINALIZECAPS,          /* Finalize media caps (callee only), used at
                                       ** the beginning of the call.
                                       */
   eCCTKMUPDRSN_REFRESHSDP,            /* Refresh SDP */
   eCCTKMUDPRSN_NOCAPS,                /* No media caps (callee only), used when there is 
                                       ** remote user, just tell the MSE to send the empty 180
                                       ** Ringing.
                                       */   
   eCCTKMUPDRSN_MAX                    /* Reason count (not used) */
} CCTKMUPDRSN;

/* Call deletion reason */
typedef enum CCTKCALLDELRSN
{
   eCCTKCALLDELRSN_NORMAL,             /* Local user termination. */
   eCCTKCALLDELRSN_BUSY,               /* Local user busy (reject). */
   eCCTKCALLDELRSN_UNAVAILABLE,        /* Local user unavailable (reject). */
   eCCTKCALLDELRSN_EMERGEXPIRE,        /* Expiration of an emergency call on hold
                                       ** by local user. */
   eCCTKCALLDELRSN_RGBCKREJECT,        /* Rejection of an operator ringback. */
   eCCTKCALLDELRSN_ANONYMOUS,          /* Rejection of an anonymous caller. */
   eCCTKCALLDELRSN_TERMINATED,        /* Request Termination of ringsplash from INVITE Alert-Info */
   eCCTKCALLDELRSN_MAX                 /* Reason count (not used) */
} CCTKCALLDELRSN;

/* Codec information */
typedef struct CCTKCODECINFO
{
   CCTKCODEC      eType;               /* Codec */
   BOS_UINT8      uRtpCode;            /* RTP payload code */
   BOS_UINT32     uPtime;              /* Packetization rate (ms) */
   BOS_UINT16     uRtpPort;            /* (S)RTP port */
   BOS_UINT16     uRtcpPort;           /* (S)RTCP port */
   BOS_BOOL       bRtcpXrSupport;      /* RFC3611 RTCP-XR (voip-metrics) support. */
   CCTKSILSUPP    eSilSupp;            /* Silence suppression (for audio only) */

   /* TODO: m= line grouping */
   union
   {
      CCTKILBCMODE   eIlbcMode;        /* iLBC */
      CCTKG7231VAD   eG7231Vad;        /* G.723.1 */
      CCTKG729VAD    eG729Vad;         /* G.729x */
      CCTKAMRPARM    amrCfg;           /* GSM-AMR/AMR-WB */
#if CCTKCFG_VIDEO
      CCTKH263PARM   h263;             /* H.263 */
      CCTKH264PARM   h264;             /* H.264 */
#endif
#if CCTKCFG_RED
      CCTKREDPARM    redCfg;
#endif
   } parm;
} CCTKCODECINFO;

/* Bitmap for televt types */
#define CCTKNTE_DTMF        ( 1 << 0 )  /* DTMF tone 0 - 9, *, #, A - D (0 - 15) */
#define CCTKNTE_FLASH       ( 1 << 1 )  /* Flash (16) */
#define CCTKNTE_OFFHOOK     ( 1 << 2 )  /* Off-hook (64) */
#define CCTKNTE_ONHOOK      ( 1 << 3 )  /* On-hook (65) */
#define CCTKNTE_DIALTONE    ( 1 << 4 )  /* Dial tone (66) */
#define CCTKNTE_RING        ( 1 << 5 )  /* Ringing tone (70) */
#define CCTKNTW_OFFHOOKWARN ( 1 << 6 )  /* Off-hook warning tone (88) */

/* Telephone-event information */
typedef struct CCTKTELEVT
{
   BOS_UINT8      uRtpCode;            /* RTP payload code for narrowband */
   BOS_UINT8      uRtpCodeWB;          /* RTP payload code for wideband */
   BOS_UINT16     uRtpPort;            /* (S)RTP port */
   BOS_UINT16     uRtcpPort;           /* (S)RTP port */
   BOS_BOOL       bRtcpXrSupport;      /* RFC3611 RTCP-XR (voip-metrics) support. */
   BOS_UINT32     uEvtTypes;           /* Bitmap for televt types (CCTKNTE_XXX).
                                       ** Set to 0 to not include the a=fmtp line.
                                       */

   /* TODO: m= line grouping */
} CCTKTELEVT;

/* Comfort noise information */
typedef struct CCTKCN
{
   BOS_UINT8      uRtpCode;            /* RTP payload code for narrowband */
   BOS_UINT8      uRtpCodeWB;          /* RTP payload code for wideband */
   BOS_UINT16     uRtpPort;            /* (S)RTP port */
   BOS_UINT16     uRtcpPort;           /* (S)RTP port */
   BOS_BOOL       bRtcpXrSupport;      /* RFC3611 RTCP-XR (voip-metrics) support. */

   /* TODO: m= line grouping */
} CCTKCN;

/* Media mode */
typedef enum CCTKMEDMODE
{
   eCCTKMEDMODE_INACTIVE,              /* Inactive */
   eCCTKMEDMODE_ACTIVE,                /* Active */
   eCCTKMEDMODE_LOOPBACK,              /* Loopback */
   eCCTKMEDMODE_CONFERENCE,            /* Conference */
   eCCTKMEDMODE_MAX                    /* Media mode count (not used) */
} CCTKMEDMODE;

/* Loopback type */
typedef enum CCTKLBTYPE
{
   eCCTKLBTYPE_PKTLB,                  /* Packet level loopback */
   eCCTKLBTYPE_MEDIALB,                /* Media level loopback */
   eCCTKLBTYPE_MAX                     /* Loopback type count (not used) */
} CCTKLBTYPE;

/* Packet loopback type */
typedef enum CCTKPKTLBTYPE
{
   eCCTKPKTLBTYPE_ENCAPRTP,            /* Encapsulated RTP */
   eCCTKPKTLBTYPE_RTPLB,               /* RTP loopback */
   eCCTKPKTLBTYPE_MAX                  /* Packet loopback count (not used) */
} CCTKPKTLBTYPE;

/* Loopback attribute info */
typedef struct CCTKLBINFO
{
   BOS_BOOL       bIsSrc;              /* Is Loopback Source (or Mirror)? */
   BOS_BOOL       bIsStart;            /* Loopback Mirror start enabled */
   CCTKLBTYPE     eLbType;             /* Loopback type */

   /* Used when eLbType is eCCTKLBTYPE_PKTLB */
   CCTKPKTLBTYPE  ePktLbType;          /* Packet loopback type */
   BOS_UINT8      uRtpCode;            /* RTP payload code */
} CCTKLBINFO;

#if CCTKCFG_SRTP
#define CCTK_KEYSALT_LEN 29

typedef struct CCTKCRYPTOINFO
{
   BOS_BOOL       bIsSpecified;        /* Indicates if the parameters are used. */
   CCTKCSUITE     eCryptoSuite;        /* Crypto suite */
   BOS_UINT8      cKeySalt[CCTK_KEYSALT_LEN + 1];
                                       /* Key and salt concatenated */

   BOS_UINT64     uLifeTime;           /* Set val to '0' to use the default */
#if ( CCTKCFG_MAX_CRYPTO_SESPARM > 0 )
   BOS_UINT32     uNumSessionParm;     /* Number of sessions parameters */
   CCTKCSESPARAM  session[CCTKCFG_MAX_CRYPTO_SESPARM];
                                       /* Session parameters */
#endif
} CCTKCRYPTOINFO;
#endif

typedef struct CCTKTRANSPORTADDR
{
   BOS_IP_ADDRESS addrLcl;
   BOS_UINT16     rtpPortLcl;
   BOS_UINT16     rtcpPortLcl;
   
   BOS_IP_ADDRESS addrRmt;
   BOS_UINT16     rtpPortRmt;
   BOS_UINT16     rtcpPortRmt;
} CCTKTRANSPORTADDR;

/* Audio stream info */
typedef struct CCTKAUDINFO
{
   CCTKMEDMODE    mode;                /* Media mode */
   BOS_UINT32     uNumCodec;           /* Number of audio codecs */
   CCTKCODECINFO  codec[CCTKCFG_MAX_AUDIO_CODEC];
                                       /* Codec info */
   BOS_BOOL       bTelEvtUsed;         /* Whether telephone-event is used */
   CCTKTELEVT     televt;              /* Telephone-event info */
   BOS_BOOL       bCnUsed;             /* Whether Comfort Noise "codec" is used */
   CCTKCN         cn;                  /* Comfort Noise info */
   CCTKLBINFO     lb;                  /* Used when mode is eCCTKMEDMODE_LOOPBACK */
   BOS_UINT32     uMaxPtime;           /* Max packetization rate (ms), 0 if not used
                                       ** See also uPtime under CCTKCODECINFO.
                                       */
#if CCTKCFG_SRTP
   CCTKCRYPTOINFO crypto;              /* SRTP crypto */
#endif
} CCTKAUDINFO;

#if CCTKCFG_VIDEO
/* Video stream info */
typedef struct CCTKVIDINFO
{
   CCTKMEDMODE    mode;                /* Media mode */
   BOS_UINT32     uNumCodec;           /* Number of audio codecs */
   CCTKCODECINFO  codec[CCTKCFG_MAX_VIDEO_CODEC];
                                       /* Codec info */
#if CCTKCFG_SRTP
   CCTKCRYPTOINFO crypto;              /* SRTP crypto */
#endif
} CCTKVIDINFO;
#endif

/* T.38 stream info */
typedef struct CCTKT38INFO
{
   BOS_UINT16     uPort;               /* T.38 port, 0 if not used */
   BOS_BOOL       bRedundancy;         /* Whether Redundancy is used */
} CCTKT38INFO;

typedef struct CCTKSTRMINFO
{
   BOS_IP_ADDRESS addr;                /* Media address */
   CCTKAUDINFO    audio;               /* Audio stream info */
#if CCTKCFG_VIDEO
   CCTKVIDINFO    video;               /* Video stream info */
#endif
   CCTKT38INFO    t38;                 /* T.38 stream info */
   /* TODO: m= line grouping support */
} CCTKSTRMINFO;

typedef struct CCTKICEMEDIAINFO
{
   /* addr/port pairs */
   BOS_UINT32        uNumTsAddr;
   CCTKTRANSPORTADDR transportAddr[CCTKCFG_MAX_TRANSPORT_ADDRESS];

   /* flags for Media Flow */
   BOS_BOOL       bIceTsNotReady; /* no valid pair yet for audio stream. */
} CCTKICEMEDIAINFO;

typedef struct CCTKICEINFO
{
   CCTKICEMEDIAINFO audio;
#if CCTKCFG_VIDEO
   CCTKICEMEDIAINFO video;
#endif   
   CCTKICEMEDIAINFO t38;
   CCTKEVT          eCctkEvt;   
} CCTKICEINFO;

/* Media info list (for eCCTKEVT_MEDIA) */
typedef struct CCTKMEDINFO
{
#if CCTKCFG_ICE_LITE_SUPPORT
   CCTKICEINFO    iceInfo;             /* all the transport addresses that need 
                                        * to be ready to receive media data. */
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
   
   CCTKSTRMINFO   ingress;             /* Ingress stream (to net), note IP/port
                                       ** refers to the remote destination port.
                                       */
   CCTKSTRMINFO   egress;              /* Egress stream (from net), note IP/port
                                       ** refers to the local receive port.
                                       */
} CCTKMEDINFO;

/* Caller identification information.
*/
typedef struct CCTKCLIDINFO
{
   BOS_UINT8     cName[CCTK_CLID_LEN + 1];
   BOS_UINT8     cNumber[CCTK_CLID_LEN + 1];
} CCTKCLIDINFO;

/* Caller information.
*/
typedef struct CCTKCALLINFO
{
   CCTKCLIDINFO  clid;
   BOS_UINT8     cAlertInfo[CCTKCFG_MAX_ALERT_INFO][CCTK_STR_LEN + 1];
   BOS_UINT8     cErrorInfo[CCTK_STR_LEN + 1];
   BOS_BOOL      bLoopBack;
   BOS_BOOL      bEmergency;
   BOS_BOOL      bAnonymous;
   BOS_BOOL      bMedKeepAlive;
   BOS_UINT32    uJoin;
   
#if CCTKCFG_REMOTE_USER_CONTROL
   /* Additional call information for redirecting a remote user call */
   BOS_UINT8     cFromUserName[CCTK_STR_LEN + 1];  /* username in FROM header */
   BOS_UINT8     cTarget[CCTK_STR_LEN + 1];        /* Target username for the INVITE */
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

} CCTKCALLINFO;

/* Call data record information.
*/
typedef struct CCTKCDR
{
   BOS_UINT8  uCallId[CCTK_STR_LEN + 1];
   BOS_UINT8  uPeer[CCTK_STR_LEN + 1];

   CCTKCLIDINFO clid;
      
   BOS_BOOL   bAnonymous;
   BOS_BOOL   bEmergency;
   BOS_BOOL   bAccepted;

   time_t     timeStart;
   time_t     timeStop;

} CCTKCDR;

/* Message waiting indication information.
*/
typedef struct CCTKMWIVOICE
{
   BOS_UINT32 newMsg;
   BOS_UINT32 oldMsg;
   BOS_UINT32 newUrgMsg;
   BOS_UINT32 oldUrgMsg;

} CCTKMWIVOICE;

typedef struct CCTKMWIINFO
{
   BOS_BOOL      bMsgStatus;
   CCTKMWIVOICE  voiceMsg;

} CCTKMWIINFO;

typedef struct CCTKFEATINFO
{
   BOS_BOOL      bCfv;
   BOS_BOOL      bScf;
   BOS_BOOL      bDnd;

   BOS_UINT32    uCfvCnt;
   BOS_UINT32    uScfCnt;

} CCTKFEATINFO;

typedef struct CCTKKAINFO
{
   BOS_UINT8 uKaPeer[CCTK_STR_LEN + 1];
   BOS_BOOL bDatagram;

} CCTKKAINFO;

#define CCTK_INGRESS_CODEC_LIST_SIZE 400
#define CCTK_EGRESS_CODEC_LIST_SIZE  400

typedef struct CCTKCALLSTATS
{
   BOS_UINT32   uLclSSRC;
   BOS_UINT32   uLclPktSent;
   BOS_UINT32   uLclOctetSent;
   BOS_UINT32   uLclPktRecvd;
   BOS_UINT32   uLclOctetRecvd;
   BOS_SINT32   sLclPktLost;
   BOS_UINT16   uLclJitter;
   BOS_UINT16   uLclLatency;
   BOS_UINT32   uRemSSRC;
   BOS_UINT32   uRemPktSent;
   BOS_UINT32   uRemOctetSent;
   BOS_SINT32   sRemPktLost;
   BOS_UINT32   uRemJitter;
   BOS_UINT32   uLclSilencePktSent;
   BOS_UINT32   uLclSilenceOctetSent;
   BOS_UINT32   uLclSilencePktRecvd;
   BOS_UINT32   uLclSilenceOctetRecvd;
   BOS_UINT16   uBurstLost;
   BOS_UINT16   uLostSequence;   
   BOS_UINT8    uIngressCodecList[CCTK_INGRESS_CODEC_LIST_SIZE]; 
   BOS_UINT8    uEgressCodecList[CCTK_EGRESS_CODEC_LIST_SIZE]; 

} CCTKCALLSTATS;

typedef struct CCTKEXTCALLSTATS
{
   BOS_UINT8  uNetPktLossRate;
   BOS_UINT8  uJbDiscardRate;
   BOS_UINT8  uBurstLossDensity;
   BOS_UINT8  uGapLossDensity;
   BOS_UINT16 uBurstDuration;
   BOS_UINT16 uGapDuration;
   BOS_UINT16 uRdTripNetDelay;
   BOS_UINT16 uEndSystemDelay;
   BOS_UINT8  uMinGapThreshold;
   BOS_SINT8  sSignalLevel;
   BOS_SINT8  sNoiseLevel;
   BOS_UINT8  uResidualEchoRetLoss;
   BOS_UINT8  uRFactor;
   BOS_UINT8  uExtRFactor;
   BOS_UINT8  uEstMOSLQ;
   BOS_UINT8  uEstMOSCQ;
   BOS_UINT8  uPktLossConcealmentType;
   BOS_UINT8  uJbAdaptive;
   BOS_UINT8  uJbRate;
   BOS_UINT8  uReserved;
   BOS_UINT16 uJbNominal;
   BOS_UINT16 uJbMax;
   BOS_UINT16 uJbAbsMax;
   BOS_UINT16 uInterArrivalJitter;

} CCTKEXTCALLSTATS;

typedef struct CCTKCALLMETRICS
{
   BOS_UINT32 uUserGuid;
   CCTKCALLSTATS callStats;
   BOS_BOOL bExtCallStats;
   CCTKEXTCALLSTATS extCallStatsLoc;
   CCTKEXTCALLSTATS extCallStatsRem;

} CCTKCALLMETRICS;

typedef struct CCTKXRTPSTAT
{
   CCTKCALLMETRICS callMetrics;
   BOS_IP_ADDRESS ipLcl;
   BOS_UINT16     uPortLcl;
   BOS_UINT32     uPtimeLcl;
   BOS_IP_ADDRESS ipRem;
   BOS_UINT16     uPortRem;
   BOS_UINT32     uPtimeRem;
   BOS_TIME_MS    tSetupDuration;
   BOS_TIME_MS    tCallDuration;

} CCTKXRTPSTAT;

/* Target information.
*/
typedef struct CCTKTARGETINFO
{
   BOS_UINT8      uNumTargets;
   BOS_IP_ADDRESS targetipAddr[CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS];
   BOS_UINT16     uTargetPort[CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS];

} CCTKTARGETINFO;

typedef struct CCTKEXPIREINFO
{
   BOS_UINT32 uExp;           /* Time from tUpdateTime until the current registration expires */
   BOS_UINT32 uReReg;         /* Time from tUpdateTime until the next register */
   BOS_BOOL   bPrimary;       /* Flag whether currently registered to primary target */
   time_t     tUpdateTime;    /* Time when the expiry and re-reg values were set */
} CCTKEXPIREINFO;

typedef struct CCTKREMUSRINFO
{
   BOS_UINT8 uUsrName[CCTK_STR_LEN + 1];
   BOS_UINT8 uUsrSigHost[CCTK_STR_LEN + 1];
   BOS_UINT16 uUsrSigPort;

} CCTKREMUSRINFO;

typedef struct
{
   BOS_UINT8 *puData;
   BOS_UINT32 uSize;

} CCTKMSGPASSTHRU;

typedef struct
{
   BOS_UINT32 uDtmf;
   BOS_UINT32 uDuration;

} CCTKOOBDTMF;

typedef struct
{
   BOS_UINT8     cwt[CCTK_STR_LEN + 1];

} CCTKCWTINFO;

typedef struct
{
   BOS_UINT8     pau[CCTK_ASSUMED_ID_LEN + 1];
} CCTKPAUINFO;

typedef struct
{
   BOS_UINT32 incomingCalls;
   BOS_UINT32 incomingAnsweredCalls;
   BOS_UINT32 incomingConnectedCalls;
   BOS_UINT32 incomingFailedCalls;
   BOS_UINT32 outgoingCalls;
   BOS_UINT32 outgoingAnsweredCalls;
   BOS_UINT32 outgoingConnectedCalls;
   BOS_UINT32 outgoingFailedCalls;
} CCTK_STATS;

typedef struct
{
   CCTKCDR cdr;
   BOS_BOOL bIncoming;
   CCTK_STATS cctkStats;
   BOS_UINT8 localSip[CCTK_STR_LEN + 1];
   BOS_UINT8 remoteSip[CCTK_STR_LEN + 1];

} CCTKCDRINFO;

/* ---- Function Prototypes ----------------------------------------------------- */

#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
**  FUNCTION:   cctkStartup
**
**  PURPOSE:    Initializes and starts up CCTK.
**
**  PARAMETERS:
**              evtCb - Event "callback" function pointer.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
**              - This function also initializes internal queue, thread, framework
**                and the SIP stack.
***********************************************************************************/
CCTKSTATUS cctkStartup( CCTKEVTCB evtcb );

/***********************************************************************************
**  FUNCTION:   cctkShutdown
**
**  PURPOSE:    Shuts down CCTK.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
**              - This function also shuts down internal queue, thread, framework
**                and the SIP stack.
**              - Termination is immediate. All resources are destroyed forcefully.
**                Application is responsible for taking care of call signalling
**                clean-ups such as by calling cctkReset() before calling this API.
***********************************************************************************/
CCTKSTATUS cctkShutdown( void );

/***********************************************************************************
**  FUNCTION:   cctkReset
**
**  PURPOSE:    Cleans up CCTK resources.
**
**  PARAMETERS:
**              eRstMode - Reset mode to be applied for this particular action.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkReset( CCTKRSTMODE eRstMode );

/***********************************************************************************
**  FUNCTION:   cctkConfig
**
**  PURPOSE:    Configure CCTK after startup.
**
**  PARAMETERS:
**              pCfg - Configuration block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - API can be called multiple times to reconfigure CCTK.
**              - Application is responsible for ensuring CCTK does not have
**                calls in progress or user registered. App must take down all
**                calls and registrations such as by calling cctkReset() before
**                calling this API. Error will be returned otherwise.
***********************************************************************************/
CCTKSTATUS cctkConfig( const CCTKCFG *pCfg );

/***********************************************************************************
**  FUNCTION:   cctkSetParm
**
**  PURPOSE:    Set parameter for id.
**
**  PARAMETERS:
**              uId     - Object id to set parameter.
**                        CCTK_INVALID_HANDLE is used for system configurations.
**              eParm   - Parameter id.
**              pVal    - Parameter value.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - Can be used when CCTK is enabled.
***********************************************************************************/
CCTKSTATUS cctkSetParm( BOS_UINT32 uId, CCTKPARM eParm, const void *pVal );

/***********************************************************************************
**  FUNCTION:   cctkGetParm
**
**  PURPOSE:    Get parameter for id.
**
**  PARAMETERS:
**              uId     - Object id to set parameter.
**              eParm   - Parameter id.
**              pVal    - Parameter value.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
**              - Can be used when CCTK is enabled.
***********************************************************************************/
CCTKSTATUS cctkGetParm( BOS_UINT32 uId, CCTKPARM eParm, void *pVal );

/***********************************************************************************
**  FUNCTION:   cctkUserAdd
**
**  PURPOSE:    Add a user to the system.
**
**  PARAMETERS:
**              pUid - User id (id created on return).
**              pCfg - User config block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkUserAdd( BOS_UINT32 *pUid, const CCTKUSRCFG *pCfg );

/***********************************************************************************
**  FUNCTION:   cctkUserRegister
**
**  PURPOSE:    Sends a REGISTER to register the configured user.
**
**  PARAMETERS:
**              uUid - User id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkUserRegister( BOS_UINT32 uUid );

/***********************************************************************************
**  FUNCTION:   cctkUserUnregister
**
**  PURPOSE:    Send a REGISTER with expires=0 to unregister the contact.
**
**  PARAMETERS:
**              uUid    - User id.
**              bClear  - Set clear to TRUE to unregister all registrar-cached
**                        contacts associated with the Address of Record (AoR).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkUserUnregister( BOS_UINT32 uUid, BOS_BOOL bClear );

/***********************************************************************************
**  FUNCTION:   cctkUserRemove
**
**  PURPOSE:    Remove the user from the system.
**
**  PARAMETERS:
**              uUid - User id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The uUid must not be used on successful return.
***********************************************************************************/
CCTKSTATUS cctkUserRemove( BOS_UINT32 uUid );

/***********************************************************************************
**  FUNCTION:   cctkEventCreate
**
**  PURPOSE:    Create an event object for subscription.
**
**  PARAMETERS:
**              pEid       - Subscription event id (id created on return).
**              uUid       - User id for the subscription.
**              eSubtype   - Subscription event type (e.g. reg-event).
**              uExpTime   - Subscription expiration time (must be non-zero).
**              pcTo       - Subscription destination address (URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkEventCreate( BOS_UINT32 *pEid,
                            BOS_UINT32 uUid,
                            CCTKSUBEVT eSubtype,
                            BOS_UINT32 uExpTime,
                            const BOS_UINT8 *pcTo );

/***********************************************************************************
**  FUNCTION:   cctkEventSubscribe
**
**  PURPOSE:    Send a SUBSCRIBE to the event service.
**
**  PARAMETERS:
**              uEid - Event id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkEventSubscribe( BOS_UINT32 uEid );

/***********************************************************************************
**  FUNCTION:   cctkEventUnsubscribe
**
**  PURPOSE:    Send a SUBSCRIBE with Expires equal to 0 to unscribe from
**              the event service.
**
**  PARAMETERS:
**              uEid - Event id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkEventUnsubscribe( BOS_UINT32 uEid );

/***********************************************************************************
**  FUNCTION:   cctkEventDelete
**
**  PURPOSE:    Unsubscribe and delete the (subscribed) event.
**
**  PARAMETERS:
**              uEid - Event id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The uEid must not be used on return.
***********************************************************************************/
CCTKSTATUS cctkEventDelete( BOS_UINT32 uEid );

/***********************************************************************************
**  FUNCTION:   cctkCallCreate
**
**  PURPOSE:    Creates a handle for call operation.
**
**  PARAMETERS:
**              pCid - Call id (created on return).
**              uUid - User id for the call.
**              pcTo - Call destination address (dial-string or URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallCreate( BOS_UINT32 *pCid,
                           BOS_UINT32 uUid,
                           const BOS_UINT8 *pcTo );

/***********************************************************************************
**  FUNCTION:   cctkCallMake
**
**  PURPOSE:    Initiate a call (Send an INVITE).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallMake( BOS_UINT32 uCid );

/***********************************************************************************
**  FUNCTION:   cctkCallAnswer
**
**  PURPOSE:    Answer the call (Send a 200 OK for the INVITE).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallAnswer( BOS_UINT32 uCid );

/***********************************************************************************
**  FUNCTION:   cctkCallForward
**
**  PURPOSE:    Forward the call (Send a 3xx response for the INVITE).
**
**  PARAMETERS:
**              uCid    - Call id.
**              pcFwdTo - Forward-to address (dial-string or URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallForward( BOS_UINT32 uCid, const BOS_UINT8 *pcFwdTo );

/***********************************************************************************
**  FUNCTION:   cctkCallHold
**
**  PURPOSE:    Hold the current call (Send a re-INVITE with SDP/a=sendonly).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallHold( BOS_UINT32 uCid );

/***********************************************************************************
**  FUNCTION:   cctkCallUnhold
**
**  PURPOSE:    Unhold the current call (Send a re-INVITE with SDP/a=sendonly).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallUnhold( BOS_UINT32 uCid );

/***********************************************************************************
**  FUNCTION:   cctkCallTransfer
**
**  PURPOSE:    Attended call transfer (REFER with replaces).
**
**  PARAMETERS:
**              uCid    - Call id for the call to the transferer.
**              uXcid   - Call id for the call to transfer to.
**              bNoCallTermination - Boolean to indicate whether to terminate the calls
**                                   upon successful transfer. When set to BOS_FALSE, results in 
**                                   standard call transfer behaviour with call termination
**                                   done by CCTK upon successful transfer. When set to BOS_TRUE,
**                                   results in special tranfer behaviour that does not terminate 
**                                   any calls as needed for server-based 
**                                   transfer scenario.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallTransfer( BOS_UINT32 uCid, BOS_UINT32 uXcid, BOS_BOOL bNoCallTermination );

/***********************************************************************************
**  FUNCTION:   cctkCallTransferBlind
**
**  PURPOSE:    Unattended call transfer (REFER no replaces).
**
**  PARAMETERS:
**              uCid   - Call id for the call to the transferer.
**              pcAddr - Transfer-to address (dial-string or URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallTransferBlind( BOS_UINT32 uCid, const BOS_UINT8 *pcAddr );

/***********************************************************************************
**  FUNCTION:   cctkCallMediaNotify
**
**  PURPOSE:    Update remote party with the local media change.
**
**  PARAMETERS:
**              uCid - Call id.
**              eReason - Reason for the update.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - eReason eCCTKMUDPRSN_FINALIZECAPS:
**                For callee only at the beginning of the call.  Used to confirm
**                with CCTK that the media caps is set.
**              - eReason eCCTKMUPDRSN_REFRESHSDP:
**                Used to trigger a SDP offer to be sent.
***********************************************************************************/
CCTKSTATUS cctkCallMediaNotify( BOS_UINT32 uCid, CCTKMUPDRSN eReason );

/***********************************************************************************
**  FUNCTION:   cctkCallDelete
**
**  PURPOSE:    Delete allocated call resources.
**
**  PARAMETERS:
**              uCid - Call id.
**              eReason - Reason for the call deletion.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - Send BYE / terminate the current call.
**              - The uCid must not be used on return.
***********************************************************************************/
CCTKSTATUS cctkCallDelete( BOS_UINT32 uCid, CCTKCALLDELRSN eReason );

/***********************************************************************************
**  FUNCTION:   cctkCallDtmfInfo
**
**  PURPOSE:    Issues an information message about DTMF detected on the call
**              resource.
**
**  PARAMETERS:
**              uCid - Call id.
**              uDtmf - DTMF identifier.
**              uDur - Duration of the DTMF.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallDtmfInfo( BOS_UINT32 uCid, BOS_UINT32 uDtmf, BOS_UINT32 uDur );

/***********************************************************************************
**  FUNCTION:   cctkCallFlashInfo
**
**  PURPOSE:    Issues an information message about hook flash detected on the call
**              resource.
**
**  PARAMETERS:
**              uCid - Call id.
**              pcHfMimeType - Content-Type of the INFO message
**              pcHfContent  - Content of the INFO message
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallFlashInfo( BOS_UINT32 uCid,
                              const BOS_UINT8 *pcHfMimeType,
                              const BOS_UINT8 *pcHfContent );

/***********************************************************************************
**  FUNCTION:   cctkCallPrivInfo
**
**  PURPOSE:    Updates the privacy information for the current call.
**
**  PARAMETERS:
**              uCid - Call id.
**              pPrivCfg - Privacy config.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallPrivInfo( BOS_UINT32 uCid, const CCTKPRIVCFG *pPrivCfg );

/***********************************************************************************
**  FUNCTION:   cctkMemStatusShow
**
**  PURPOSE:    Show the memory allocation status.
**
**  PARAMETERS:
**              bCctkOnly     - True to retrieve the amount of memory allocated
**                              by CCTK.
**              bTraceTable   - True to trace the entire memory allocation table.
**                              If bCctkOnly is enabled, only the memory blocks
**                              allocated by CCTK are traced.
**                              Not used when CCTK_TRACELVL_INFO is disabled.
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
void cctkMemStatusShow( BOS_BOOL bCctkOnly, BOS_BOOL bTraceTable );


/***********************************************************************************
**  FUNCTION:   cctkPublish
**
**  PURPOSE:    Publishes information from the application to the remote end.
**
**  PARAMETERS:
**              uUsrGuid - The user GUID associated with this action.
**              uResGuid - The resource GUID associated with this action.
**              eType    - The type of publication.
**              pcTo     - Where to publish.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The type (eType) passed as parameter is used to determine how
**                to interpret the resource GUID.  For example, if the type is
**                'call-metrics', then the GUID must be the call identifier of
**                interest.
**              - This API is to be used for publication of out-of-dialog
**                information which will not affect the resource state it applies
**                to in any ways.  It simply uses the resource as source of
**                necessary information in order to know what, how and to whom the
**                publication is to be made.  The resource target reserves the
**                right to refuse/reject a publication at any time.
***********************************************************************************/
CCTKSTATUS cctkPublish( BOS_UINT32 uUsrGuid,
                        BOS_UINT32 uResGuid, 
                        CCTKPUBLISH eType,
                        const BOS_UINT8 *pcTo );


/***********************************************************************************
**  FUNCTION:   cctkServiceControl
**
**  PURPOSE:    Controls a service that is owned by the CCTK.
**
**  PARAMETERS:
**              uUsrGuid - The user GUID associated with this action.
**              eType    - The type of service to control.
**              bEnable  - Whether to enable or disable the service.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The type (eType) passed as parameter is used to determine what
**                specific action is to be taken for enabling or disabling the
**                service.
**              - This API is to be used for control over a service that is owned
**                by CCTK and on which the outside application has no visibility
**                or direct control, instead it relies on the CCTK user module to
**                apply the needed action for the specific service.
***********************************************************************************/
CCTKSTATUS cctkServiceControl( BOS_UINT32 uUsrGuid,
                               CCTKSUBEVT eType,
                               BOS_BOOL bEnable );


/***********************************************************************************
**  FUNCTION:   cctkStatusShow
**
**  PURPOSE:    Show the data status information for those data under CCTK
**              control.
**
**  PARAMETERS: None
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
void cctkStatusShow( void );


#if CCTKCFG_REMOTE_USER_CONTROL
/***********************************************************************************
**  FUNCTION:   cctkCallProxy
**
**  PURPOSE:    Proxy a call resource.
**
**  PARAMETERS:
**              uCid - Call id.
**              uPid - Call id of the proxy resource.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - Forward the current call on to the network and proxy the
**                result to the user.
**              - The uCid must not be used on return.
***********************************************************************************/
CCTKSTATUS cctkCallProxy( BOS_UINT32 uCid, BOS_UINT32 uPid );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */


#if CCTKCFG_MESSAGE_PASS_THROUGH
/***********************************************************************************
**  FUNCTION:   cctkPassThruMsg
**
**  PURPOSE:    Issue a pass through message from the application via a MESSAGE
**              method.
**
**  PARAMETERS:
**              puResGuid - The resource GUID created to manage this action.
**              uUsrGuid  - The user GUID associated with this action.
**              pcTo      - Where to send the message to.
**              pcBody    - The body of the message to be happended (transparent).
**              uSize     - The size of the body information to be happended.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The application must keep reference of the returned GUID
**                (puResGuid) to be used later on when final status for this
**                message is received.
**              - Once final response is received and passed to the application
**                the resource is terminated and the application may release the
**                GUID.  Further action on the GUID will have no effect.
***********************************************************************************/
CCTKSTATUS cctkPassThruMsg( BOS_UINT32 *puResGuid,
                            BOS_UINT32 uUsrGuid,
                            const BOS_UINT8 *pcTo,
                            const BOS_UINT8 *pcBody,
                            const BOS_UINT32 uSize );


/***********************************************************************************
**  FUNCTION:   cctkPassThruRsp
**
**  PURPOSE:    Issue a final response to a pass through request received prior.
**
**  PARAMETERS:
**              uResGuid  - The resource GUID to associate this response with.
**              uFinalRsp - The final response indication.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The application MUST pass the response in the form of a
**                2xx/4xx/5xx/6xx final SIP response.
**              - Provisional (1xx/3xx) responses are unacceptable and will
**                be remapped to a default 200 response if invoked.
**              - Following this API call, the application may release the GUID
**                associated with the pass through request as it will be removed
**                from internal knowledge and will be invalid.
***********************************************************************************/
CCTKSTATUS cctkPassThruRsp( BOS_UINT32 uResGuid,
                            BOS_UINT32 uFinalRsp );
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */


/***********************************************************************************
**  FUNCTION:   cctkGenerateId
**
**  PURPOSE:    Generate a unique object id for identification.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Returns a global unique identifier or an invalid value if
**              such identifier cannot be selected.
**
**  NOTES:      This function is synchronous.
***********************************************************************************/
BOS_UINT32 cctkGenerateId( void );


#if defined( __cplusplus )
}
#endif

#endif /* __CCTK_API__H__INCLUDED__ */
