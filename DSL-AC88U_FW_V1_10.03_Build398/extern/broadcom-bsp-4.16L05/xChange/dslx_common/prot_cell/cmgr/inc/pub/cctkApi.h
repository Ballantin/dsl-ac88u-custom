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

/*************************** Needed from cctkMainCfg.h *************************************/
/* Codec */
typedef enum CCTKCODEC
{
   /* Audio codecs - Narrowband */
   eCCTKCODEC_PCMU,                    /* G.711 u-law - 64 kbps */
   eCCTKCODEC_PCMA,                    /* G.711 A-law - 64 kbps */
   eCCTKCODEC_G7231,                   /* G.723.1 - 5.3/6.3 kbps */
   eCCTKCODEC_G726_16,                 /* G.726 - 16 kbps */
   eCCTKCODEC_G726_24,                 /* G.726 - 24 kbps */
   eCCTKCODEC_G726_32,                 /* G.726 - 32 kbps */
   eCCTKCODEC_G726_40,                 /* G.726 - 40 kbps */
   eCCTKCODEC_G728,                    /* G.728 - 16 kbps */
   eCCTKCODEC_G729,                    /* G.729 - 8 kbps */
   eCCTKCODEC_G729E,                   /* G.729 Annex E - 11.8 kbps */
   eCCTKCODEC_ILBC,                    /* iLBC */
   eCCTKCODEC_BV16,                    /* BroadVoice Narrowband - 16 kbps */
   eCCTKCODEC_GSMAMR,                  /* AMR-NB (G.722.1) */
   eCCTKCODEC_GSMEFR,                  /* EFR - 12.2 kbps */

   /* Audio codecs - Wideband */
   eCCTKCODEC_BV32,                    /* BroadVoice Wideband - 32 kbps */
   eCCTKCODEC_G722,                    /* G.722 (Wideband ADPCM) static payload type - 64 kbps */
   eCCTKCODEC_G722_48,                 /* G.722 - 48 kbps */
   eCCTKCODEC_G722_56,                 /* G.722 - 56 kbps */
   eCCTKCODEC_G722_64,                 /* G.722 - 64 kbps */
   eCCTKCODEC_G7221,                   /* G.722.1 (DCT codec) - 32 kbps */
#if CCTKCFG_RED
   eCCTKCODEC_RED,
#endif
   eCCTKCODEC_AMRWB,                   /* AMR-WB (G.722.2) */
   eCCTKCODEC_LPCMWB,                  /* Linear PCM-WB - 256 kbps */

#if CCTKCFG_VIDEO
   /* Video codecs */
   eCCTKCODEC_H263,                    /* H.263 */
   eCCTKCODEC_H264,                    /* H.264 */
   eCCTKCODEC_MPEG4,                   /* MPEG4 */
#endif

   eCCTKCODEC_MAX                      /* Number of codecs (not used) */
} CCTKCODEC;

/* a=silsupp parameter for voice codecs */
typedef enum CCTKSILSUPP
{
   eCCTKSILSUPP_NOTUSED,               /* a=silsupp not used */
   eCCTKSILSUPP_OFF,                   /* a=silsupp off */
   eCCTKSILSUPP_ON                     /* a=silsupp on */
} CCTKSILSUPP;

typedef enum CCTKAMRBOOL
{
   eCCTKAMR_NOTSET,                    /* Not specified */
   eCCTKAMR_YES,                       /* true */
   eCCTKAMR_NO,                        /* false */
} CCTKAMRBOOL;

typedef struct CCTKAMRPARM
{
   BOS_UINT16    uModeSet;
   CCTKAMRBOOL   bOctetAlign;
   CCTKAMRBOOL   bModeChangeNeighbor;
   CCTKAMRBOOL   bModeChangePeriod;
   BOS_UINT16    uMaxRed;
} CCTKAMRPARM;

/* CCTK firewall filter commands */
#if CMGRCFG_FIREWALL_CONTROL
typedef enum CCTKFWCMD
{
   FW_CLOSE,
   FW_OPEN,
   FW_UNKNOWN
} CCTKFWCMD;
#endif

/*******************************************************************************************/


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
    CCTKAMRPARM    amrCfg;           /* GSM-AMR/AMR-WB */
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
   BOS_UINT8     cErrorInfo[CCTK_STR_LEN + 1];
   BOS_BOOL      bLoopBack;
   BOS_BOOL      bEmergency;
   BOS_BOOL      bAnonymous;
   BOS_BOOL      bMedKeepAlive;
   BOS_UINT32    uJoin;
} CCTKCALLINFO;


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

/* For now ... only one codec allowed.
*/
#define CCTKCFG_MAX_AUDIO_CODEC    1

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


/* Media info list (for eCCTKEVT_MEDIA) */
typedef struct CCTKMEDINFO
{
   CCTKSTRMINFO   ingress;             /* Ingress stream (to net), note IP/port
                                       ** refers to the remote destination port.
                                       */
   CCTKSTRMINFO   egress;              /* Egress stream (from net), note IP/port
                                       ** refers to the local receive port.
                                       */
} CCTKMEDINFO;


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



#if defined( __cplusplus )
}
#endif

#endif /* __CCTK_API__H__INCLUDED__ */
