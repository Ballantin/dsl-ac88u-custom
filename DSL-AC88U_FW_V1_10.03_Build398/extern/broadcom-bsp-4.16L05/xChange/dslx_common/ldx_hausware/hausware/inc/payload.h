/****************************************************************************
*
*     Copyright (c) 1995- 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    payload.h
*
*  @brief   This file contains the API definitions for the NETVHD payloads.
*
****************************************************************************/
#ifndef PAYLOAD_H
#define PAYLOAD_H

/* ---- Include Files ---------------------------------------------------- */

#ifndef HAPI_TYPES_H
#include <hapi_types.h>
#endif
#if !defined( HAPI_PKT_H )
#  include<hapi_pkt.h>
#endif
#if !defined( HAPI_WRAP_DEFS_H )
#  include<hapi_wrap_defs.h>
#endif

/* ---- Constants and Types ---------------------------------------------- */

/* G.711 Rates */

typedef enum
{
   G711_MULAW64KBPS,                   /* 64 kbps mu-law PCM */
   G711_ALAW64KBPS,                    /* 64 kbps A-law PCM */
   G711_MULAW128KBPS,                  /* 128 kbps mu-law PCM */
   G711_ALAW128KBPS,                   /* 128 kbps A-law PCM */
   G711_NUMRATES
} HSEG711RATE;

/* G.726 Rates */

typedef enum
{
   G726_16KBPS,                        /* 16 kbps ADPCM */
   G726_24KBPS,                        /* 24 kbps ADPCM */
   G726_32KBPS,                        /* 32 kbps ADPCM */
   G726_40KBPS,                        /* 40 kbps ADPCM */
   G726_NUMRATES
} HSEG726RATE;


/* G.729 Rates */

typedef enum
{
   G729_8KBPS_A,                       /* 8 kbps (using Annex A) */
   G729_8KBPS,                         /* 8 kbps (original) */
   G729_118KBPS,                       /* 11.8 kbps (using Annex E) */
   G729_NUMRATES
} HSEG729RATE;

/* G.723.1 Rates */

typedef enum
{
   G7231_5_3KBPS,                      /* 5.3 kbps */
   G7231_6_3KBPS,                      /* 6.3 kbps */
   G7231_NUMRATES
} HSEG7231RATE;

/* G.728 Rates */
typedef enum
{
   G728_16KBPS,                        /* 16 kbps */
   G728_NUMRATES
} HSEG728RATE;

 
/* BroadVoice Rates */
typedef enum
{
   BV_NB16KBPS,                        /* 16 kbps */
   BV_WB32KBPS,                        /* 32 kbps */
   BV_NUMRATES
} HSEBVRATE;

/***************************************************************************
** Voice rate codes
****************************************************************************/

/*
                  high byte                     low byte
   +-------------------------------+-------------------------------+
   |       VAD Type                |   Payload voice code          |
   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
    15  14  13  12  11  10  9   8   7   6   5   4   3   2   1   0

 
 */
 
