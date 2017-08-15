/****************************************************************************
*
*  HAUSWARE Definitions for NETVHD
*
*  PURPOSE:
*     This file contains the API definitions for the NETVHD
*
*           (c) Copyright 1995-1998, HotHaus Technologies Inc.
*                          All rights reserved
*
****************************************************************************/

#ifndef HAPI_NET_H                        /* support nested includes */
#define HAPI_NET_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>                   /* Hausware environment defines */
#endif
#ifndef HAPI_PKT_H
#include <hapi_pkt.h>
#endif
#ifndef HAPI_WRAP_DEFS_H
#include <hapi_wrap_defs.h>
#endif
#ifndef PAYLOAD_H
#include <payload.h>                   /* NetVHD Packet Payload Info */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                   /* NetVHD Packet Payload Info */
#endif
#ifndef HAPI_PVE_H
#include <hapi_pve.h>                     /* PVE Service defines */
#endif
#ifndef HAPI_CODEC_H
#include <hapi_codec.h>
#endif
#ifndef HAPI_CDIS_H
#include <hapi_cdis.h>
#endif
#ifndef HAPI_PTE_H
#include <hapi_pte.h>
#endif

#define HAPINETSRV              HAPI_NET_SRV

/*************************************************************************
**                         NETVHD COMMANDS
*************************************************************************/
#define HAPI_NET_EGRESSPACKET_CMD     (HSF_MKCMD_EXT(HAPINETSRV, 1)) /* incoming network packet */
#define HAPI_NET_CONFIG_SETREGS_CMD   (HSF_MKCMD_EXT(HAPINETSRV, 2))  /* set configuration registers */
#define HAPI_NET_CONFIG_GETREGS_CMD   (HSF_MKCMD_EXT(HAPINETSRV, 3))  /* get configuration registers */
#define HAPI_NET_CONFIG_RESETREGS_CMD (HSF_MKCMD_STD(HAPINETSRV, 4))  /* reset configuration registers */
#define HAPI_NET_SETMODE_CMD          (HSF_MKCMD_STD(HAPINETSRV, 5)) /* set mode */
#define HAPI_NET_GETMODE_CMD          (HSF_MKCMD_EXT(HAPINETSRV, 6)) /* get mode */
#define HAPI_NET_SETVOICERATE_CMD     (HSF_MKCMD_STD(HAPINETSRV, 8)) /* set voice ingress rate */
#define HAPI_NET_SETFAXRATE_CMD       (HSF_MKCMD_STD(HAPINETSRV, 10)) /* set fax ingress rate */
#define HAPI_NET_GETVOICESTATE_CMD    (HSF_MKCMD_EXT(HAPINETSRV, 13)) /* get net statistics */
#define HAPI_NET_GETVOICESTATS_CMD    (HSF_MKCMD_EXT(HAPINETSRV, 14)) /* get net statistics */
#define HAPI_NET_TONE_SETREGS_CMD     (HSF_MKCMD_EXT(HAPINETSRV, 15)) /* set tone registers */
#define HAPI_NET_TONE_GETREGS_CMD     (HSF_MKCMD_EXT(HAPINETSRV, 16)) /* get tone registers */
#define HAPI_NET_VOICE_SETREGS_CMD    (HSF_MKCMD_EXT(HAPINETSRV, 17)) /* set voice registers */
#define HAPI_NET_VOICE_GETREGS_CMD    (HSF_MKCMD_EXT(HAPINETSRV, 18)) /* get voice registers */
#define HAPI_NET_GETFAXSTATS_CMD      (HSF_MKCMD_EXT(HAPINETSRV, 19)) /* get fax relay statistics */
#define HAPI_NET_GETPDESTATS_CMD      (HSF_MKCMD_EXT(HAPINETSRV, 20)) /* get data relay statistics */
#define HAPI_NET_FAXR_SETREGS_CMD     (HSF_MKCMD_EXT(HAPINETSRV, 21)) /* set fax relay registers */
#define HAPI_NET_FAXR_GETREGS_CMD     (HSF_MKCMD_EXT(HAPINETSRV, 22)) /* get fax relay registers */
#define HAPI_NET_PDE_SETREGS_CMD      (HSF_MKCMD_EXT(HAPINETSRV, 23)) /* set data relay registers */
#define HAPI_NET_PDE_GETREGS_CMD      (HSF_MKCMD_EXT(HAPINETSRV, 24)) /* get data relay registers */
#define HAPI_NET_CONFIG_SETREG1_CMD   (HSF_MKCMD_STD(HAPINETSRV, 25)) /* get data relay registers */
#define HAPI_NET_TONE_SETREG1_CMD     (HSF_MKCMD_STD(HAPINETSRV, 26)) /* get data relay registers */
#define HAPI_NET_VOICE_SETREG1_CMD    (HSF_MKCMD_STD(HAPINETSRV, 27)) /* get data relay registers */
#define HAPI_NET_FAXR_SETREG1_CMD     (HSF_MKCMD_STD(HAPINETSRV, 28)) /* get data relay registers */
#define HAPI_NET_PDE_SETREG1_CMD      (HSF_MKCMD_STD(HAPINETSRV, 29)) /* get data relay registers */
#define HAPI_NET_PTE_SETREGS_CMD      (HSF_MKCMD_EXT(HAPINETSRV, 30)) /* set data relay registers */
#define HAPI_NET_PTE_SETREG1_CMD      (HSF_MKCMD_STD(HAPINETSRV, 31)) /* set data relay registers */
#define HAPI_NET_PTE_GETREGS_CMD      (HSF_MKCMD_EXT(HAPINETSRV, 32)) /* get data relay registers */
#define HAPI_NET_EGRESSRTPPKT_CMD     (HSF_MKCMD_EXT(HAPINETSRV, 33)) /* incoming RTP packet */
#define HAPI_NET_EGRESSRTCPPKT_CMD    (HSF_MKCMD_EXT(HAPINETSRV, 34)) /* incoming RTCP packet */
#define HAPI_NET_RTP_SETREGS_CMD      (HSF_MKCMD_EXT(HAPINETSRV, 35)) /* set RTP registers */
#define HAPI_NET_RTP_SETREG1_CMD      (HSF_MKCMD_STD(HAPINETSRV, 36)) /* set RTP registers */
#define HAPI_NET_RTP_SETREGS2_CMD     (HSF_MKCMD_EXT(HAPINETSRV, 37)) /* set RTP registers */
#define HAPI_NET_RTP_GETREGS_CMD      (HSF_MKCMD_EXT(HAPINETSRV, 38)) /* get RTP registers */
#define HAPI_NET_RTP_GETREGS2_CMD     (HSF_MKCMD_EXT(HAPINETSRV, 39)) /* get RTP registers */
#define HAPI_NET_T38_SETREGS_CMD      (HSF_MKCMD_EXT(HAPINETSRV, 40))  /* set T38 registers */
#define HAPI_NET_T38_SETREG1_CMD      (HSF_MKCMD_STD(HAPINETSRV, 41))  /* set T38 registers */
#define HAPI_NET_T38_GETREGS_CMD      (HSF_MKCMD_EXT(HAPINETSRV, 42))  /* get T38 registers */

