/****************************************************************************
*
*  Copyright (c) 2002-2008 Broadcom
*
*  This program is the proprietary software of Broadcom and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*    Description:
*
*      Payload Interface.
*
****************************************************************************/

#ifndef HAPI_PKT_H
#define HAPI_PKT_H

/* ---- Include Files ---------------------------------------- */
#ifndef HAPI_TYPES_H
#include <hapi_types.h>
#endif

/* ---- Constants and Types ---------------------------------- */

/***************************************************************************
** HAPI Packet Header Format
****************************************************************************/

typedef struct
{
   HAPI_UINT16   payloadID;              /* Payload ID containing type and code */
   HAPI_UINT16   payloadLength;          /* Payload length (number of 8-bit bytes) */
   HAPI_UINT16   payloadTimeStampHigh;   /* High word of payload timestamp (not used by jitter buffer) */
   HAPI_UINT16   payloadTimeStampLow;    /* Low word of payload timestamp (used by jitter buffer) */

} HAPI_PKT_HDR;

typedef struct
{
   HAPI_UINT16   payloadID;              /* Payload ID containing type and code */
   HAPI_UINT16   payloadLength;          /* Payload length (number of 8-bit bytes) */
   HAPI_UINT16   payloadOffset;          /* Offset of the payload from start of this structure */
   HAPI_UINT16   payloadTimeStampLow;    /* Low word of payload timestamp (used by jitter buffer) */

} HAPI_PKT_EGRESS_HDR;

#define HAPI_PKT_HDR_SIZE sizeof( HAPI_PKT_HDR )

/***************************************************************************
** Payload ID access macros
****************************************************************************/
#define HAPI_PKT_MKPID( payloadType, payloadCode ) ((payloadType << 8) | (payloadCode))
#define HAPI_PKT_GETPT( payloadID )                (((HAPI_PKT_TYPE)((payloadID) >> 8)))
#define HAPI_PKT_GETPC( payloadID )                ((payloadID) & 0xff)

/* Modify payload code but keep payload type the same */
#define HAPI_PKT_MODPC( payloadID, payloadCode )   (payloadID = (payloadCode) | ((payloadID) & 0xff00))

/* Get VoiceCode */
#define HAPI_PKT_GETVC( payloadID )                ((HAPI_PKT_VOICECODE)((payloadID) & 0xff))

#define HAPI_PKT_MODPT( payloadID, payloadType )   (payloadID = ((payloadType) << 8) | ((payloadID) & 0xff))
#define HAPI_PKT_GETPAYLOADP(bufp)                 ((void *)((HAPI_SINT16*)(bufp) + SIZEOF_W(HAPI_PKT_EGRESS_HDR)))
#define GET_NETPKT_TOTALSIZE( packetp )            (SIZEOF_B(HAPI_PKT_EGRESS_HDR) + (((HAPI_PKT_EGRESS_HDR *)packetp)->payloadLength ) )

#define HAPI_PKT_IS_722( payloadID ) \
   (((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G722_64_MODE_1) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G722_64_MODE_2) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G722_64_MODE_3))

#define HAPI_PKT_IS_7221( payloadID ) \
   (((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G7221_24) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G7221_32))

#define HAPI_PKT_IS_7231( payloadID ) \
   (((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G7231_53) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G7231_63) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G7231_SID))

#define HAPI_PKT_IS_729( payloadID ) \
   (((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G729_8) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G729A_8) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G729_118) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_G729_SID))

#define HAPI_PKT_IS_711_726( payloadID ) \
   ((HAPI_PKT_GETVC(payloadID)) <= HAPI_PKT_VC_G726_40)

#define HAPI_PKT_IS_726( payloadID ) \
   (((HAPI_PKT_GETVC(payloadID)) >= HAPI_PKT_VC_G726_16) \
 && ((HAPI_PKT_GETVC(payloadID)) <= HAPI_PKT_VC_G726_40))

#define HAPI_PKT_IS_GSM_EFR( payloadID ) \
   (((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_GSM_EFR_122) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_GSM_EFR_SID))

#define HAPI_PKT_IS_GSM_FR( payloadID ) \
   ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_GSM_FR_13)

#define HAPI_PKT_IS_GSM_HR( payloadID ) \
   ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_GSM_HR_56)

