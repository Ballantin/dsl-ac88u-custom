/**********************************************************************************
** Copyright (c) 2008-2012 Broadcom
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
**      This file contains the CCTK main configuration.
**
***********************************************************************************/

#ifndef __CCTK_MAIN_CFG_API__H__INCLUDED__
#define __CCTK_MAIN_CFG_API__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <cctkCfg.h>
#include <bosTypes.h>
#include <bosIpAddr.h>

/* ---- Constants and Types ----------------------------------------------------- */
#define CCTK_IPV6_SIZE     ( 16 )      /* Max size of numerical IPv6 address */

#define CCTK_AMR_WB_MAX_MODE  9        /* Max number of mode possible for AMR-WB */
#define CCTK_AMR_NB_MAX_MODE  8        /* Max number of modes possible for AMR-NB */ 

/* SIP name information */
typedef struct CCTKSIPNAME
{
   BOS_UINT8      *pcUsrName;          /* SIP user name
                                       ** (e.g. E.164, user name, etc)
                                       */
   BOS_UINT8      *pcDisplay;          /* SIP display name */

   BOS_BOOL       bUsrNameFromTempId;  /* Flag to indicate that the SIP name was
                                       ** derived from a temporary ID
                                       */
} CCTKSIPNAME;

/* Route type */
typedef enum CCTKROUTETYPE
{
   eCCTKROUTETYPE_PRELOADED,           /* Pre-loaded routing */
   eCCTKROUTETYPE_LOCALPOLICY,         /* Local policy routing */
} CCTKROUTETYPE;

/* Host port address information */
typedef struct CCTKHOSTPORT
{
   BOS_UINT8      *pcAddr;             /* FQDN/domain/IP address */
   BOS_UINT16     uPort;               /* Port number (host byte ordering) */
} CCTKHOSTPORT;

/* SIP host port address information */
typedef struct CCTKSIPHOST
{
   CCTKHOSTPORT   hostPort;            /* Host port address information */
   BOS_BOOL       bSec;                /* True to indicate the SIP host port as
                                       ** secured (SIPS via TLS). The
                                       ** certificates/keys in CCTKTLSCFG would be
                                       ** used to establish a secure connection.
                                       ** Not used if CCTK_TRANSPORT_TLS is not set
                                       ** when configuring transport.
                                       ** False to treat host port as unsecured
                                       ** (SIP via TCP/UDP).
                                       */
} CCTKSIPHOST;

/* Routing information */
typedef struct CCTKROUTE
{
   CCTKSIPHOST    addr;                /* The SIP host address of the proxy to which
                                       ** all initial outgoing requests are sent.
                                       ** Set the "pcAddr" to NULL if not used.
                                       */
   BOS_BOOL       bLr;                 /* Indicates whether loose route (lr)
                                       ** parameter is included. If the proxy is the
                                       ** first of the route set, defining lr has 
                                       ** the effect of having the proxy's URI to 
                                       ** appear in the Route header instead of the 
                                       ** Request-URI. 
                                       */
} CCTKROUTE;

/* Route set */
typedef struct CCTKROUTELST
{
   CCTKROUTETYPE  eType;               /* Type of routing. Default is pre-loaded
                                       ** route.
                                       */
   CCTKROUTE      route[CCTKCFG_MAX_OBPROXY];
                                       /* Route set. If local policy routing is
                                       ** configured, only the first item of the 
                                       ** route set, i.e. route[0], is used.
                                       ** If user is configured to be secured only,
                                       ** i.e. "bSecOnly" enabled, the first item
                                       ** of the route set must be secured, i.e.
                                       ** "addr.bSec" enabled.
                                       */
} CCTKROUTELST;

/* User network configuration block */
typedef struct CCTKUSRNET
{
   CCTKSIPNAME    sipName;             /* SIP name information */
   BOS_BOOL       bSecOnly;            /* True to indicate this is a secured user.
                                       ** Any inbound/outbound connections MUST be
                                       ** secured by using SIPS via TLS.
                                       ** The certificates/keys in CCTKTLSCFG would
                                       ** be used to establish a secure connection.
                                       ** Not used if CCTK_TRANSPORT_TLS is not set 
                                       ** when configuring transport.
                                       ** False to indicate the user can be both
                                       ** unsecured and secured. The decision is
                                       ** internally made by CCTK based on the
                                       ** received/outgoing packets.
                                       */
   CCTKROUTELST   obProxyLst;          /* An ordered list of proxies where the
                                       ** initial outgoing requests must traverse 
                                       ** through before reaching the remote target.
                                       ** Either the pre-loaded routing or the local
                                       ** policy routing approach can be configured.
                                       ** Default is pre-loaded routing. Set the
                                       ** first address of the route set to 0 if
                                       ** not used.
                                       */
   BOS_UINT32     uDnsId;              /* User identifier to be passed onto the
                                       ** system's DNS lookup functions when CCTK
                                       ** performs a DNS lookup. Set to 0 to use
                                       ** the user GUID as the DNS user identifier.
                                       */
} CCTKUSRNET;