#define HAPI_NET_UPDATE_HEARTBEAT_TIMER_CMD (HSF_MKCMD_STD(HAPINETSRV, 43))  /* */
#define HAPI_NET_UPDATE_STATS_TIMER_CMD     (HSF_MKCMD_STD(HAPINETSRV, 44))  /* */

#define HAPI_NET_VBD_AUTOSWITCH_MASK     (HSF_MKCMD_STD(HAPINETSRV, 45))  /* */
      /* OP1: mask (HAPI_NET_AUTOSWITCH_MASK_XXXX) */
      /* OP2: not used */


/* RESPONSES */

#define HAPI_NET_EGRESSPACKET_RSP     (HSF_MKRSP_EXT(HAPI_NET_EGRESSPACKET_CMD))
#define HAPI_NET_CONFIG_SETREGS_RSP   (HSF_MKRSP_EXT(HAPI_NET_CONFIG_SETREGS_CMD))
#define HAPI_NET_CONFIG_GETREGS_RSP   (HSF_MKRSP_EXT(HAPI_NET_CONFIG_GETREGS_CMD))
#define HAPI_NET_CONFIG_RESETREGS_RSP (HSF_MKRSP_EXT(HAPI_NET_CONFIG_RESETREGS_CMD))
#define HAPI_NET_SETMODE_RSP          (HSF_MKRSP_STD(HAPI_NET_SETMODE_CMD))
#define HAPI_NET_GETMODE_RSP          (HSF_MKRSP_EXT(HAPI_NET_GETMODE_CMD))
#define HAPI_NET_SETVOICERATE_RSP     (HSF_MKRSP_EXT(HAPI_NET_SETVOICERATE_CMD))
#define HAPI_NET_SETFAXRATE_RSP       (HSF_MKRSP_EXT(HAPI_NET_SETFAXRATE_CMD))
#define HAPI_NET_GETVOICESTATE_RSP    (HSF_MKRSP_EXT(HAPI_NET_GETVOICESTATE_CMD))
#define HAPI_NET_GETVOICESTATS_RSP    (HSF_MKRSP_EXT(HAPI_NET_GETVOICESTATS_CMD))
#define HAPI_NET_TONE_SETREGS_RSP     (HSF_MKRSP_EXT(HAPI_NET_TONE_SETREGS_CMD))
#define HAPI_NET_TONE_GETREGS_RSP     (HSF_MKRSP_EXT(HAPI_NET_TONE_GETREGS_CMD))
#define HAPI_NET_VOICE_SETREGS_RSP    (HSF_MKRSP_EXT(HAPI_NET_VOICE_SETREGS_CMD))
#define HAPI_NET_VOICE_GETREGS_RSP    (HSF_MKRSP_EXT(HAPI_NET_VOICE_GETREGS_CMD))
#define HAPI_NET_GETFAXSTATS_RSP      (HSF_MKRSP_EXT(HAPI_NET_GETFAXSTATS_CMD))
#define HAPI_NET_GETPDESTATS_RSP      (HSF_MKRSP_EXT(HAPI_NET_GETPDESTATS_CMD))
#define HAPI_NET_FAXR_SETREGS_RSP     (HSF_MKRSP_EXT(HAPI_NET_FAXR_SETREGS_CMD))
#define HAPI_NET_FAXR_GETREGS_RSP     (HSF_MKRSP_EXT(HAPI_NET_FAXR_GETREGS_CMD))
#define HAPI_NET_PDE_SETREGS_RSP      (HSF_MKRSP_EXT(HAPI_NET_PDE_SETREGS_CMD))
#define HAPI_NET_PDE_GETREGS_RSP      (HSF_MKRSP_EXT(HAPI_NET_PDE_GETREGS_CMD))
#define HAPI_NET_CONFIG_SETREG1_RSP   (HSF_MKRSP_EXT(HAPI_NET_CONFIG_SETREG1_CMD))
#define HAPI_NET_TONE_SETREG1_RSP     (HSF_MKRSP_EXT(HAPI_NET_TONE_SETREG1_CMD))
#define HAPI_NET_VOICE_SETREG1_RSP    (HSF_MKRSP_EXT(HAPI_NET_VOICE_SETREG1_CMD))
#define HAPI_NET_FAXR_SETREG1_RSP     (HSF_MKRSP_EXT(HAPI_NET_FAXR_SETREG1_CMD))
#define HAPI_NET_PDE_SETREG1_RSP      (HSF_MKRSP_EXT(HAPI_NET_PDE_SETREG1_CMD))
#define HAPI_NET_PTE_SETREGS_RSP      (HSF_MKRSP_EXT(HAPI_NET_PTE_SETREGS_CMD))
#define HAPI_NET_PTE_SETREG1_RSP      (HSF_MKRSP_EXT(HAPI_NET_PTE_SETREG1_CMD))
#define HAPI_NET_PTE_GETREGS_RSP      (HSF_MKRSP_EXT(HAPI_NET_PTE_GETREGS_CMD))
#define HAPI_NET_T38_SETREGS_RSP      (HSF_MKRSP_EXT(HAPI_NET_T38_SETREGS_CMD))
#define HAPI_NET_T38_SETREG1_RSP      (HSF_MKRSP_EXT(HAPI_NET_T38_SETREG1_CMD))
#define HAPI_NET_T38_GETREGS_RSP      (HSF_MKRSP_EXT(HAPI_NET_T38_GETREGS_CMD))
#define HAPI_NET_RTP_GETREGS_RSP      (HSF_MKRSP_EXT(HAPI_NET_RTP_GETREGS_CMD))
#define HAPI_NET_RTP_GETREGS2_RSP     (HSF_MKRSP_EXT(HAPI_NET_RTP_GETREGS2_CMD))

