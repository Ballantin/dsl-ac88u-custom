/**********************************************************************************
** Copyright © 2008 Broadcom
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

/****************************************************************************
*
*    Filename: codec.h
*
****************************************************************************
*    Description:
*
*        This file contains common definitions for codec types and
*        static RTP payload types.
*
****************************************************************************/

#ifndef CODEC_H
#define CODEC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <bosTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

/* Voice codec types */
typedef enum
{
   CODEC_NULL,           /* NULL */
   CODEC_PCMU,           /* G.711 ulaw */
   CODEC_PCMA,           /* G.711 alaw */
   CODEC_G726_16,        /* G.726 - 16 kbps */
   CODEC_G726_24,        /* G.726 - 24 kbps */
   CODEC_G726_32,        /* G.726 - 32 kbps */
   CODEC_G726_40,        /* G.726 - 40 kbps */
   CODEC_G7231_53,       /* G.723.1 - 5.3 kbps */
   CODEC_G7231_63,       /* G.723.1 - 6.3 kbps */
   CODEC_G7231A_53,      /* G.723.1A - 5.3 kbps */
   CODEC_G7231A_63,      /* G.723.1A - 6.3 kbps */
   CODEC_G729A,          /* G.729A */
   CODEC_G729B,          /* G.729B */
   CODEC_G711_LINEAR,    /* Linear media queue data */
   CODEC_G729,           /* G.729 */
   CODEC_G729E,          /* G.729E */
   CODEC_BV16,           /* BRCM Broadvoice - 16 kbps */
   CODEC_BV32,           /* BRCM Broadvoice - 32 kbps */
   CODEC_NTE,            /* Named telephone events */
   CODEC_ILBC_20,        /* iLBC speech coder - 20 ms frame / 15.2 kbps */
   CODEC_ILBC_30,        /* iLBC speech coder - 30 ms frame / 13.3 kbps */
   CODEC_G7231_53_VAR,   /* G723.1 variable rates (preferred=5.3) */
   CODEC_G7231_63_VAR,   /* G723.1 variable rates (preferred=6.3) */
   CODEC_G7231_VAR,      /* G723.1 variable rates */
   CODEC_T38,            /* T.38 fax relay */
   CODEC_T38_MUTE,       /* Mute before switching to T.38 fax relay */
   CODEC_RED,            /* Redundancy - RFC 2198 */
   CODEC_G722_MODE_1,    /* G.722 Mode 1 64 kbps */
   CODEC_LINPCM128,      /* Narrowband linear PCM @ 128 Kbps */
   CODEC_LINPCM256,      /* Wideband linear PCM @ 256 Kbps */

   /* Maximum number of codec types.  Must be last defined codec, always. */
   CODEC_MAX_TYPES,

   /* Place-holder for dynamic codec types that haven't been mapped yet */
   CODEC_DYNAMIC = (0xffff),

   /* Place-holder for unknown codec types that should be ignored/removed from list */
   CODEC_UNKNOWN = (0xfffe)

} CODEC_TYPE;


/* Voice codec class for checking. */
typedef enum
{
   CODEC_G723x_53,
   CODEC_G723x_63,
   CODEC_G723x_VAR,
   CODEC_G723x,
   CODEC_G726x,
   CODEC_G729x,
   CODEC_ILBCx,
   CODEC_T38x,
   CODEC_PCMx,
   CODEC_VBDx,
   CODEC_NON_VOICE

} CODEC_CLASS;


enum
{
   CODEC_UNSUPPORTED = 0,
   CODEC_SUPPORTED   = 1
};


/* Bit-flags for named telephone event types (RFC 2833) */
enum
{
   /* NTE Events */
   CODEC_NTE_RING    =  (1 << 0),      /* Ringing event */
   CODEC_NTE_ONHOOK  =  (1 << 1),      /* Onhook event */
   CODEC_NTE_OSI     =  (1 << 2),      /* Open switch interval event */
   CODEC_NTE_DTMF0   =  (1 << 3),      /* DTMF Tone 0 */
   CODEC_NTE_DTMF1   =  (1 << 4),      /* DTMF Tone 1 */
   CODEC_NTE_DTMF2   =  (1 << 5),      /* DTMF Tone 2 */
   CODEC_NTE_DTMF3   =  (1 << 6),      /* DTMF Tone 3 */
   CODEC_NTE_DTMF4   =  (1 << 7),      /* DTMF Tone 4 */
   CODEC_NTE_DTMF5   =  (1 << 8),      /* DTMF Tone 5 */
   CODEC_NTE_DTMF6   =  (1 << 9),      /* DTMF Tone 6 */
   CODEC_NTE_DTMF7   =  (1 << 10),     /* DTMF Tone 7 */
   CODEC_NTE_DTMF8   =  (1 << 11),     /* DTMF Tone 8 */
   CODEC_NTE_DTMF9   =  (1 << 12),     /* DTMF Tone 9 */
   CODEC_NTE_DTMFS   =  (1 << 13),     /* DTMF Tone * */
   CODEC_NTE_DTMFH   =  (1 << 14),     /* DTMF Tone # */
   CODEC_NTE_DTMFA   =  (1 << 15),     /* DTMF Tone A */
   CODEC_NTE_DTMFB   =  (1 << 16),     /* DTMF Tone B */
   CODEC_NTE_DTMFC   =  (1 << 17),     /* DTMF Tone C */
   CODEC_NTE_DTMFD   =  (1 << 18)      /* DTMF Tone D */

};