/* Bandwidth-efficient mode */
#define HAPI_PKT_IS_GSM_AMR( payloadID ) \
   ((((HAPI_PKT_GETVC(payloadID)) >= HAPI_PKT_VC_GSM_AMR_475) \
 && ((HAPI_PKT_GETVC(payloadID)) <= HAPI_PKT_VC_GSM_AMR_122)) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_AMR_SID))

/* Octet-aligned mode */
#define HAPI_PKT_IS_GSM_AMR_OA( payloadID ) \
   ((((HAPI_PKT_GETVC(payloadID)) >= HAPI_PKT_VC_GSM_AMR_OA_475) \
 && ((HAPI_PKT_GETVC(payloadID)) <= HAPI_PKT_VC_GSM_AMR_OA_122)) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_AMR_OA_SID))

/* Bandwidth-efficient mode */
#define HAPI_PKT_IS_AMR_WB( payloadID ) \
   ((((HAPI_PKT_GETVC(payloadID)) >= HAPI_PKT_VC_WB_AMR_660) \
 && ((HAPI_PKT_GETVC(payloadID)) <= HAPI_PKT_VC_WB_AMR_2385)) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_AMR_WB_SID))

/* Octet-aligned mode */
#define HAPI_PKT_IS_AMR_WB_OA( payloadID ) \
   ((((HAPI_PKT_GETVC(payloadID)) >= HAPI_PKT_VC_WB_AMR_OA_660) \
 && ((HAPI_PKT_GETVC(payloadID)) <= HAPI_PKT_VC_WB_AMR_OA_2385)) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_AMR_WB_OA_SID))

#define HAPI_PKT_IS_VARIABLE_RATE( payloadID ) \
   ((HAPI_PKT_GETVC(payloadID) >= HAPI_PKT_VC_VARIABLE_MIN) \
 && (HAPI_PKT_GETVC(payloadID) <= HAPI_PKT_VC_VARIABLE_MAX))