#define HAPI_NET_UPDATE_HEARTBEAT_TIMER_RSP (HSF_MKRSP_STD(HAPI_NET_UPDATE_HEARTBEAT_TIMER_CMD))  /* */
#define HAPI_NET_UPDATE_STATS_TIMER_RSP  (HSF_MKRSP_STD(HAPI_NET_UPDATE_STATS_TIMER_CMD))  /* */

/*************************************************************************
**                   NETVHD ASYNCHRONOUS EVENTS
*************************************************************************/
#define HAPI_NET_HEARTBEAT_EVT        (HSF_MKEVT_STD(HAPINETSRV, 0))  /* interval event */
#define HAPI_NET_INGRESSPACKET_EVT    (HSF_MKEVT_EXT(HAPINETSRV, 1))  /* outgoing network packet */
#define HAPI_NET_MODE_EVT             (HSF_MKEVT_STD(HAPINETSRV, 2))  /* mode change event */
#define HAPI_NET_VOICERATE_EVT        (HSF_MKEVT_STD(HAPINETSRV, 3))  /* voice rate change event */
#define HAPI_NET_FAXRATE_EVT          (HSF_MKEVT_STD(HAPINETSRV, 4))  /* fax rate change event */
#define HAPI_NET_JITTER_EVT           (HSF_MKEVT_STD(HAPINETSRV, 5))  /* jitter buffer event */
#define HAPI_NET_EXCEPTION_EVT        (HSF_MKEVT_STD(HAPINETSRV, 6))  /* exception event */
#define HAPI_NET_VOICESTATS_EVT       (HSF_MKEVT_EXT(HAPINETSRV, 7))  /* statistics event for voice */
#define HAPI_NET_FAXSTATS_EVT         (HSF_MKEVT_EXT(HAPINETSRV, 8))  /* statistics event for fax relay */
#define HAPI_NET_PDESTATS_EVT         (HSF_MKEVT_EXT(HAPINETSRV, 9))  /* statistics event far data relay */
#define HAPI_NET_VOICESTATE_EVT       (HSF_MKEVT_EXT(HAPINETSRV, 10)) /* voice state event */
#define HAPI_NET_INGRESSRTPPKT_EVT    (HSF_MKEVT_EXT(HAPINETSRV, 11)) /* outgoing RTP network packet */
#define HAPI_NET_INGRESSRTCPPKT_EVT   (HSF_MKEVT_EXT(HAPINETSRV, 12)) /* outgoing RTCP network packet */