/* User name and password for a realm.
** Specify 0 for default (any) realm. Typically, the default realm should
** be the last item on the list.
*/
typedef struct CCTKRLM
{
   BOS_UINT8      *pcRlm;              /* Authentication realm */
   BOS_UINT8      *pcUsrName;          /* Authentication user name */
   BOS_UINT8      *pcPwd;              /* Authentication password */
} CCTKRLM;

/* Authentication realm list */
typedef struct CCTKRLMLST
{
   BOS_UINT32     uNum;                /* Number of realms of the list below */
   CCTKRLM        rlm[CCTKCFG_MAX_REALM];
                                       /* Realm list */
} CCTKRLMLST;

/* Client authentication information */
typedef struct CCTKAUTHCINFO
{
   BOS_UINT8      *pcAuthName;         /* User name (for client authentication) for
                                       ** REGISTER and by default all challenged
                                       ** calls associated with the user. Set to
                                       ** NULL if same as SIP user name "pcUsrName"
                                       ** above.
                                       */
   BOS_UINT8      *pcPwd;              /* Password (for client authentication) for
                                       ** REGISTER and by default all challenged
                                       ** calls associated with the user.
                                       */
   CCTKRLMLST     rlmlst;              /* List of authentication realms. This field
                                       ** can be used if the user will be challenged
                                       ** when sending requests to other realms.
                                       */
} CCTKAUTHCINFO;

/* User authentication configuration block */
typedef struct CCTKAUTH
{
   CCTKAUTHCINFO  client;              /* Client authentication information */
} CCTKAUTH;

/* NAT keep-alive configuration block */
typedef struct CCTKNATKACFG
{
   CCTKSIPHOST    server;              /* Keep-alive proxy server. In the case when
                                       ** "hostPort.pcAddr" is an FQDN,
                                       ** "hostPort.uPort" can be set to 0 to let
                                       ** DNS determine the destination port.
                                       ** Set "hostPort.pcAddr" to NULL to disable
                                       ** keep-alive.
                                       */
   BOS_UINT16     uSrcPort;            /* Source port used for establishing a keep-
                                       ** alive connection (TLS if "server.bSec" is
                                       ** enabled, or UDP and TCP otherwise).
                                       ** Not used if keep-alive is disabled. Set to
                                       ** 0 to let CCTK choose a port randomly.
                                       ** NOTE: When connection-oriented transport
                                       ** (TCP or TLS) is enabled and the source
                                       ** port is set to the listening port, i.e.
                                       ** "uSipPort" for TCP and "uSipsPort" for
                                       ** TLS, the listening port will be closed for
                                       ** establishing the keep-alive connection.
                                       */
   BOS_UINT32     uTmr;                /* Keep-alive time interval (in sec).
                                       ** This must be set to a non-zero value when
                                       ** keep-alive is enabled.
                                       */
   BOS_UINT32     uFailureThreshold;   /* Keep-alive failure threshold.
                                       ** Tracks the number of times keep-alive
                                       ** messages are sent before a connection
                                       ** is declared failed.
                                       ** Applies only to STUN over UDP
                                       */
   BOS_BOOL       bUsePersistCnx;      /* Set to use persistent connections for
                                       ** keep-alive */

} CCTKNATKACFG;

/* User NAT configuration block */
typedef struct CCTKNAT
{
   CCTKNATKACFG   kaCfg;               /* Keep-alive configuration */
} CCTKNAT;

/* User security configuration block */
typedef struct CCTKUSRSEC
{
   CCTKAUTH       auth;                /* Authentication configuration */
   CCTKNAT        nat;                 /* NAT configuration */
} CCTKUSRSEC;