typedef enum
{
   /* no VAD */
   HAPI_PKT_VC_G711_ULAW_NOVAD     = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G711_ULAW )      ,     /* G711 - uLaw - 64 kbps */
   HAPI_PKT_VC_G711_ALAW_NOVAD     = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G711_ALAW )      ,     /* G711 - aLaw - 64 kbps */
   HAPI_PKT_VC_G726_16_NOVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G726_16 )        ,     /* G.726 - 16 kbps */
   HAPI_PKT_VC_G726_24_NOVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G726_24 )        ,     /* G.726 - 24 kbps */
   HAPI_PKT_VC_G726_32_NOVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G726_32 )        ,     /* G.726 - 32 kbps */
   HAPI_PKT_VC_G726_40_NOVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G726_40 )        ,     /* G.726 - 40 kbps */
   HAPI_PKT_VC_G729A_8_NOVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G729A_8 )        ,     /* G.729(A) - 8 kbps */
   HAPI_PKT_VC_G729_8_NOVAD        = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G729_8 )         ,     /* G.729  - 8 kbps */
   HAPI_PKT_VC_G729_118_NOVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G729_118 )       ,     /* G.729E - 11.8 kbps */
   HAPI_PKT_VC_G7231_53_NOVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G7231_53 )       ,     /* G.723.1 - 5.3 kbps */
   HAPI_PKT_VC_G7231_63_NOVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G7231_63 )       ,     /* G.723.1 - 6.3 kbps */
   HAPI_PKT_VC_G728_16_NOVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G728_16 )        ,     /* G.728 - 16 kbps */
   HAPI_PKT_VC_BV16_NOVAD          = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_BV16 )           ,     /* BroadVoice - 16 kbps */
   HAPI_PKT_VC_LINPCM128_NOVAD     = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_LINPCM128 )      ,     /* Linear PCM - 128 kbps */
   HAPI_PKT_VC_ILBC20_NOVAD        = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_ILBC20 )         ,     /* iLBC 20 - 15.2 kbps */
   HAPI_PKT_VC_ILBC30_NOVAD        = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_ILBC30 )         ,     /* iLBC 30 - 13.3 kbps */
   HAPI_PKT_VC_G711WB_ULAW_NOVAD   = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G711WB_ULAW )    ,     /* G711 wideband - uLaw - 128 kbps */
   HAPI_PKT_VC_G711WB_ALAW_NOVAD   = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G711WB_ALAW )    ,     /* G711 wideband - aLaw - 128 kbps */
   HAPI_PKT_VC_BV32_NOVAD          = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_BV32 )           ,     /* BroadVoice Wide Band 32 kbps */
   HAPI_PKT_VC_LINPCM256_NOVAD     = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_LINPCM256 )      ,     /* Linear PCM - 256 kbps */
   HAPI_PKT_VC_G722_64_MODE_1_NOVAD= HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G722_64_MODE_1 ) ,     /* G.722 - 64 kbps, mode 1 */
   HAPI_PKT_VC_G722_64_MODE_2_NOVAD= HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G722_64_MODE_2 ) ,     /* G.722 - 64 kbps, mode 2 */
   HAPI_PKT_VC_G722_64_MODE_3_NOVAD= HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G722_64_MODE_3 ) ,     /* G.722 - 64 kbps, mode 3 */
   HAPI_PKT_VC_G7221_24_NOVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G7221_24 )       ,     /* G.722.1 - 24 kbps */
   HAPI_PKT_VC_G7221_32_NOVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_G7221_32 )       ,     /* G.722.1 - 32 kbps */
   HAPI_PKT_VC_LINPCM64_NOVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_NONE, HAPI_PKT_VC_LINPCM64 )       ,     /* Linear PCM - 64 kbps */
  /* generic VAD */
   HAPI_PKT_VC_G711_ULAW_GVAD     = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G711_ULAW )     ,    /* G711 - uLaw - 64 kbps */
   HAPI_PKT_VC_G711_ALAW_GVAD     = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G711_ALAW )     ,    /* G711 - aLaw - 64 kbps */
   HAPI_PKT_VC_G726_16_GVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G726_16 )       ,    /* G.726 - 16 kbps */
   HAPI_PKT_VC_G726_24_GVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G726_24 )       ,    /* G.726 - 24 kbps */
   HAPI_PKT_VC_G726_32_GVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G726_32 )       ,    /* G.726 - 32 kbps */
   HAPI_PKT_VC_G726_40_GVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G726_40 )       ,    /* G.726 - 40 kbps */
   HAPI_PKT_VC_G729A_8_GVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G729A_8 )       ,    /* G.729(A) - 8 kbps */
   HAPI_PKT_VC_G729_8_GVAD        = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G729_8 )        ,    /* G.729  - 8 kbps */
   HAPI_PKT_VC_G729_118_GVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G729_118 )      ,    /* G.729E - 11.8 kbps */
   HAPI_PKT_VC_G7231_53_GVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G7231_53 )      ,    /* G.723.1 - 5.3 kbps */
   HAPI_PKT_VC_G7231_63_GVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G7231_63 )      ,    /* G.723.1 - 6.3 kbps */
   HAPI_PKT_VC_G728_16_GVAD       = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G728_16 )       ,    /* G.728 - 16 kbps */
   HAPI_PKT_VC_BV16_GVAD          = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_BV16 )          ,    /* BroadVoice - 16 kbps */
   HAPI_PKT_VC_LINPCM128_GVAD     = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_LINPCM128 )     ,    /* Linear PCM - 128 kbps */
   HAPI_PKT_VC_ILBC20_GVAD        = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_ILBC20 )        ,    /* iLBC 20 - 15.2 kbps */
   HAPI_PKT_VC_ILBC30_GVAD        = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_ILBC30 )        ,    /* iLBC 30 - 13.3 kbps */
   HAPI_PKT_VC_G711WB_ULAW_GVAD   = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G711WB_ULAW )   ,    /* G711 wideband - uLaw - 128 kbps */
   HAPI_PKT_VC_G711WB_ALAW_GVAD   = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G711WB_ALAW )   ,    /* G711 wideband - aLaw - 128 kbps */
   HAPI_PKT_VC_BV32_GVAD          = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_BV32)           ,    /* BroadVoice Wide Band 32 kbps */
   HAPI_PKT_VC_LINPCM256_GVAD     = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_LINPCM256 )     ,    /* Linear PCM - 256 kbps */
   HAPI_PKT_VC_G722_64_MODE_1_GVAD= HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G722_64_MODE_1 ),    /* G.722 - 64 kbps, mode 1 */
   HAPI_PKT_VC_G722_64_MODE_2_GVAD= HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G722_64_MODE_2 ),    /* G.722 - 64 kbps, mode 2 */
   HAPI_PKT_VC_G722_64_MODE_3_GVAD= HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G722_64_MODE_3 ),    /* G.722 - 64 kbps, mode 3 */
   HAPI_PKT_VC_G7221_24_GVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G7221_24 )      ,    /* G.722.1 - 24 kbps */
   HAPI_PKT_VC_G7221_32_GVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_G7221_32 )      ,    /* G.722.1 - 32 kbps */
   HAPI_PKT_VC_LINPCM64_GVAD      = HAPI_PKT_MKPID(HAPI_VAD_TYPE_GENERIC, HAPI_PKT_VC_LINPCM64 )      ,    /* Linear PCM - 64 kbps */
  /* build in vad */
   HAPI_PKT_VC_G729A_8_ANNEX_B_VAD  = HAPI_PKT_MKPID(HAPI_VAD_TYPE_BUILTIN,HAPI_PKT_VC_G729A_8 )        ,     /* G.729(A) - 8 kbps */
   HAPI_PKT_VC_G7231_53_ANNEX_A_GVAD = HAPI_PKT_MKPID(HAPI_VAD_TYPE_BUILTIN,HAPI_PKT_VC_G7231_53)       ,     /* G.723.1 - 5.3 kbps */
   HAPI_PKT_VC_G7231_53__ANNEX_A__VAD = HAPI_PKT_MKPID(HAPI_VAD_TYPE_BUILTIN, HAPI_PKT_VC_G7231_63)     ,     /* G.723.1 - 6.3 kbps */


} HAPI_PKT_VOICERATE;
 
/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif      /* PAYLOAD_H */


 