/* legacy names */
#define HAPINET_EGRESSPACKET     HAPI_NET_EGRESSPACKET_CMD
#define HAPINET_CONFIG_SETREGS   HAPI_NET_CONFIG_SETREGS_CMD
#define HAPINET_CONFIG_GETREGS   HAPI_NET_CONFIG_GETREGS_CMD
#define HAPINET_CONFIG_RESETREGS HAPI_NET_CONFIG_RESETREGS_CMD
#define HAPINET_SETMODE          HAPI_NET_SETMODE_CMD
#define HAPINET_GETMODE          HAPI_NET_GETMODE_CMD
#define HAPINET_SETVOICERATE     HAPI_NET_SETVOICERATE_CMD
#define HAPINET_SETFAXRATE       HAPI_NET_SETFAXRATE_CMD
#define HAPINET_GETVOICESTATE    HAPI_NET_GETVOICESTATE_CMD
#define HAPINET_GETVOICESTATS    HAPI_NET_GETVOICESTATS_CMD
#define HAPINET_TONE_SETREGS     HAPI_NET_TONE_SETREGS_CMD
#define HAPINET_TONE_GETREGS     HAPI_NET_TONE_GETREGS_CMD
#define HAPINET_VOICE_SETREGS    HAPI_NET_VOICE_SETREGS_CMD
#define HAPINET_VOICE_GETREGS    HAPI_NET_VOICE_GETREGS_CMD
#define HAPINET_GETFAXSTATS      HAPI_NET_GETFAXSTATS_CMD
#define HAPINET_GETPDESTATS      HAPI_NET_GETPDESTATS_CMD
#define HAPINET_FAXR_SETREGS     HAPI_NET_FAXR_SETREGS_CMD
#define HAPINET_FAXR_GETREGS     HAPI_NET_FAXR_GETREGS_CMD
#define HAPINET_T38_SETREGS      HAPI_NET_T38_SETREGS_CMD
#define HAPINET_T38_GETREGS      HAPI_NET_T38_GETREGS_CMD
#define HAPINET_PDE_SETREGS      HAPI_NET_PDE_SETREGS_CMD
#define HAPINET_PDE_GETREGS      HAPI_NET_PDE_GETREGS_CMD
#define HAPINET_CONFIG_SETREG1   HAPI_NET_CONFIG_SETREG1_CMD
#define HAPINET_TONE_SETREG1     HAPI_NET_TONE_SETREG1_CMD
#define HAPINET_VOICE_SETREG1    HAPI_NET_VOICE_SETREG1_CMD
#define HAPINET_FAXR_SETREG1     HAPI_NET_FAXR_SETREG1_CMD
#define HAPINET_T38_SETREG1      HAPI_NET_T38_SETREG1_CMD
#define HAPINET_PDE_SETREG1      HAPI_NET_PDE_SETREG1_CMD
#define HAPINET_PTE_SETREGS      HAPI_NET_PTE_SETREGS_CMD
#define HAPINET_PTE_SETREG1      HAPI_NET_PTE_SETREG1_CMD
#define HAPINET_PTE_GETREGS      HAPI_NET_PTE_GETREGS_CMD
#define HAPINET_EGRESSRTPPKT     HAPI_NET_EGRESSRTPPKT_CMD
#define HAPINET_EGRESSRTCPPKT    HAPI_NET_EGRESSRTCPPKT_CMD
#define HAPINET_RTP_SETREGS      HAPI_NET_RTP_SETREGS_CMD
#define HAPINET_RTP_SETREG1      HAPI_NET_RTP_SETREG1_CMD
#define HAPINET_RTP_SETREGS2     HAPI_NET_RTP_SETREGS2_CMD
#define HAPINET_RTP_GETREGS      HAPI_NET_RTP_GETREGS_CMD
#define HAPINET_RTP_GETREGS2     HAPI_NET_RTP_GETREGS2_CMD