/* User configuration block */
typedef struct CCTKUSR
{
   CCTKUSRNET     net;                 /* Network configuration */
   CCTKUSRSEC     sec;                 /* Security configuration */
} CCTKUSR;

/* General registration configuration block */
typedef struct CCTKREGGEN
{
   CCTKSIPHOST    registrar;           /* Address of the registrar. This field
                                       ** corresponds to the REGISTER Request-URI
                                       ** and where the packet would be sent to.
                                       ** Set "pcAddr" to NULL if not registering.
                                       ** If user is configured to be secured only,
                                       ** i.e. "bSecOnly" enabled, the address
                                       ** must be secured, i.e. "bSec" enabled.
                                       */
   CCTKHOSTPORT   aor;                 /* Address of record. This field is used as
                                       ** the host-port portion of the Address of
                                       ** Record for the REGISTER and other outgoing
                                       ** requests associated with this user.
                                       ** Set "pcAddr" to NULL if same as registrar.
                                       ** Note to place calls directing to a proxy,
                                       ** the proxy would still need to be filled in
                                       ** through the "pTo" field of
                                       ** cctkCallCreate().
                                       */
} CCTKREGGEN;

/* Registration configuration block */
typedef struct CCTKREG
{
   CCTKREGGEN     gen;                 /* General configuration */
} CCTKREG;

/* SIP user host port address information */
typedef struct CCTKSIPUSRHOST
{
   CCTKSIPHOST    sipHost;             /* SIP host-port address information */
   BOS_UINT8      *pcUsrName;          /* User name of the SIP host-port address.
                                       ** NULL to omit user name in SIP address.
                                       */
} CCTKSIPUSRHOST;

/* General MWI configuration block */
typedef struct CCTKMWIGEN
{
   CCTKSIPUSRHOST server;              /* Address of the MWI server. This field
                                       ** corresponds to the SUBSCRIBE Request-URI
                                       ** and where the packet would be sent to.
                                       ** Set "hostPort.pcAddr" to NULL if not
                                       ** subscribing to an MWI server. If user is
                                       ** configured to be secured only, i.e.
                                       ** "bSecOnly" enabled, the address must be
                                       ** secured, i.e. "hostPort.bSec" enabled.
                                       */
   BOS_BOOL bUnsolNtfy;                /* Set to true in order to enable support for
                                       ** for unsolicited notifications. False otherwise.
                                       */
} CCTKMWIGEN;

/* MWI configuration block */
typedef struct CCTKMWI
{
   CCTKMWIGEN     gen;                 /* General configuration */
} CCTKMWI;

/* General basic call configuration block */
typedef struct CCTKCALLBASGEN
{
   void           *dummy;              /* Add main configuration here */
} CCTKCALLBASGEN;

/* Basic call configuration block */
typedef struct CCTKCALLBAS
{
   CCTKCALLBASGEN gen;                 /* General configuration */
} CCTKCALLBAS;

/* Supplementary call configuration block */
typedef struct CCTKCALLSUPP
{
   CCTKSIPHOST    mohServer;           /* Music-on-hold server, based on
                                       ** draft-ietf-sipping-service-examples-13,
                                       ** 2.3. Set "pcAddr" to NULL if music-on-hold
                                       ** server is unavailable. If user is
                                       ** configured to be secured only,
                                       ** i.e. "bSecOnly" enabled, the address must
                                       ** be secured, i.e. "bSec" enabled.
                                       */
} CCTKCALLSUPP;

/* iLBC mode */
typedef enum CCTKILBCMODE
{
   eCCTKILBCMODE_NOTUSED,              /* Not specified */
   eCCTKILBCMODE_20MS,                 /* Prefer 20ms frames */
   eCCTKILBCMODE_30MS                  /* Prefer 30ms frames */
} CCTKILBCMODE;

/* G.723.1 Annex A (VAD) */
typedef enum CCTKG7231VAD
{
   eCCTKG7231VAD_NOTUSED,               /* Not specified (Implied yes) */
   eCCTKG7231VAD_YES,                   /* annexa=yes */
   eCCTKG7231VAD_NO,                    /* annexa=no */
} CCTKG7231VAD;

/* G.729 Annex B */
typedef enum CCTKG729VAD
{
   eCCTKG729VAD_NOTUSED,               /* Not specified (Implied yes) */
   eCCTKG729VAD_YES,                   /* annexb=yes */
   eCCTKG729VAD_NO,                    /* annexb=no */
} CCTKG729VAD;

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