enum
{
   /* Mask for inlcuding all DTMF events */
   CODEC_NTE_DTMF = CODEC_NTE_DTMF0    | \
                    CODEC_NTE_DTMF1    | \
                    CODEC_NTE_DTMF2    | \
                    CODEC_NTE_DTMF3    | \
                    CODEC_NTE_DTMF4    | \
                    CODEC_NTE_DTMF5    | \
                    CODEC_NTE_DTMF6    | \
                    CODEC_NTE_DTMF7    | \
                    CODEC_NTE_DTMF8    | \
                    CODEC_NTE_DTMF9    | \
                    CODEC_NTE_DTMFS    | \
                    CODEC_NTE_DTMFH    | \
                    CODEC_NTE_DTMFA    | \
                    CODEC_NTE_DTMFB    | \
                    CODEC_NTE_DTMFC    | \
                    CODEC_NTE_DTMFD,


   /* Mask for inlcuding all LCS events */
   CODEC_NTE_LCS  = CODEC_NTE_RING   | \
                    CODEC_NTE_ONHOOK | \
                    CODEC_NTE_OSI
};


/* Packetization periods */
typedef enum
{
   CODEC_PTIME_5     = 5,        /* 5 ms  */
   CODEC_PTIME_10    = 10,       /* 10 ms */
   CODEC_PTIME_20    = 20,       /* 20 ms */
   CODEC_PTIME_30    = 30,       /* 30 ms */
   CODEC_PTIME_40    = 40,       /* 40 ms */
   CODEC_PTIME_ANY   = 0xFFFF    /* ptime = '-' */
} CODEC_PKT_PERIOD;


enum
{
   /* Mask for codecs that support all packetization periods */
   CODEC_SUPPORT_ALL_PTIME =  CODEC_PCMU  | \
                              CODEC_PCMA  | \
                              CODEC_G726x | \
                              CODEC_BV16  | \
                              CODEC_BV32  | \
                              CODEC_NTE   | \
                              CODEC_RED
};


/* RTP payload types */
typedef enum
{
   RTP_PAYLOAD_PCMU        = 0,               /* G.711 mu-law 64 kbps   */
   RTP_PAYLOAD_G726_32     = 2,               /* G.726-32               */
   RTP_PAYLOAD_G723        = 4,               /* G.723                  */
   RTP_PAYLOAD_PCMA        = 8,               /* G.711 A-law 64 kbps    */
   RTP_PAYLOAD_CNA         = 13,              /* Comfort noise          */
   RTP_PAYLOAD_G729        = 18,              /* G.729                  */
   RTP_PAYLOAD_CN          = 19,              /* Comfort noise          */
   RTP_PAYLOAD_INVALID     = 0xff
} RTP_PAYLOAD_TYPE;