#define HAPINET_HEARTBEAT        HAPI_NET_HEARTBEAT_EVT
#define HAPINET_INGRESSPACKET    HAPI_NET_INGRESSPACKET_EVT
#define HAPINET_MODE             HAPI_NET_MODE_EVT
#define HAPINET_VOICERATE        HAPI_NET_VOICERATE_EVT
#define HAPINET_FAXRATE          HAPI_NET_FAXRATE_EVT
#define HAPINET_JITTER           HAPI_NET_JITTER_EVT
#define HAPINET_EXCEPTION        HAPI_NET_EXCEPTION_EVT
#define HAPINET_VOICESTATS       HAPI_NET_VOICESTATS_EVT
#define HAPINET_FAXSTATS         HAPI_NET_FAXSTATS_EVT
#define HAPINET_PDESTATS         HAPI_NET_PDESTATS_EVT
#define HAPINET_VOICESTATE       HAPI_NET_VOICESTATE_EVT
#define HAPINET_INGRESSRTPPKT    HAPI_NET_INGRESSRTPPKT_EVT
#define HAPINET_INGRESSRTCPPKT   HAPI_NET_INGRESSRTCPPKT_EVT

#define HAPINET_GETVOICESTATS_RSP  HAPI_NET_GETVOICESTATS_RSP
#define HAPINET_GETFAXSTATS_RSP    HAPI_NET_GETFAXSTATS_RSP