#ifdef CCTKCFG_RED
typedef struct CCTKREDPARM
{
    BOS_UINT32 uNumCodec;
    CCTKCODEC  eCodec[CCTKCFG_MAX_AUDIO_CODEC];
} CCTKREDPARM;
#endif

#if CCTKCFG_VIDEO
/* H.263 video codec specific attributes. */
typedef struct CCTKH263PARM
{
   /* Boolean indicating whether the parameters are used. */
   BOS_BOOL                   bIsSpecified;

   /* H.263 profile number, in the range 0 through 10, specifying the
   ** supported H.263 annexes/subparts.
   */
   BOS_UINT32                 uProfile;

   /* Level of bitstream operation, in the range 0 through 100, specifying
   ** the level of computational complexity of the decoding process.
   */
   BOS_UINT32                 uLevel;
} CCTKH263PARM;

/* H.264 profile types. All decoders conforming to a specific profile
** have to support all features of that profile.
*/
typedef enum CCTKH264PROFILE
{
   eCCTKH264PROFILE_BASELINE,
   eCCTKH264PROFILE_MAIN,
   eCCTKH264PROFILE_EXTENDED
} CCTKH264PROFILE;

/* Level of H.264 bitstream operation, specifying the upper limits for the
** picture size (in macroblocks), the decoderprocessing rate (in macroblocks
** per second), the size of the multi-picture buffers, the video bit-rate
** and the video buffer size complexity of the decoding process.
*/
typedef enum CCTKH264LEVEL
{
   eCCTKH264LEVEL_1_0,
   eCCTKH264LEVEL_1_1,
   eCCTKH264LEVEL_1_2,
   eCCTKH264LEVEL_1_3,
   eCCTKH264LEVEL_2_0,
   eCCTKH264LEVEL_2_1,
   eCCTKH264LEVEL_2_2,
   eCCTKH264LEVEL_3_0,
   eCCTKH264LEVEL_3_1,
   eCCTKH264LEVEL_3_2,
   eCCTKH264LEVEL_4_0,
   eCCTKH264LEVEL_4_1,
   eCCTKH264LEVEL_4_2,
   eCCTKH264LEVEL_5_0,
   eCCTKH264LEVEL_5_1
} CCTKH264LEVEL;

#define CCTKSDP_MAX_H264_PARM_SETS_LEN   64

/* H.264 RTP stream packetization mode types. */
typedef enum CCTKH264PACKETIZATIONMODE
{
   /* Primarily intended for low-delay applications that are compatible with
   ** systems using ITU-T Recommendation H.241. Fragmented and aggregated
   ** packets not supported.
   */
   eCCTKH264PACKETIZATIONMODE_SINGLE_NAL,

   /* It is primarily intended for low-delay applications. Fragmented and
   ** aggregated packets are supported.
   */
   eCCTKH264PACKETIZATIONMODE_NON_INTERLEAVED,

   /* The interleaved mode is targeted for systems that do not require very
   ** low end-to-end latency. The interleaved mode allows transmission of
   ** NAL units out of NAL unit decoding order.
   */
   eCCTKH264PACKETIZATIONMODE_INTERLEAVED
} CCTKH264PACKETIZATIONMODE;


