/****************************************************************************
*
* <:copyright-BRCM:2014:DUAL/GPL:standard
* 
*    Copyright (c) 2014 Broadcom
*    All Rights Reserved
* 
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed
* to you under the terms of the GNU General Public License version 2
* (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
* with the following added to such license:
* 
*    As a special exception, the copyright holders of this software give
*    you permission to link this software with independent modules, and
*    to copy and distribute the resulting executable under terms of your
*    choice, provided that you also meet, for each linked independent
*    module, the terms and conditions of the license of that module.
*    An independent module is a module which is not derived from this
*    software.  The special exception does not apply to any modifications
*    of the software.
* 
* Not withstanding the above, under no circumstances may you combine
* this software in any way with any other Broadcom software provided
* under a license other than the GPL, without Broadcom's express prior
* written consent.
* 
* :>
*
****************************************************************************
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

#include <vrgTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

/* Voice codec types */
typedef enum
{
   CODEC_NULL           = (0),        /* NULL */
   CODEC_PCMU,   /* G.711 ulaw */
   CODEC_PCMA,   /* G.711 alaw */
   CODEC_G726_16,   /* G.726 - 16 kbps */
   CODEC_G726_24,   /* G.726 - 24 kbps */
   CODEC_G726_32,   /* G.726 - 32 kbps */
   CODEC_G726_40,   /* G.726 - 40 kbps */
   CODEC_G7231_53,   /* G.723.1 - 5.3 kbps */
   CODEC_G7231_63,   /* G.723.1 - 6.3 kbps */
   CODEC_G7231A_53,   /* G.723.1A - 5.3 kbps */
   CODEC_G7231A_63,   /* G.723.1A - 6.3 kbps */
   CODEC_G729A,  /* G.729A */
   CODEC_G729B,  /* G.729B */
   CODEC_G711_LINEAR,  /* Linear media queue data */
   CODEC_G728,  /* G.728 */
   CODEC_G729,  /* G.729 */
   CODEC_G729E,  /* G.729E */
   CODEC_BV16,  /* BRCM Broadvoice - 16 kbps */
   CODEC_BV32,  /* BRCM Broadvoice - 32 kbps */
   CODEC_NTE,  /* Named telephone events */
   CODEC_ILBC_20,  /* iLBC speech coder - 20 ms frame / 15.2 kbps */
   CODEC_ILBC_30,  /* iLBC speech coder - 30 ms frame / 13.3 kbps */
   CODEC_G7231_53_VAR, /* G723.1 variable rates (preferred=5.3) */
   CODEC_G7231_63_VAR, /* G723.1 variable rates (preferred=6.3) */
   CODEC_G7231_VAR, /* G723.1 variable rates */
   CODEC_T38,  /* T.38 fax relay */
   CODEC_T38_MUTE,  /* Mute before switching to T.38 fax relay */
   CODEC_RED,  /* Redundancy - RFC 2198 */
   CODEC_G722_MODE_1,  /* G.722 Mode 1 64 kbps */
   CODEC_LINPCM64,   /* 8-bit linear PCM @ 64 Kbps */
   CODEC_LINPCM128,  /* Narrowband linear PCM @ 128 Kbps */
   CODEC_LINPCM256,  /* Wideband linear PCM @ 256 Kbps */
   CODEC_CLEARCHANNEL, /* Clear-channel digital @ 64 Kbps */

   CODEC_GSMAMR_12K,  /* GSM AMR codec @ 12.2 kbps */
   CODEC_GSMAMR_10K,  /* GSM AMR codec @ 10.2 kbps */
   CODEC_GSMAMR_795,  /* GSM AMR codec @ 7.95 kbps */
   CODEC_GSMAMR_740,  /* GSM AMR codec @ 7.4 kbps */
   CODEC_GSMAMR_670,  /* GSM AMR codec @ 6.7 kbps */
   CODEC_GSMAMR_590,  /* GSM AMR codec @ 5.9 kbps */
   CODEC_GSMAMR_515,  /* GSM AMR codec @ 5.15 kbps */
   CODEC_GSMAMR_475,  /* GSM AMR codec @ 4.75 kbps */
   CODEC_GSMAMR_OA_12K,  /* GSM AMR OA codec @ 12.2 kbps */
   CODEC_GSMAMR_OA_10K,  /* GSM AMR OA codec @ 10.2 kbps */
   CODEC_GSMAMR_OA_795,  /* GSM AMR OA codec @ 7.95 kbps */
   CODEC_GSMAMR_OA_740,  /* GSM AMR OA codec @ 7.4 kbps */
   CODEC_GSMAMR_OA_670,  /* GSM AMR OA codec @ 6.7 kbps */
   CODEC_GSMAMR_OA_590,  /* GSM AMR OA codec @ 5.9 kbps */
   CODEC_GSMAMR_OA_515,  /* GSM AMR OA codec @ 5.15 kbps */
   CODEC_GSMAMR_OA_475,  /* GSM AMR OA codec @ 4.75 kbps */

   CODEC_GSMEFR, /* GSM EFR codec @ 12.2 kbps */
   CODEC_GSMFR,  /* GSM FR codec @ 13.2 kbps */
   CODEC_GSMHR,  /* GSM HR codec @ 5.6 kbps */

   CODEC_AMRWB_66,    /* AMR WB codec @ 6.6 kbps */
   CODEC_AMRWB_885,   /* AMR WB codec @ 8.85 kbps */
   CODEC_AMRWB_1265,  /* AMR WB codec @ 12.65 kbps */
   CODEC_AMRWB_1425,  /* AMR WB codec @ 14.25 kbps */
   CODEC_AMRWB_1585,  /* AMR WB codec @ 15.85 kbps */
   CODEC_AMRWB_1825,  /* AMR WB codec @ 18.25 kbps */
   CODEC_AMRWB_1985,  /* AMR WB codec @ 19.85 kbps */
   CODEC_AMRWB_2305,  /* AMR WB codec @ 23.05 kbps */
   CODEC_AMRWB_2385,  /* AMR WB codec @ 23.85 kbps */
   CODEC_AMRWB_OA_66,    /* AMR WB octet-aligned codec @ 6.6 kbps */
   CODEC_AMRWB_OA_885,   /* AMR WB octet-aligned codec @ 8.85 kbps */
   CODEC_AMRWB_OA_1265,  /* AMR WB octet-aligned codec @ 12.65 kbps */
   CODEC_AMRWB_OA_1425,  /* AMR WB octet-aligned codec @ 14.25 kbps */
   CODEC_AMRWB_OA_1585,  /* AMR WB octet-aligned codec @ 15.85 kbps */
   CODEC_AMRWB_OA_1825,  /* AMR WB octet-aligned codec @ 18.25 kbps */
   CODEC_AMRWB_OA_1985,  /* AMR WB octet-aligned codec @ 19.85 kbps */
   CODEC_AMRWB_OA_2305,  /* AMR WB octet-aligned codec @ 23.05 kbps */
   CODEC_AMRWB_OA_2385,  /* AMR WB octet-aligned codec @ 23.85 kbps */

   /* Maximum number of codec types. */
   CODEC_MAX_TYPES,

   /* Place-holder for dynamic codec types that haven't been mapped yet */
   CODEC_DYNAMIC        = (0xffff),

   /* Place-holder for unknown codec types that should be ignored/removed from list */
   CODEC_UNKNOWN        = (0xfffe)
} CODEC_TYPE;