#define HAPINET_GETPDESTATS_RSP    HAPI_NET_GETPDESTATS_RSP
#define HAPINET_VOICE_GETREGS_RSP  HAPI_NET_VOICE_GETREGS_RSP
#define HAPINET_CONFIG_GETREGS_RSP HAPI_NET_CONFIG_GETREGS_RSP
#define HAPINET_TONE_GETREGS_RSP   HAPI_NET_TONE_GETREGS_RSP
#define HAPINET_FAXR_GETREGS_RSP   HAPI_NET_FAXR_GETREGS_RSP
#define HAPINET_T38_GETREGS_RSP    HAPI_NET_T38_GETREGS_RSP
#define HAPINET_PDE_GETREGS_RSP    HAPI_NET_PDE_GETREGS_RSP
#define HAPINET_PTE_GETREGS_RSP    HAPI_NET_PTE_GETREGS_RSP
#define HAPINET_GETVOICESTATE_RSP  HAPI_NET_GETVOICESTATE_RSP
#define HAPINET_RTP_GETREGS_RSP    HAPI_NET_RTP_GETREGS_RSP
#define HAPINET_RTP_GETREGS2_RSP   HAPI_NET_RTP_GETREGS2_RSP


#define HAPI_NET_AUTOSWITCH_MASK_V18_ASCII      0x01
#define HAPI_NET_AUTOSWITCH_MASK_V18_BAUDOT     0x02
#define HAPI_NET_AUTOSWITCH_MASK_V18_PREAMBLE   0x04

/* NETVHD Modes */

typedef enum
{
   NETMODE_IDLE,                       /* Idle, On hook */
   NETMODE_CALLSETUP,                  /* Call setup */
   NETMODE_PACKETVOICE,                /* Packet voice communication */
   NETMODE_DATARELAY,                  /* Data relay communication */
   NETMODE_FAXRELAY,                   /* Fax relay communication */
   NETMODE_VOICEBANDDATA,              /* Voiceband data communication */
   NETMODE_MAX                         /* Max modes */
} HSENETMODE;

/* NetVHD Exception Events */

typedef enum
{
   NETEXCEPTION_UNKNOWN = 0x0000,
   NETEXCEPTION_COMMAND,
   NETEXCEPTION_MEMORY,
   NETEXCEPTION_INVALID_PXD,
   NETEXCEPTION_INVALID_SETREGS,
   NETEXCEPTION_BAD_TIMESTAMP,
   NETEXCEPTION_ALG_UNSUPPORTED = 0x0010,
   NETEXCEPTION_ALG_INCOMPATIBLE,
   NETEXCEPTION_CLASSRELAY_B202ERROR,
   NETEXCEPTION_CLASSRELAY_RXMSGERROR,
   NETEXCEPTION_CLASSRELAY_RXMSGBUFERROR
} HSENETEXCEPTION;

/* Jitter Events */

typedef enum
{
   JITTER_UNDERRUN = 0,
   JITTER_OVERRUN,
   JITTER_HIGHWATER,
   JITTER_LOWWATER,
   JITTER_RESET
}  HSEJITTERTYPES;