/* RTP named telephone events, as defined in RFC 2833 */
typedef enum
{
   /* DTMF named events */
   RTP_NTE_DTMF0     = 0,           /* DTMF Tone 0 event */
   RTP_NTE_DTMF1     = 1,           /* DTMF Tone 1 event */
   RTP_NTE_DTMF2     = 2,           /* DTMF Tone 2 event */
   RTP_NTE_DTMF3     = 3,           /* DTMF Tone 3 event */
   RTP_NTE_DTMF4     = 4,           /* DTMF Tone 4 event */
   RTP_NTE_DTMF5     = 5,           /* DTMF Tone 5 event */
   RTP_NTE_DTMF6     = 6,           /* DTMF Tone 6 event */
   RTP_NTE_DTMF7     = 7,           /* DTMF Tone 7 event */
   RTP_NTE_DTMF8     = 8,           /* DTMF Tone 8 event */
   RTP_NTE_DTMF9     = 9,           /* DTMF Tone 9 event */
   RTP_NTE_DTMFS     = 10,          /* DTMF Tone * event */
   RTP_NTE_DTMFH     = 11,          /* DTMF Tone # event */
   RTP_NTE_DTMFA     = 12,          /* DTMF Tone A event */
   RTP_NTE_DTMFB     = 13,          /* DTMF Tone B event */
   RTP_NTE_DTMFC     = 14,          /* DTMF Tone C event */
   RTP_NTE_DTMFD     = 15,          /* DTMF Tone D event */

   /* LCS named events */
   RTP_NTE_RING      = 144,         /* Ringing event */
   RTP_NTE_ONHOOK    = 149,         /* Onhook event */
   RTP_NTE_OSI       = 159,         /* Open switch interval event */

   /* GR303 named events */
   RTP_NTE_RINGOFF   = 149,         /* Ringing off event */
   RTP_NTE_OFFHOOK   = 159          /* Offhook event */

} RTP_NTE;

/* Codec options descriptor */
typedef struct CODECOPT
{
   CODEC_TYPE  rfc2198Type;                  /* If codec type is CODEC_RED, then this is the
                                                redundant codec type. */
   BOS_UINT8   opt1;                         /* Optional data 1 for this codec
                                                   If codec type is CODEC_RED, this is the
                                                      redundancy level.
                                                   If codec type supports VBD (V.152), this is
                                                      the flag for its support. */
   BOS_UINT8   opt2;                         /* Optional data 2 for this codec
                                                   If codec type is CODEC_RED, this is the
                                                      payload type for the redundant codec. */
} CODECOPT;

/* Codec descriptor */
typedef struct CODECDESC
{
   CODEC_TYPE  type;                         /* codec type */
   BOS_UINT8   rtpPayloadType;               /* RTP payload type that corresponds to 'type' */
   CODECOPT    options;                      /* Supplementary codec options. */
} CODECDESC;

/* Codec list */
typedef struct CODECCNXLIST
{
   CODECDESC         codecs[CODEC_MAX_TYPES];   /* list of codecs */
   int               numCodecs;                 /* number of codecs/packetization period in list */
   CODEC_PKT_PERIOD  period[CODEC_MAX_TYPES];   /* list of packetization period associated with each codec */
   int               numPeriods;                /* # of ptime requested */
} CODECCNXLIST;

/* Codec list for a single connection
 *    - consists of 2 sub-lists:
 *       1. local codec list with locally-assigned RTP payload types
 *       2. remote codec list with remotely-assigned RTP payload types
 */
typedef struct CODECCNXPARMLIST
{
   CODECCNXLIST   send;             /* list of local codecs */
   CODECCNXLIST   recv;             /* list of remote codecs */
} CODECCNXPARMLIST;




/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   codecGetBaseFrameRate
**
** PURPOSE:    To retrieve the base frame rate associated a codec type.
**
** PARAMETERS: codec (in)  codec type.
**
** RETURNS:    Native frame rate associated with codec. O if error.
**
** NOTE:
*****************************************************************************
*/
unsigned int codecGetBaseFrameRate( CODEC_TYPE codec );


/*
*****************************************************************************
** FUNCTION:   codecGetBasePayloadSize
**
** PURPOSE:    To retrieve the payload size associated a codec type, running
**             at the base frame rate.
**
** PARAMETERS: codec (in)  codec type.
**
** RETURNS:    Base payload packet size. 0 if error.
**
** NOTE:
*****************************************************************************
*/
unsigned int codecGetBasePayloadSize( CODEC_TYPE codec );


/*
*****************************************************************************
** FUNCTION:   codecGetPayloadSize
**
** PURPOSE:    To retrieve the payload size associated a codec type and a
**             packetization time
**
** PARAMETERS: codec (in)  codec type
**             ptime (in)  packetization time
**
** RETURNS:    Payload packet size. 0 if error.
**
** NOTE:
*****************************************************************************
*/
unsigned int codecGetPayloadSize( CODEC_TYPE codec, unsigned int ptime );


/*
*****************************************************************************
** FUNCTION:   codecCheckClass
**
** PURPOSE:    To check whether a codec belongs to a specific pre-defined
**             class
**
** PARAMETERS: codec (in)  codec type
**             class (in)  class type
**
** RETURNS:    1 if codec belongs to the class. 0 otherwise.
**
** NOTE:
*****************************************************************************
*/
unsigned int codecCheckClass( CODEC_TYPE codec, CODEC_CLASS cclass );


#ifdef __cplusplus
    }
#endif

#endif /* CODEC_H */