#define HAPI_PKT_IS_ILBC( payloadID ) \
   (((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_ILBC20) \
 || ((HAPI_PKT_GETVC(payloadID)) == HAPI_PKT_VC_ILBC30))


#define HAPI_PKT_IS_WIDEBAND( payloadID ) \
       (((HAPI_PKT_GETVC(payloadID)) >= HAPI_PKT_VC_WIDEBAND_MIN)   \
        && ((HAPI_PKT_GETVC(payloadID)) <=HAPI_PKT_VC_WIDEBAND_MAX))

/* Set high and low parts of a 32 bit timestamp */
#define HAPI_PKT_SET_TS(hapiPktHdrp, timestamp) \
      (hapiPktHdrp)->payloadTimeStampHigh = (HAPI_UINT16)((timestamp) >> 16);  \
      (hapiPktHdrp)->payloadTimeStampLow = (HAPI_UINT16)(timestamp);           \

/***************************************************************************
** Primary Payload types
****************************************************************************/

typedef enum
{
   HAPI_PKT_UNKNOWN = 0,            /* Unknown payload type */
   HAPI_PKT_VOICE,                  /* Voice payload */
   HAPI_PKT_SID,                    /* Sid payload */
   HAPI_PKT_TONE,                   /* Voiceband signal payload */
   HAPI_PKT_VOICEBANDDATA,          /* Voiceband data payload */
   HAPI_PKT_FAXRELAY,               /* Fax relay payload */
   HAPI_PKT_DATARELAY,              /* Data relay payload */
   HAPI_PKT_CLASSRELAY,             /* CLASS relay payload */
   HAPI_PKT_PCMCLEAR,               /* PCM Clear Channel payload */
   HAPI_PKT_PARTIAL_FRAME,          /* Code to indicate partial frame in progress (e.g. G723.1) */
   HAPI_PKT_MAXTYPES

} HAPI_PKT_TYPE;

#define HAPI_PKT_IS_SID(payloadID) (HAPI_PKT_GETPT(payloadID)==HAPI_PKT_SID)
#define HAPI_PKT_IS_VBD(pt) ((pt)<=HAPI_PKT_VC_VBD_MAX)

/****************************************************************************
  Packet Voice Algorithms
****************************************************************************/
typedef enum
{
   HAPI_PKT_ALG_G711       = 0,               /* ITU-T G.711 PCM */
   HAPI_PKT_ALG_G726       = 1,               /* ITU-T G.726 ADPCM */
   HAPI_PKT_ALG_G729       = 2,               /* ITU-T G.729 CS-ACELP */
   HAPI_PKT_ALG_G7231      = 3,               /* ITU-T G.723.1 ACELP and MP-MLQ */
   HAPI_PKT_ALG_G728       = 4,               /* ITU-T G.728 (Annex G) LD-CELP */
   HAPI_PKT_ALG_BV16       = 5,               /* Broadcom standard BroadVoice16 vocoder */
   HAPI_PKT_ALG_LINPCM128  = 6,               /* Linear PCM (128 kbps) */
   HAPI_PKT_ALG_G711WB     = 7,               /* G.711 PCM wideband (128 kbps) */
   HAPI_PKT_ALG_LINPCM256  = 8,               /* Linear PCM wideband (256 kbps) */
   HAPI_PKT_ALG_BV32       = 9,               /* Broadcom standard BroadVoice32 vocoder */
   HAPI_PKT_ALG_G722       = 10,              /* ITU-T G.722 wideband ADPCM */
   HAPI_PKT_ALG_G7221      = 11,              /* ITU-T G.722.1 wideband 7 kHz RMLT */
   HAPI_PKT_ALG_ILBC20     = 12,              /* iLBC 15.2 kbps speech coder */
   HAPI_PKT_ALG_ILBC30     = 13,              /* iLBC 13.3 kbps speech coder */
   HAPI_PKT_ALG_GSMAMR     = 14,              /* GSM AMR narrowband */
   HAPI_PKT_ALG_GSMEFR     = 15,              /* GSM EFR */
   HAPI_PKT_ALG_GSMFR      = 16,              /* GSM FR */
   HAPI_PKT_ALG_GSMHR      = 17,              /* GSM HR */
   HAPI_PKT_ALG_AMRWB      = 18,              /* AMR-WB */
   HAPI_PKT_ALG_LINPCM64   = 19,              /* Linear PCM (64 kbps) */
   HAPI_PKT_ALG_MAX
} HAPI_PKT_VOICEALG;

/***************************************************************************
** Voice Payload Codes
****************************************************************************/
typedef enum
{

   HAPI_PKT_VC_G711_ULAW      = 0,     /* G711 - uLaw - 64 kbps */
   HAPI_PKT_VC_G711_ALAW      = 1,     /* G711 - aLaw - 64 kbps */
   HAPI_PKT_VC_G726_16        = 2,     /* G.726 - 16 kbps */
   HAPI_PKT_VC_G726_24        = 3,     /* G.726 - 24 kbps */
   HAPI_PKT_VC_G726_32        = 4,     /* G.726 - 32 kbps */
   HAPI_PKT_VC_G726_40        = 5,     /* G.726 - 40 kbps */
   HAPI_PKT_VC_VBD_MAX        = 5,     /* max VBD payload code */

   HAPI_PKT_VC_G729A_8        = 6,     /* G.729(A) - 8 kbps */
   HAPI_PKT_VC_G729_8         = 7,     /* G.729(A) - 8 kbps */
   HAPI_PKT_VC_G729_118       = 8,     /* G.729E - 11.8 kbps */
   HAPI_PKT_VC_G7231_53       = 9,     /* G.723.1 - 5.3 kbps */
   HAPI_PKT_VC_G7231_63       = 10,    /* G.723.1 - 6.3 kbps */
   HAPI_PKT_VC_G728_16        = 11,    /* G.728 - 16 kbps */
   HAPI_PKT_VC_BV16           = 12,    /* BroadVoice Narrow Band 16 kbps */
   HAPI_PKT_VC_LINPCM128      = 13,    /* linear PCM - 128 kbps */
   HAPI_PKT_VC_ILBC20         = 14,    /* iLBC speech coder - 20 ms frame / 15.2 kbps */
   HAPI_PKT_VC_ILBC30         = 15,    /* iLBC speech coder - 30 ms frame / 13.3 kbps */
   HAPI_PKT_VC_GSM_FR_13      = 16,    /* GSM FR - 13 kbps */
   HAPI_PKT_VC_GSM_EFR_122    = 17,    /* GSM EFR - 12.2 kbps */
   HAPI_PKT_VC_GSM_AMR_475    = 18,    /* GSM AMR - 4.75 kbps */
   HAPI_PKT_VC_GSM_AMR_515    = 19,    /* GSM AMR - 5.15 kbps */
   HAPI_PKT_VC_GSM_AMR_590    = 20,    /* GSM AMR - 5.90 kbps */
   HAPI_PKT_VC_GSM_AMR_670    = 21,    /* GSM AMR - 6.70 kbps */
   HAPI_PKT_VC_GSM_AMR_740    = 22,    /* GSM AMR - 7.40 kbps */
   HAPI_PKT_VC_GSM_AMR_795    = 23,    /* GSM AMR - 7.95 kbps */
   HAPI_PKT_VC_GSM_AMR_102    = 24,    /* GSM AMR - 10.2 kbps */
   HAPI_PKT_VC_GSM_AMR_122    = 25,    /* GSM AMR - 12.2 kbps */
   HAPI_PKT_VC_GSM_AMR_OA_475 = 26,    /* GSM AMR Octet Aligned - 4.75 kbps */
   HAPI_PKT_VC_GSM_AMR_OA_515 = 27,    /* GSM AMR Octet Aligned - 5.15 kbps */
   HAPI_PKT_VC_GSM_AMR_OA_590 = 28,    /* GSM AMR Octet Aligned - 5.90 kbps */
   HAPI_PKT_VC_GSM_AMR_OA_670 = 29,    /* GSM AMR Octet Aligned - 6.70 kbps */
   HAPI_PKT_VC_GSM_AMR_OA_740 = 30,    /* GSM AMR Octet Aligned - 7.40 kbps */
   HAPI_PKT_VC_GSM_AMR_OA_795 = 31,    /* GSM AMR Octet Aligned - 7.95 kbps */
   HAPI_PKT_VC_GSM_AMR_OA_102 = 32,    /* GSM AMR Octet Aligned - 10.2 kbps */
   HAPI_PKT_VC_GSM_AMR_OA_122 = 33,    /* GSM AMR Octet Aligned - 12.2 kbps */
   HAPI_PKT_VC_LINPCM64       = 34,    /* Linear PCM - 64 kbps */
   HAPI_PKT_VC_GSM_HR_56      = 35,    /* GSM HR - 5.6 kbps */

   HAPI_PKT_VC_WIDEBAND_MIN   = 36,    /* Start of wideband codecs */
   HAPI_PKT_VC_G711WB_ULAW    = 36,    /* G711 wideband - uLaw - 128 kbps */
   HAPI_PKT_VC_G711WB_ALAW    = 37,    /* G711 wideband - aLaw - 128 kbps */
   HAPI_PKT_VC_BV32           = 38,    /* BroadVoice Wide Band 32 kbps */
   HAPI_PKT_VC_LINPCM256      = 39,    /* linear PCM - 256 kbps */
   HAPI_PKT_VC_G722_64_MODE_1 = 40,    /* G.722 - 64 kbps, mode 1 => 64 kbps audio */
   HAPI_PKT_VC_G722_64_MODE_2 = 41,    /* G.722 - 64 kbps, mode 2 => 56 kbps audio */
   HAPI_PKT_VC_G722_64_MODE_3 = 42,    /* G.722 - 64 kbps, mode 3 => 48 kbps audio */
   HAPI_PKT_VC_G7221_24       = 43,    /* G.722.1 - 24 kbps, wideband */
   HAPI_PKT_VC_G7221_32       = 44,    /* G.722.1 - 32 kbps, wideband */
 
   HAPI_PKT_VC_WB_AMR_660     = 45,    /* AMR WB - 6.6 kbps */
   HAPI_PKT_VC_WB_AMR_885     = 46,    /* AMR WB - 8.85 kbps */
   HAPI_PKT_VC_WB_AMR_1265    = 47,    /* AMR WB - 12.65 kbps */
   HAPI_PKT_VC_WB_AMR_1425    = 48,    /* AMR WB - 14.25 kbps */
   HAPI_PKT_VC_WB_AMR_1585    = 49,    /* AMR WB - 15.85 kbps */
   HAPI_PKT_VC_WB_AMR_1825    = 50,    /* AMR WB - 18.25 kbps */
   HAPI_PKT_VC_WB_AMR_1985    = 51,    /* AMR WB - 19.85 kbps */
   HAPI_PKT_VC_WB_AMR_2305    = 52,    /* AMR WB - 23.05 kbps */
   HAPI_PKT_VC_WB_AMR_2385    = 53,    /* AMR WB - 23.85 kbps */
   HAPI_PKT_VC_WB_AMR_OA_660  = 54,    /* AMR WB Octet Aligned - 6.6 kbps */
   HAPI_PKT_VC_WB_AMR_OA_885  = 55,    /* AMR WB Octet Aligned - 8.85 kbps */
   HAPI_PKT_VC_WB_AMR_OA_1265 = 56,    /* AMR WB Octet Aligned - 12.65 kbps */
   HAPI_PKT_VC_WB_AMR_OA_1425 = 57,    /* AMR WB Octet Aligned - 14.25 kbps */
   HAPI_PKT_VC_WB_AMR_OA_1585 = 58,    /* AMR WB Octet Aligned - 15.85 kbps */
   HAPI_PKT_VC_WB_AMR_OA_1825 = 59,    /* AMR WB Octet Aligned - 18.25 kbps */
   HAPI_PKT_VC_WB_AMR_OA_1985 = 60,    /* AMR WB Octet Aligned - 19.85 kbps */
   HAPI_PKT_VC_WB_AMR_OA_2305 = 61,    /* AMR WB Octet Aligned - 23.05 kbps */
   HAPI_PKT_VC_WB_AMR_OA_2385 = 62,    /* AMR WB Octet Aligned - 23.85 kbps */
   HAPI_PKT_VC_WIDEBAND_MAX   = 62,    /* End of wideband codecs */
 
   HAPI_PKT_VC_ALG_MAX        = 63,


   /* SID Payload Codes */
   HAPI_PKT_VC_SID_MIN              = 64,
   HAPI_PKT_VC_GENERIC_SID          = 65,  /* Generic SID */
   HAPI_PKT_VC_G729_SID             = 66,  /* G729 SID */
   HAPI_PKT_VC_G7231_SID            = 67,  /* G7231 SID */
   HAPI_PKT_VC_AMR_SID              = 68,  /* GSM-AMR SID */
   HAPI_PKT_VC_GSM_EFR_SID          = 69,  /* GSM-EFR SID */
   HAPI_PKT_VC_AMR_WB_SID           = 70,  /* AMR-WB SID */
   HAPI_PKT_VC_AMR_OA_SID           = 71,  /* GSM-AMR octet-aligned SID */
   HAPI_PKT_VC_AMR_WB_OA_SID        = 72,  /* AMR-WB octet-aligned SID */
   HAPI_PKT_VC_SID_MAX              = 72,

   /* Other */
   HAPI_PKT_VC_PCM_CLEAR            = 80,  /* voice code for PCM CLEAR, which is equivalent to HAPI_PKT_VC_G711_ALAW */
   HAPI_PKT_VC_IDLE                 = 81,  /* Idle Pattern SID */
   HAPI_PKT_VC_GSM_AMR_FRAME        = 82,  /* GSM AMR frame packets, IF1, IF2, other */

   /*
    * These variable payload codes are used to allow rate switching between
    * codecs that have the property that their rate can be inferred from
    * data in the packet payload. This avoids an unnecessary RTP payload map
    * entry for each rate.
    */
   HAPI_PKT_VC_VARIABLE_MIN         = 90,
   HAPI_PKT_VC_GSM_AMR_VARIABLE     = 90,  /* all 8 amr speeds */
   HAPI_PKT_VC_G7231_VARIABLE       = 91,  /* both g723.1 speeds */
   HAPI_PKT_VC_AMR_WB_VARIABLE      = 92,  /* all 9 amrwb speeds */
   HAPI_PKT_VC_GSM_AMR_OA_VARIABLE  = 93,  /* all 8 amr octet-aligned speeds */
   HAPI_PKT_VC_AMR_WB_OA_VARIABLE   = 94,  /* all 9 amrwb octet-aligned speeds */
   
   HAPI_PKT_VC_VARIABLE_MAX         = 94,

   HAPI_PKT_VC_VOICE_NOCODE         = 0xff   /* Dummy code for initialization and rate switching */

} HAPI_PKT_VOICECODE;

/***************************************************************************
** Fax Payload codes
****************************************************************************/

typedef enum
{
   HAPI_PKT_FC_NOSIGNAL=0,
   HAPI_PKT_FC_CNG,                          /* CNG */
   HAPI_PKT_FC_CED,                          /* CED */
   HAPI_PKT_FC_V21_PREAMBLE,                 /* V.21 HDLC flags */

   HAPI_PKT_FC_V27_2400_TRAINING,            /* V.27ter - 2400 bps training */
   HAPI_PKT_FC_V27_4800_TRAINING,            /* V.27ter - 4800 bps training */
   HAPI_PKT_FC_V29_7200_TRAINING,            /* V.29 - 7200 bps training */
   HAPI_PKT_FC_V29_9600_TRAINING,            /* V.29 - 9600 bps training */
   HAPI_PKT_FC_V33_12000_TRAINING,           /* V.33 - 12000 bps training */
   HAPI_PKT_FC_V33_14400_TRAINING,           /* V.33 - 14400 bps training */
   HAPI_PKT_FC_V17_7200_SHORT_TRAINING,      /* V.17 - 7200 bps short training */
   HAPI_PKT_FC_V17_7200_LONG_TRAINING,       /* V.17 - 7200 bps long training */
   HAPI_PKT_FC_V17_9600_SHORT_TRAINING,      /* V.17 - 9600 bps short training */
   HAPI_PKT_FC_V17_9600_LONG_TRAINING,       /* V.17 - 9600 bps long training */
   HAPI_PKT_FC_V17_12000_SHORT_TRAINING,     /* V.17 - 12000 bps short training */
   HAPI_PKT_FC_V17_12000_LONG_TRAINING,      /* V.17 - 12000 bps long training */
   HAPI_PKT_FC_V17_14400_SHORT_TRAINING,     /* V.17 - 14400 bps short training */
   HAPI_PKT_FC_V17_14400_LONG_TRAINING,      /* V.17 - 14400 bps long training */

   HAPI_PKT_FC_V21,                          /* V.21 - 300 bps frame */
   HAPI_PKT_FC_V21_END,                      /* V.21 - 300 bps non-final frame with good FCS */
   HAPI_PKT_FC_V21_END_FINAL,                /* V.21 - 300 bps final frame with good FCS */
   HAPI_PKT_FC_V21_END_BAD,                  /* V.21 - 300 bps non-final frame with bad FCS */
   HAPI_PKT_FC_V21_END_BAD_FINAL,            /* V.21 - 300 bps final frame with bad FCS */
   HAPI_PKT_FC_V27_2400,                     /* V.27ter - 2400 bps */
   HAPI_PKT_FC_V27_4800,                     /* V.27ter - 4800 bps */
   HAPI_PKT_FC_V29_7200,                     /* V.29 - 7200 bps */
   HAPI_PKT_FC_V29_9600,                     /* V.29 - 9600 bps */
   HAPI_PKT_FC_V33_12000,                    /* V.33 - 12000 bps */
   HAPI_PKT_FC_V33_14400,                    /* V.33 - 14400 bps */
   HAPI_PKT_FC_V17_7200,                     /* V.17 - 7200 bps */
   HAPI_PKT_FC_V17_9600,                     /* V.17 - 9600 bps */
   HAPI_PKT_FC_V17_12000,                    /* V.17 - 12000 bps */
   HAPI_PKT_FC_V17_14400,                    /* V.17 - 14400 bps */
   HAPI_PKT_FC_V27_2400_ECM,                 /* V.27ter - 2400 bps ECM packet */
   HAPI_PKT_FC_V27_4800_ECM,                 /* V.27ter - 4800 bps ECM packet */
   HAPI_PKT_FC_V29_7200_ECM,                 /* V.29 - 7200 bps ECM packet */
   HAPI_PKT_FC_V29_9600_ECM,                 /* V.29 - 9600 bps ECM packet */
   HAPI_PKT_FC_V33_12000_ECM,                /* V.33 - 12000 bps ECM packet */
   HAPI_PKT_FC_V33_14400_ECM,                /* V.33 - 14400 bps ECM packet */
   HAPI_PKT_FC_V17_7200_ECM,                 /* V.17 - 7200 bps ECM packet */
   HAPI_PKT_FC_V17_9600_ECM,                 /* V.17 - 9600 bps ECM packet */
   HAPI_PKT_FC_V17_12000_ECM,                /* V.17 - 12000 bps ECM packet */
   HAPI_PKT_FC_V17_14400_ECM,                /* V.17 - 14400 bps ECM packet */
   HAPI_PKT_FC_V27_2400_ECM_END,             /* V.27ter - 2400 bps ECM packet end */
   HAPI_PKT_FC_V27_4800_ECM_END,             /* V.27ter - 4800 bps ECM packet end */
   HAPI_PKT_FC_V29_7200_ECM_END,             /* V.29 - 7200 bps ECM packet end */
   HAPI_PKT_FC_V29_9600_ECM_END,             /* V.29 - 9600 bps ECM packet end */
   HAPI_PKT_FC_V33_12000_ECM_END,            /* V.33 - 12000 bps ECM packet end */
   HAPI_PKT_FC_V33_14400_ECM_END,            /* V.33 - 14400 bps ECM packet end */
   HAPI_PKT_FC_V17_7200_ECM_END,             /* V.17 - 7200 bps ECM packet end */
   HAPI_PKT_FC_V17_9600_ECM_END,             /* V.17 - 9600 bps ECM packet end */
   HAPI_PKT_FC_V17_12000_ECM_END,            /* V.17 - 12000 bps ECM packet end */
   HAPI_PKT_FC_V17_14400_ECM_END,            /* V.17 - 14400 bps ECM packet end */
   HAPI_PKT_FC_V27_2400_ECM_END_BAD,         /* V.27ter - 2400 bps ECM packet end */
   HAPI_PKT_FC_V27_4800_ECM_END_BAD,         /* V.27ter - 4800 bps ECM packet end */
   HAPI_PKT_FC_V29_7200_ECM_END_BAD,         /* V.29 - 7200 bps ECM packet end */
   HAPI_PKT_FC_V29_9600_ECM_END_BAD,         /* V.29 - 9600 bps ECM packet end */
   HAPI_PKT_FC_V33_12000_ECM_END_BAD,        /* V.33 - 12000 bps ECM packet end */
   HAPI_PKT_FC_V33_14400_ECM_END_BAD,        /* V.33 - 14400 bps ECM packet end */
   HAPI_PKT_FC_V17_7200_ECM_END_BAD,         /* V.17 - 7200 bps ECM packet end */
   HAPI_PKT_FC_V17_9600_ECM_END_BAD,         /* V.17 - 9600 bps ECM packet end */
   HAPI_PKT_FC_V17_12000_ECM_END_BAD,        /* V.17 - 12000 bps ECM packet end */
   HAPI_PKT_FC_V17_14400_ECM_END_BAD,        /* V.17 - 14400 bps ECM packet end */

   HAPI_PKT_FC_PAGEEND,                      /* Phase C non-ECM data page end */
   HAPI_PKT_FC_ECM_PAGEEND,                  /* Phase C ECM data page end */
   HAPI_PKT_FC_V21_SIGEND,                   /* V.21 sig end */

   HAPI_PKT_FC_FAXMAX

} HAPI_PKT_FAXCODE;

/***************************************************************************
** Fax over AAL2 Payload codes
****************************************************************************/

typedef enum
{
   HAPI_PKT_FC_AAL2_T30_PREAMBLE = 0,        /* T30 preamble indicator */
   HAPI_PKT_FC_AAL2_T30_DATA,                /* T30 data */
   HAPI_PKT_FC_AAL2_EPT,                     /* EPT indicator */
   HAPI_PKT_FC_AAL2_TRAINING,                /* Training indicator */
   HAPI_PKT_FC_AAL2_FAX_DATA,                /* Fax high-speed data */
   HAPI_PKT_FC_AAL2_FAX_IDLE                 /* Fax end high-speed indicator */

} HAPI_PKT_FAXCODE_AAL2;

/***************************************************************************
** Modem Payload codes
****************************************************************************/

typedef struct
{
   HAPI_PKT_HDR hsxPacketHdr;       /* Packet header */
   HAPI_UINT16       numBits;            /* Number of bits in packet */
   HAPI_UINT16       dataBit;            /* First word of data bit */
} HAPI_PKT_MODEMPACKET;

/* size of modempacket header (hsxPacketHdr + numBits) */
#define HAPI_PKT_MODEMPACKETHDRSIZE (sizeof(HAPI_PKT_HDR) + sizeof(HAPI_UINT16))

typedef enum
{
   HAPI_PKT_MC_NOCARRIER = 0,    /* No Carrier in this payload */
   HAPI_PKT_MC_CED,              /* CED */
   HAPI_PKT_MC_DATA,             /* Data in this payload */

   /* Modem Tone Codes */
   HAPI_PKT_MC_ANS_PR,           /* Answer tone with phase reversal evert 450ms */
   HAPI_PKT_MC_ANSAM_PR,         /* AM modulated Answer tone with phase reversal
                                   every 450ms */
   HAPI_PKT_MC_AC,               /* AC tone */
   HAPI_PKT_MC_AA,               /* AA tone */
   HAPI_PKT_MC_USB1,             /* Unscrambled binary 1's */
   HAPI_PKT_MC_S1L,              /* Unscrambled double dibits low channel */

   HAPI_PKT_MC_MODEMRATE,        /* modem bit rate */
   HAPI_PKT_MC_CM,               /* V.8 call menu */
   HAPI_PKT_MC_JM,               /* V.8 joint menu */
   HAPI_PKT_MC_HDLC              /* HDLC flag */
} HAPI_PKT_MODEMCODE;

/***************************************************************************
** Voice-Signal (Tone) Payload codes
****************************************************************************/

typedef enum
{
   HAPI_PKT_TC_DTMF_ON =0 , /* DTMF Relay Packet */
   HAPI_PKT_TC_DTMF_OFF,    /* DTMF Relay Packet */
   HAPI_PKT_TC_MFR1_ON,     /* MF-R1 Relay Packet */
   HAPI_PKT_TC_MFR1_OFF,    /* MF-R1 Relay Packet */
   HAPI_PKT_TC_MFR2F_ON,    /* MF-R2 Forward Relay Packet */
   HAPI_PKT_TC_MFR2F_OFF,   /* MF-R2 Forward Relay Packet */
   HAPI_PKT_TC_MFR2B_ON,    /* MF-R2 Backward Relay Packet */
   HAPI_PKT_TC_MFR2B_OFF,   /* MF-R2 Backward Relay Packet */
   HAPI_PKT_TC_TONE_UPSPEED,/* PVE vocoder upspeed control */
   HAPI_PKT_TC_TONE_INGRESS,/* Notify PVE of Ingress media queue change */
                            /* dummy code for initialization and rate switching */
   HAPI_PKT_TC_NO_TONE,
   HAPI_PKT_TC_CPG_ON,
   HAPI_PKT_TC_CUSTOM_ON,
   HAPI_PKT_TC_DIALSTRINGCOMPLETE,

   HAPI_PKT_TC_TONE_NOCODE = 0xff

} HAPI_PKT_TONECODE;

#define HAPI_PKT_TC_OFF_BIT 1 /* All odd values indicate tone off */

typedef struct
{
   HAPI_UINT16 digitpwrlevel;
   HAPI_UINT16 duration;

} HAPI_PKT_TONEPAYLOAD;
/* Macros to extract the digit and power level. They are combined for source sharing */
#define HAPI_PKT_TONEPAYLOAD_DIGIT(payloadp) ((((HAPI_PKT_TONEPAYLOAD *)(payloadp))->digitpwrlevel >> 8) & 0xff)
#define HAPI_PKT_TONEPAYLOAD_POWER(payloadp) (((HAPI_PKT_TONEPAYLOAD *)(payloadp))->digitpwrlevel & 0xff)



/***************************************************************************
** Class Payload codes
****************************************************************************/

typedef enum {
   HAPI_PKT_CLASS_START,                 /* Start of CLASS message */
   HAPI_PKT_CLASS_BODY,                  /* CLASS message */
   HAPI_PKT_CLASS_END                    /* CLASS message terminator */
} HAPI_PKT_CLASSRELAYCODE;

#endif      /* HAPI_PKT_H */