/*************************************************************************
**                   NETVHD STATISTICS
*************************************************************************/

typedef struct
{
   HAPI_UINT16   hsxHapiMIB;                /* ? */
   HAPI_UINT16   hsxUpdateInterval;         /* Stats update interval (ms) */
   HAPI_UINT16   hsxReceiveFrameCount;      /* Frames recieved */
   HAPI_UINT16   hsxTransmitFrameCount;     /* Frames transmitted */
} HSZNETVHDSTATS;

typedef struct
{
   HAPI_UINT16   hsxTransmitToneCount;
   HAPI_UINT16   hsxReceiveToneCount;
   HAPI_UINT16   hsxReserved;
} HSZNETVHDMISCSTATS;

typedef struct
{
   //HSZNETVHDSTATS hsxVHDstats;         /* statistics for NetVHD */
   HAPI_PVE_STATS hsxPVEstats;         /* statistics for PVE encoder and decoder */
   //HSZNETVHDMISCSTATS   hsxMISstats;   /* miscellaneous statistics (for PTE) */

} HSZNETVOICESTATS;

/*************************************************************************
**                   NETVHD REGISTERS AND DEFINES
*************************************************************************/

/* Call Setup & packet voice options */

#define HSNET_PTE_OPTION_DETECTOR_INGRESSMASK       HAPI_PTE_INGRESSMASK /* Ingress digit Detector mask (HAPI_PTE_TONE_TYPE) */
#define HSNET_PTE_OPTION_DETECTOR_EGRESSMASK        HAPI_PTE_EGRESSMASK /* Egress digit Detector mask (HAPI_PTE_TONE_TYPE) */
#define HSNET_PTE_OPTION_MODE_DETECTOR_MASK         HAPI_PTE_MODE_DETECTOR_MASK /* Digit Detector mask (HAPI_PTE_TONE_TYPE) */
#define HSNET_PTE_OPTION_DIG_RELAY                  HAPI_PTE_MODE_DIG_RELAY /* Enable digit (DTMF/MF) relay */
#define HSNET_PTE_OPTION_DIG_EVT                    HAPI_PTE_MODE_DIG_EVT   /* Enable digit (DTMF/MF) detection & events */
#define HSNET_PTE_OPTION_MODE_DR_EG_EVT             HAPI_PTE_MODE_DR_EG_EVT /* Enable digit (DTMF/MF) relay egress  events */
#define HSNET_PTE_OPTION_MODE_SQUELCH               HAPI_PTE_MODE_SQUELCH   /* Squelch ingress digits after detection */
#define HSNET_PTE_OPTION_MODE_G711_UPSPEED          HAPI_PTE_MODE_G711_UPSPEED   /* Use G711 upspeed for relay */
#define HSNET_PTE_OPTION_MODE_G711_UPSPEED_LAW      HAPI_PTE_MODE_G711_UPSPEED_LAW   /* Use G711 Alaw for upspeed relay */

/* Call Discriminator options */

#define HSNET_CDIS_OPTION_CNGACTIVE        CDIS_CNG_ENABLE /* Enable CNG detection */
#define HSNET_CDIS_OPTION_V18ACTIVE        CDIS_V18_ENABLE /* Enable V18 detection */
#define HSNET_CDIS_OPTION_BELL103ACTIVE    CDIS_BELL103_ENABLE /* Enable Bell 103 detection */
#define HSNET_CDIS_OPTION_CUSTOMTONEACTIVE CDIS_CUSTOMTONE_ENABLE /* Enable custom tone detection */