/* H.264 video codec specific attributes. */
typedef struct CCTKH264PARAMS
{
   /* Boolean indicating whether the parameters are used. */
   BOS_BOOL                   bIsSpecified;

   /* H.264 profile type. All decoders conforming to a specific profile
   ** have to support all features of that profile.
   */
   CCTKH264PROFILE            eProfile;

   /* If set, constraint 0, 1, or 2 of the Baseline profile, the Main profile,
   ** or the Extended profile, respectively, are obeyed in the NAL unit
   ** stream.
   */
   BOS_BOOL                   bConstraintSet0;
   BOS_BOOL                   bConstraintSet1;
   BOS_BOOL                   bConstraintSet2;

   /* Level of bitstream operation, specifying the level of computational
   ** complexity of the decoding process.
   */
   CCTKH264LEVEL              eLevel;

   /* The value of max-mbps is an integer indicating the maximum macroblock
   ** processing rate in units of macroblocks per second.  The max-mbps
   ** parameter signals that the receiver is capable of decoding video at a
   ** higher rate than is required by the signaled level conveyed in the
   ** value of the profile-level-id parameter.
   */
   BOS_UINT32                 uMaxMbps;

   /* The value of max-br is an integer indicating the maximum video bit rate
   ** in units of 1000 bits per second. The max-br parameter signals that
   ** the video decoder of the receiver is capable of decoding video at a
   ** higher bit rate than is required by the signaled level conveyed in
   ** the value of the profile-level-id parameter.
   */
   BOS_UINT32                 uMaxBr;

   /* This parameter signals the properties of an RTP payload type or the
   ** capabilities of a receiver implementation.
   */
   CCTKH264PACKETIZATIONMODE  ePacketizationMode;

   /* This parameter MAY be used to convey any sequence and picture parameter
   ** set NAL units. The value of the parameter is the representation
   ** of the initial parameter set NAL units as specified in sections 7.3.2.1
   ** and 7.3.2.2 of H.264 specification. A comma is used to separate any
   ** pair of parameter sets in the list. The parameter sets should be
   ** represented as hexidecimal. CallCtrl is responsible for converting
   ** the representation to base64.
   */
   BOS_UINT8                  cPropParamSets[CCTKSDP_MAX_H264_PARM_SETS_LEN + 1];

   /* This parameter MAY be used to signal whether the receiver of this
   ** parameter is allowed to add parameter sets in its signaling response
   ** using the sprop-parameter-sets MIME parameter.
   */
   BOS_BOOL                   bParameterAdd;
} CCTKH264PARM;
#endif

/* CCTK firewall filter commands */
#if CMGRCFG_FIREWALL_CONTROL
typedef enum CCTKFWCMD
{
   FW_CLOSE,
   FW_OPEN,
   FW_UNKNOWN
} CCTKFWCMD;
#endif

/* Codec capabilities */
typedef struct CCTKCODECCAP
{
   CCTKCODEC      eType;
   BOS_UINT32     uPtime;              /* Packetization rate, 0 to disable */
   union
   {
      /* per-codec config */
      CCTKILBCMODE   eIlbcMode;        /* iLBC */
      CCTKG7231VAD   eG7231Vad;        /* G.723.1 */
      CCTKG729VAD    eG729Vad;         /* G.729x */
      CCTKAMRPARM    amrCfg;           /* GSM-AMR/AMR-WB */
#if CCTKCFG_RED
      CCTKREDPARM    redCfg;
#endif
#if CCTKCFG_VIDEO
      CCTKH263PARM   *pH263;           /* H.263 */
      CCTKH264PARM   *pH264;           /* H.264 */
#endif
   } parm;
} CCTKCODECCAP;

/* a=silsupp parameter for voice codecs */
typedef enum CCTKSILSUPP
{
   eCCTKSILSUPP_NOTUSED,               /* a=silsupp not used */
   eCCTKSILSUPP_OFF,                   /* a=silsupp off */
   eCCTKSILSUPP_ON                     /* a=silsupp on */
} CCTKSILSUPP;

/* Port range */
typedef struct CCTKPTRANGE
{
   BOS_UINT16     uMinPort;            /* Min port */
   BOS_UINT16     uMaxPort;            /* Max port */
} CCTKPTRANGE;

/* Audio capability configuration and policies - General */
typedef struct CCTKAUDGEN
{
   BOS_UINT32     uNumCodec;           /* Number of audio codecs */
   CCTKCODECCAP   codec[CCTKCFG_MAX_AUDIO_CODEC];
                                       /* Audio codec cap list */
   BOS_BOOL       bTelEvt;             /* Telephone-event */
   BOS_UINT32     uTelEvtTypes;        /* Bitmap for televt types (CCTKNTE_XXX).
                                       ** Set to 0 to not include the a=fmtp line.
                                       */
   BOS_BOOL       bCn;                 /* Comfort Noise (CN) support in m= line */
   CCTKSILSUPP    eSilSupp;            /* Silence suppression support (a=silSupp) */
   BOS_UINT32     uMaxPtime;           /* Max ptime, 0 to disable */
   CCTKPTRANGE    rtpPort;             /* RTP port range */
   CCTKPTRANGE    rtcpPort;            /* RTCP port range
                                       ** (set to 0 to use RTP +1)
                                       */
} CCTKAUDGEN;