/* Voice codec class for checking. */
typedef enum
{
   CODEC_G722x,
   CODEC_G723x_53,
   CODEC_G723x_63,
   CODEC_G723x_VAR,
   CODEC_G723x,
   CODEC_G726x,
   CODEC_G729x,
   CODEC_ILBCx,
   CODEC_AMRWBx,
   CODEC_AMRWBx_OA,
   CODEC_T38x,
   CODEC_PCMx,
   CODEC_VBDx,
   CODEC_WIDEBAND,
   CODEC_AMRx,
   CODEC_AMRx_OA,
   CODEC_ALL_PTIME,
   CODEC_NON_10MS,
   CODEC_NON_VOICE,

   CODEC_NULL_CLASS

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

/* RTP payload types */
typedef enum
{
   RTP_PAYLOAD_PCMU        = 0,               /* G.711 mu-law 64 kbps   */
   RTP_PAYLOAD_G726_32     = 2,               /* G.726-32               */
   RTP_PAYLOAD_GSM         = 3,               /* GSM Full Rate          */
   RTP_PAYLOAD_G723        = 4,               /* G.723                  */
   RTP_PAYLOAD_PCMA        = 8,               /* G.711 A-law 64 kbps    */
   RTP_PAYLOAD_G722        = 9,               /* G.722                  */
   RTP_PAYLOAD_CNA         = 13,              /* Comfort noise          */
   RTP_PAYLOAD_G728        = 15,              /* G.728                  */
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
   VRG_UINT8   opt1;                         /* Optional data 1 for this codec
                                                   If codec type is CODEC_RED, this is the
                                                      redundancy level.
                                                   If codec type supports VBD (V.152), this is
                                                      the flag for its support. */
   VRG_UINT8   opt2;                         /* Optional data 2 for this codec
                                                   If codec type is CODEC_RED, this is the
                                                      payload type for the redundant codec. */
} CODECOPT;

/* Codec descriptor */
typedef struct CODECDESC
{
   CODEC_TYPE  type;                         /* codec type */
   VRG_UINT8   rtpPayloadType;               /* RTP payload type that corresponds to 'type' */
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