/* Net OPtions */
#define HSNET_NET_OPTION_CDIS                 0x0001 /* Enable call discriminator */
#define HSNET_NET_OPTION_FAXRELAY             0x0002 /* Switch to fax relay */
#define HSNET_NET_OPTION_MODEMRELAY           0x0004 /* Switch to modem relay */
#define HSNET_NET_OPTION_AUTO_ENC_SWITCH      0x0008 /* Switch to modem relay */
#define HSNET_NET_OPTION_ECAN_CONTROL         0x0010 /* Switch to modem relay */
#define HSNET_NET_OPTION_IGNORE_CNG_ING_EVT   0x0020 /* No Switch on CNG ingressevt  */
#define HSNET_NET_OPTION_IGNORE_CNG_EGR_EVT   0x0040 /*No switch on CNG egress evt (set by default) */
#define HSNET_NET_OPTION_VBD_FAXR_AUTO_SWITCH_DISABLE 0x0080 /* Disable auto-switch capability into VBD and FAXR */

/**********************************/
/*       Default Registers        */
/**********************************/

#define HSNET_PXD_UNASSIGNED_HDL           0xFF  /* unassigned pxd handle */


#define HSNET_NET_OPTION_DEFAULT      (HSNET_NET_OPTION_CDIS | \
                                       HSNET_NET_OPTION_FAXRELAY | \
                                       HSNET_NET_OPTION_AUTO_ENC_SWITCH | \
                                       HSNET_NET_OPTION_IGNORE_CNG_EGR_EVT | \
                                       HSNET_NET_OPTION_ECAN_CONTROL )

#define HSNET_CDIS_OPTION_DEFAULT            (HSNET_CDIS_OPTION_CNGACTIVE)
#define HSNET_PTE_OPTION_CALLSETUP_DEFAULT   (HSNET_PTE_OPTION_DIG_EVT)
#define HSNET_PTE_OPTION_VOICE_DEFAULT       (HSNET_PTE_OPTION_DIG_RELAY | \
                                              HSNET_PTE_OPTION_DIG_EVT )


/* NetVHD Configuration Option Registers */

typedef struct
{
   HAPI_UINT16   hsxStatsInterval;          /* Statistics interval(ms) */
   HAPI_UINT16   hsxHeartbeatInterval;      /* Heartbeat interval(ms) */
   HAPI_UINT16   hsxPxdHandle;              /* Associated PXD */
   HAPI_UINT16   hsxNetOptions;             /* Net options */
   HAPI_UINT16   hsxCallDOptions;           /* Call discriminator options */
   HAPI_UINT16   hsxPteCallSetupOptions;   /* Pte option in Packet voice*/
   HAPI_UINT16   hsxPtePacketVoiceOptions;   /* Pte option in Packet voice*/
   HAPI_UINT16   hsxPacketVoiceEncoder;     /* Packet voice default encoder */
   HAPI_UINT16   hsxVoiceBandDefault;       /* Voiceband Data Mode default */
   HAPI_UINT16   hsxVoiceSuperPktInterval;  /* Packet Voice default superpacketization interval in 8kHz samples*/
   HAPI_UINT16   hsxVBDSuperPktInterval;    /* Voiceband Data default superpacketization interval in 8kHz samples*/
} HSZNETREGS;

#define HAPI_NET_DEF_REGS   \
{  \
   0,                                  /* Stats interval */  \
   0,                                  /* Heartbeat interval */  \
   HSNET_PXD_UNASSIGNED_HDL,           /* PxDHandle */  \
   HSNET_NET_OPTION_DEFAULT,           /* Net default options */ \
   HSNET_CDIS_OPTION_DEFAULT,          /* Call discriminator options */  \
   HSNET_PTE_OPTION_CALLSETUP_DEFAULT,            /* dtmf relay*/ \
   HSNET_PTE_OPTION_VOICE_DEFAULT,            /* dtmf relay*/ \
   (HAPI_VAD_TYPE_GENERIC << 8) + HAPI_PKT_VC_G711_ULAW,                /* Default encoder algorithm */  \
   (HAPI_VAD_TYPE_NONE << 8) + HAPI_PKT_VC_G711_ULAW,                     /* Default rate for Voiceband Data Mode */  \
   160,                                /* 20ms superpactetization interval for Packet Voice */ \
   160                                /* 20ms superpactetization interval for VBD*/ \
}



#endif      /* HAPI_NET_H */