/* Audio capability configuration and policies - QoS-related */
typedef struct CCTKAUDQOS
{
   BOS_BOOL       bRtcpXrSupport;      /* RFC3611 RTCP-XR (voip-metrics) support. */
} CCTKAUDQOS;

/* SRTP usage preference */
typedef enum CCTKSRTPPREF
{
   eCCTKSRTPPREF_DISABLED,             /* SRTP disabled */
   eCCTKSRTPPREF_PREFERRED,            /* SRTP preferred to be used */
   eCCTKSRTPPREF_MANDATORY             /* SRTP usage mandatory */
} CCTKSRTPPREF;

#if CCTKCFG_SRTP
/* SRTP crypto suite */
typedef enum CCTKCSUITE
{
   eCCTKCSUITE_AES_CM_128_HMAC_SHA1_80,
   eCCTKCSUITE_AES_CM_128_HMAC_SHA1_32,
   eCCTKCSUITE_F8_128_HMAC_SHA1_80,
   eCCTKCSUITE_MAX
} CCTKCSUITE;

/* SDP Crypto Attribute and parameters
** format: a=crypto:inline <crypto-suite> <key-params> [<session-params>]
*/
#define CCTK_SESPARM_MAX_TOKEN_LEN     31
#define CCTK_SESPARM_MAX_VAL_LEN       31

/* Session parameters [Optional] - used to override SRTP session defaults
 * for the SRTP and SRTP streams
 */
typedef struct CCTKCSESPARAM
{
   /* Session parm token / value */
   BOS_UINT8      cToken[CCTK_SESPARM_MAX_TOKEN_LEN + 1];
   BOS_UINT8      cVal[CCTK_SESPARM_MAX_VAL_LEN + 1];
} CCTKCSESPARAM;

/* SRTP crypto info */
typedef struct CCTKCRYPTO
{
   /* Mandatory parameter(s) */
   CCTKCSUITE     eCryptoSuite;        /* Crypto suite */

   /* Optional parameter(s) */
   BOS_UINT64     uLifeTime;           /* Set val to '0' to use the default */
#if ( CCTKCFG_MAX_CRYPTO_SESPARM > 0 )
   BOS_UINT32     uNumSessionParm;     /* Number of sessions parameters */
   CCTKCSESPARAM  session[CCTKCFG_MAX_CRYPTO_SESPARM];
                                       /* Session parameters */
#endif
} CCTKCRYPTO;

/* List of supported crypto */
typedef struct CCTKCRYPTOLST
{
   BOS_UINT32     uNumCrypto;          /* Number of crypto */
   CCTKCRYPTO     crypto[CCTKCFG_MAX_CRYPTO];
                                       /* Crypto information */
} CCTKCRYPTOLST;
#endif

/* Audio capability configuration and policies - Security-related */
typedef struct CCTKAUDSEC
{
   CCTKSRTPPREF   eSrtpPref;           /* SRTP usage preference */
#if CCTKCFG_SRTP
   CCTKCRYPTOLST  cryptoLst;           /* Crypto list */
#endif
} CCTKAUDSEC;

/* Audio capability configuration and policies */
typedef struct CCTKMEDAUD
{
   CCTKAUDGEN     gen;                 /* General */
   CCTKAUDQOS     qos;                 /* QoS-related */
   CCTKAUDSEC     sec;                 /* Security-related */
} CCTKMEDAUD;

/* Fax capability configuration and policies */
typedef struct CCTKMEDFAX
{
   CCTKPTRANGE    t38Port;             /* T38 port range */
   BOS_UINT32     uMaxDatagram;        /* Max T38 datagram size */
   BOS_UINT32     uPtime;              /* Packetization rate, 0 to disable */
} CCTKMEDFAX;

#if CCTKCFG_VIDEO
/* Video capability configuration and policies */
typedef struct CCTKVIDGEN
{
   BOS_UINT32     uNumCodec;           /* Number of video codecs */
   CCTKCODECCAP   codec[CCTKCFG_MAX_VIDEO_CODEC];
                                       /* Video codec cap list */
   CCTKPTRANGE    rtpPort;             /* RTP port range */
   CCTKPTRANGE    rtcpPort;            /* RTCP port range
                                       ** (set to 0 to use RTP +1)
                                       */
} CCTKVIDGEN;

/* Video capability configuration and policies - Security-related */
typedef struct CCTKVIDSEC
{
   CCTKSRTPPREF   eSrtpPref;           /* SRTP usage preference */
#if CCTKCFG_SRTP
   CCTKCRYPTOLST  cryptoLst;           /* Crypto list */
#endif
} CCTKVIDSEC;

/* Video capability configuration and policies */
typedef struct CCTKMEDVID
{
   CCTKVIDGEN     gen;                 /* General */
   CCTKVIDSEC     sec;                 /* Security-related */
} CCTKMEDVID;
#endif

/* Used in mediaTypes under CCTKMEDLST */
#define CCTK_MEDIA_AUDIO_MASK 0x00000001
#define CCTK_MEDIA_T38_MASK   0x00000002
#define CCTK_MEDIA_VIDEO_MASK 0x00000004

/* Media list */
typedef struct CCTKMEDLST
{
   BOS_UINT32     mMediaTypes;         /* Bitmap of the media type used */
   BOS_UINT32     uNumAudCodec;        /* Number of audio codecs */
   CCTKCODEC      audCodeLst[CCTKCFG_MAX_AUDIO_CODEC];
                                       /* Audio codec list */
#if CCTKCFG_VIDEO
   BOS_UINT32     uNumVidCodec;        /* Number of video codecs */
   CCTKCODEC      vidCodeLst[CCTKCFG_MAX_VIDEO_CODEC];
                                       /* Video codec list */
#endif
} CCTKMEDLST;

/* Media capability configuration and policies */
typedef struct CCTKMEDCFG
{
   BOS_IP_ADDRESS ipaddr;              /* Media IP address */
   BOS_UINT8      *pcSesName;          /* Controls the s= content.  By default (when
                                       ** set to NULL), the s= line is set to "-"
                                       ** as per RFC 3264.
                                       */
   CCTKMEDAUD     *pAudio;             /* Audio cap configuration and policies,
                                       ** set to NULL if not available.
                                       */
#if CCTKCFG_VIDEO
   CCTKMEDVID     *pVideo;             /* Video cap configuration and policies,
                                       ** set to NULL if not available.
                                       */
#endif
   CCTKMEDFAX     *pFax;               /* Fax cap configuration and policies,
                                       ** set to NULL if not available.
                                       */
   CCTKMEDLST     *pMediaLst;          /* Offer media list, if different from the
                                       ** configured capabilities.  The list is
                                       ** used when the user would like a subset of
                                       ** the caps configured, for example, using
                                       ** only audio when both audio and T.38 are
                                       ** supported.
                                       */
   BOS_SINT32     sNtpTimeAdj;         /* the time that needs to be adjusted from 
                                       ** the current time of day time to get the 
                                       ** NTP time
                                       */
} CCTKMEDCFG;

/* CCTK call configuration block */
typedef struct CCTKCALLCFG
{
   CCTKCALLBAS    basic;               /* Basic call configuration */
   CCTKCALLSUPP   supp;                /* Supplementary call configuration */
   CCTKMEDCFG     media;               /* Media configuration */
} CCTKCALLCFG;

/* General event subscription configuration block */
typedef struct CCTKEVTGEN
{
   void           *dummy;              /* Add main config here */
} CCTKEVTGEN;

/* CCTK event subscription configuration block */
typedef struct CCTKEVTCFG
{
   CCTKEVTGEN     gen;                 /* General configuration */
} CCTKEVTCFG;

/* Default configuration */
typedef struct CCTKDFT
{
   CCTKCALLCFG    call;                /* Call configuration */
   CCTKEVTCFG     evt;                 /* Event subscription configuration */
} CCTKDFT;

#if CCTKCFG_REMOTE_USER_CONTROL   
/* Default configuration */
typedef struct CCTKREMUSR
{
   BOS_BOOL        bRemUsr;           /* flag indicating if this is a remote user */ 
} CCTKREMUSR;
#endif

/* CCTK user configuration block */
typedef struct CCTKUSRCFG
{
   CCTKUSR        usr;                 /* User configuration */
   CCTKREG        reg;                 /* Registration configuration */
   CCTKMWI        mwi;                 /* MWI configuration */
   CCTKDFT        dft;                 /* Default configurations */
#if CCTKCFG_REMOTE_USER_CONTROL    
   CCTKREMUSR     remUsr;
#endif
} CCTKUSRCFG;

/* Accessible network, IP type MUST match network address */
typedef struct CCTKACCESSNET
{
   BOS_IP_ADDRESS addr;                /* Network address */
   BOS_IP_ADDRESS mask;                /* Network mark */
} CCTKACCESSNET;

/* Network accessibility information */
typedef struct CCTKACCESSINFO
{
   BOS_UINT32     uNum;                /* Number of accessible networks */
   CCTKACCESSNET  net[CCTKCFG_MAX_ACCESSNET];
                                       /* List of accessible networks */
} CCTKACCESSINFO;

/* Host address */
typedef struct CCTKHOSTADDR
{
   BOS_IP_ADDRESS ipAddr;              /* Host IP address */
#if CCTKCFG_IPV6
   BOS_UINT32     uScopeId;            /* Optional IPv6 scope ID.  Used for IPv6
                                       ** only, when the OS requires the scope
                                       ** ID to be set.
                                       */
#endif
   BOS_UINT8      *pcFqdn;             /* FQDN of host address, NULL if none */
   CCTKACCESSINFO access;              /* Network accessibility information */
} CCTKHOSTADDR;

/* Host address information */
typedef struct CCTKHOSTINFO
{
   BOS_UINT32     uNum;                /* Number of host addresses */
   CCTKHOSTADDR   addr[CCTKCFG_MAX_HOSTADDR];
                                       /* Host address(es) */
} CCTKHOSTINFO;

/* General network configuration block */
typedef struct CCTKNETGEN
{
   CCTKHOSTINFO   host;                /* Host address information */
} CCTKNETGEN;

/* Emergency service address information */
typedef struct CCTKEMGADDR
{
   BOS_UINT8 *pcHouseNum;              /* House number. NULL if unavailable. */
   BOS_UINT8 *pcStreet;                /* Group of streets below neighborhood level.
                                       ** NULL if unavailable.
                                       */
   BOS_UINT8 *pcCity;                  /* City, township. NULL if unavailable. */
   BOS_UINT8 *pcSubDiv;                /* National subdivision (state, province,
                                       ** region). NULL if unavailable.
                                       */
   BOS_UINT8 *pcCountry;               /* Country - 2 chars, CA, US, etc.
                                       ** NULL if unavailable.
                                       */
   BOS_UINT8 *pcPostal;                /* Postal code. NULL if unavailable. */
   BOS_UINT8 *pcLocInfo;               /* Additional location information.
                                       ** NULL if unavailable.
                                       */
   BOS_UINT8 *pcTimeStamp;             /* Civic time-stamp. NULL if unavailable. */
   
   BOS_UINT8 *pcPos;                   /* pos element - contains lat/long/altitude coordinates*/
   BOS_UINT8 *pcSrsName;               /* srsName - type of positioning system used*/
   BOS_UINT8 *pcFLR;                   /* FLR for civic address - if special altiltude type was used.
                                       ** NULL if unavailable*/
   BOS_UINT8 *pcGeoTimeStamp;          /* Geo timestamp*/
   
   BOS_BOOL bUseGeoLoc;   /*If true use geo location, otherwise use civic address*/
   
} CCTKEMGADDR;

/* General network protocol configuration block */
typedef struct CCTKNETPROTGEN
{
   CCTKEMGADDR    emgAddr;             /* Emergency service address information */
} CCTKNETPROTGEN;

/* Network protocol configuration block */
typedef struct CCTKNETPROT
{
   CCTKNETPROTGEN gen;                 /* General network protocol configuration */
} CCTKNETPROT;

/* Network configuration block */
typedef struct CCTKNETCFG
{
   CCTKNETGEN     gen;                 /* General configuration */
   CCTKNETPROT    prot;                /* Protocol configuration */
} CCTKNETCFG;

/* General local configuration block */
typedef struct CCTKLOCGEN
{
   BOS_UINT8      *pcSeed;             /* Random seed for tag/ID generation */
} CCTKLOCGEN;

/* Local configuration block */
typedef struct CCTKLOCCFG
{
   CCTKLOCGEN     gen;                 /* General configuration */
} CCTKLOCCFG;

/* CCTK system configuration block */
typedef struct CCTKCFG
{
   CCTKNETCFG     net;                 /* Network configuration */
   CCTKLOCCFG     loc;                 /* Local configuration */
} CCTKCFG;


/* ---- Variable Externs -------------------------------------------------------- */
/* ---- Function Prototypes ----------------------------------------------------- */

#endif /* __CCTK_MAIN_CFG_API__H__INCLUDED__ */
