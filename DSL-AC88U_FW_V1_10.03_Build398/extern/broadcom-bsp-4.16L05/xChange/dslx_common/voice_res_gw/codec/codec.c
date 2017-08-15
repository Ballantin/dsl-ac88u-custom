/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
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
*    Filename: codec.c
*
****************************************************************************
*    Description:
*
*     Codec related definitions
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <codec.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* Native codec frame rates. */
#define CODEC_G711_FRAMERATE              5
#define CODEC_G722_MODE_1_FRAMERATE       5
#define CODEC_G7231_FRAMERATE             30
#define CODEC_G726_FRAMERATE              5
#define CODEC_G729_FRAMERATE              10
#define CODEC_G728_FRAMERATE              5
#define CODEC_BV_FRAMERATE                5
#define CODEC_NTE_FRAMERATE               5
#define CODEC_LINPCM_FRAMERATE            5
#define CODEC_ILBC_20_FRAMERATE           20
#define CODEC_ILBC_30_FRAMERATE           30
#define CODEC_T38_FRAMERATE               10
#define CODEC_GSMAMR_FRAMERATE            20
#define CODEC_GSMEFR_FRAMERATE            20
#define CODEC_GSMFR_FRAMERATE             20
#define CODEC_GSMHR_FRAMERATE             20
#define CODEC_AMRWB_FRAMERATE             20

/* Native codec payload size (in BYTES) for various voice encoding formats. */
#define CODEC_G711_PAYLOAD_BYTE           40
#define CODEC_G722_MODE_1_PAYLOAD_BYTE    40
#define CODEC_G7231_63_PAYLOAD_BYTE       24
#define CODEC_G7231_53_PAYLOAD_BYTE       20
#define CODEC_G726_16_PAYLOAD_BYTE        10
#define CODEC_G726_24_PAYLOAD_BYTE        15
#define CODEC_G726_32_PAYLOAD_BYTE        20
#define CODEC_G726_40_PAYLOAD_BYTE        25
#define CODEC_G729_PAYLOAD_BYTE           10
#define CODEC_G729E_PAYLOAD_BYTE          15
#define CODEC_G728_PAYLOAD_BYTE           10
#define CODEC_BV16_PAYLOAD_BYTE           10
#define CODEC_BV32_PAYLOAD_BYTE           20
#define CODEC_G7231_SID_PAYLOAD_BYTE      4
#define CODEC_G729_SID_PAYLOAD_BYTE       2
#define CODEC_LINPCM256_PAYLOAD_BYTE      160
#define CODEC_LINPCM128_PAYLOAD_BYTE      80
#define CODEC_LINPCM64_PAYLOAD_BYTE       40
#define CODEC_NTE_PAYLOAD_BYTE            4
#define CODEC_ILBC_20_PAYLOAD_BYTE        38
#define CODEC_ILBC_30_PAYLOAD_BYTE        50
#define CODEC_GSMAMR_475_PAYLOAD_BYTE     12
#define CODEC_GSMAMR_515_PAYLOAD_BYTE     13
#define CODEC_GSMAMR_590_PAYLOAD_BYTE     15
#define CODEC_GSMAMR_670_PAYLOAD_BYTE     17
#define CODEC_GSMAMR_740_PAYLOAD_BYTE     19
#define CODEC_GSMAMR_795_PAYLOAD_BYTE     20
#define CODEC_GSMAMR_10K_PAYLOAD_BYTE     26
#define CODEC_GSMAMR_12K_PAYLOAD_BYTE     31
#define CODEC_GSMAMR_OA_475_PAYLOAD_BYTE  13
#define CODEC_GSMAMR_OA_515_PAYLOAD_BYTE  14
#define CODEC_GSMAMR_OA_590_PAYLOAD_BYTE  16
#define CODEC_GSMAMR_OA_670_PAYLOAD_BYTE  18
#define CODEC_GSMAMR_OA_740_PAYLOAD_BYTE  20
#define CODEC_GSMAMR_OA_795_PAYLOAD_BYTE  21
#define CODEC_GSMAMR_OA_10K_PAYLOAD_BYTE  27
#define CODEC_GSMAMR_OA_12K_PAYLOAD_BYTE  32
#define CODEC_GSMEFR_PAYLOAD_BYTE         31
#define CODEC_GSMFR_PAYLOAD_BYTE          33
#define CODEC_GSMHR_PAYLOAD_BYTE          15

#define CODEC_AMRWB_2385_PAYLOAD_BYTE     60
#define CODEC_AMRWB_2305_PAYLOAD_BYTE     58
#define CODEC_AMRWB_1985_PAYLOAD_BYTE     50
#define CODEC_AMRWB_1825_PAYLOAD_BYTE     46
#define CODEC_AMRWB_1585_PAYLOAD_BYTE     40
#define CODEC_AMRWB_1425_PAYLOAD_BYTE     36
#define CODEC_AMRWB_1265_PAYLOAD_BYTE     32
#define CODEC_AMRWB_885_PAYLOAD_BYTE      23
#define CODEC_AMRWB_66_PAYLOAD_BYTE       17
#define CODEC_AMRWB_OA_2385_PAYLOAD_BYTE  61
#define CODEC_AMRWB_OA_2305_PAYLOAD_BYTE  59
#define CODEC_AMRWB_OA_1985_PAYLOAD_BYTE  51
#define CODEC_AMRWB_OA_1825_PAYLOAD_BYTE  47
#define CODEC_AMRWB_OA_1585_PAYLOAD_BYTE  41
#define CODEC_AMRWB_OA_1425_PAYLOAD_BYTE  37
#define CODEC_AMRWB_OA_1265_PAYLOAD_BYTE  33
#define CODEC_AMRWB_OA_885_PAYLOAD_BYTE   23
#define CODEC_AMRWB_OA_66_PAYLOAD_BYTE    18

#define CODEC_T38_PAYLOAD_BYTE            52

/* T.38 raw packet size - based on 10ms packet rate */
#define CODEC_T38_RAW_PKT_SIZE            18



typedef struct CODECMAP
{
   CODEC_TYPE  codec;
   VRG_UINT16  pktperiod;
   VRG_UINT16  pktsize;
} CODECMAP;


/* ---- Private Variables ------------------------------------------------ */

/* Lookup table for codec packetization size and packetization period */
static const CODECMAP gCodecMap[] =
{
   { CODEC_PCMU,           CODEC_G711_FRAMERATE,         CODEC_G711_PAYLOAD_BYTE           },
   { CODEC_PCMA,           CODEC_G711_FRAMERATE,         CODEC_G711_PAYLOAD_BYTE           },
   { CODEC_G722_MODE_1,    CODEC_G722_MODE_1_FRAMERATE,  CODEC_G722_MODE_1_PAYLOAD_BYTE    },
   { CODEC_G726_16,        CODEC_G726_FRAMERATE,         CODEC_G726_16_PAYLOAD_BYTE        },
   { CODEC_G726_24,        CODEC_G726_FRAMERATE,         CODEC_G726_24_PAYLOAD_BYTE        },
   { CODEC_G726_32,        CODEC_G726_FRAMERATE,         CODEC_G726_32_PAYLOAD_BYTE        },
   { CODEC_G726_40,        CODEC_G726_FRAMERATE,         CODEC_G726_40_PAYLOAD_BYTE        },
   { CODEC_G7231_53,       CODEC_G7231_FRAMERATE,        CODEC_G7231_53_PAYLOAD_BYTE       },
   { CODEC_G7231_63,       CODEC_G7231_FRAMERATE,        CODEC_G7231_63_PAYLOAD_BYTE       },
   { CODEC_G7231A_53,      CODEC_G7231_FRAMERATE,        CODEC_G7231_53_PAYLOAD_BYTE       },
   { CODEC_G7231A_63,      CODEC_G7231_FRAMERATE,        CODEC_G7231_63_PAYLOAD_BYTE       },
   { CODEC_G729A,          CODEC_G729_FRAMERATE,         CODEC_G729_PAYLOAD_BYTE           },
   { CODEC_G729B,          CODEC_G729_FRAMERATE,         CODEC_G729_PAYLOAD_BYTE           },
   { CODEC_G728,           CODEC_G728_FRAMERATE,         CODEC_G728_PAYLOAD_BYTE           },
   { CODEC_G729,           CODEC_G729_FRAMERATE,         CODEC_G729_PAYLOAD_BYTE           },
   { CODEC_G729E,          CODEC_G729_FRAMERATE,         CODEC_G729E_PAYLOAD_BYTE          },
   { CODEC_BV16,           CODEC_BV_FRAMERATE,           CODEC_BV16_PAYLOAD_BYTE           },
   { CODEC_BV32,           CODEC_BV_FRAMERATE,           CODEC_BV32_PAYLOAD_BYTE           },
   { CODEC_LINPCM64,       CODEC_LINPCM_FRAMERATE,       CODEC_LINPCM64_PAYLOAD_BYTE       },
   { CODEC_LINPCM128,      CODEC_LINPCM_FRAMERATE,       CODEC_LINPCM128_PAYLOAD_BYTE      },
   { CODEC_LINPCM256,      CODEC_LINPCM_FRAMERATE,       CODEC_LINPCM256_PAYLOAD_BYTE      },
   { CODEC_CLEARCHANNEL,   CODEC_LINPCM_FRAMERATE,       CODEC_LINPCM64_PAYLOAD_BYTE       },
   { CODEC_NTE,            CODEC_NTE_FRAMERATE,          CODEC_NTE_PAYLOAD_BYTE            },

   /* For variable rate G7231, we need to account for the maximum packet size.
   ** Since variable rate switches between 5.3 and 6.3 kbps, the maximum packet
   ** size is that of G723.1 6.3 kbps */
   { CODEC_G7231_VAR,      CODEC_G7231_FRAMERATE,        CODEC_G7231_63_PAYLOAD_BYTE       },
   { CODEC_G7231_53_VAR,   CODEC_G7231_FRAMERATE,        CODEC_G7231_63_PAYLOAD_BYTE       },
   { CODEC_G7231_63_VAR,   CODEC_G7231_FRAMERATE,        CODEC_G7231_63_PAYLOAD_BYTE       },

   { CODEC_ILBC_20,        CODEC_ILBC_20_FRAMERATE,      CODEC_ILBC_20_PAYLOAD_BYTE        },
   { CODEC_ILBC_30,        CODEC_ILBC_30_FRAMERATE,      CODEC_ILBC_30_PAYLOAD_BYTE        },

   { CODEC_GSMAMR_12K,     CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_12K_PAYLOAD_BYTE     },
   { CODEC_GSMAMR_10K,     CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_10K_PAYLOAD_BYTE     },
   { CODEC_GSMAMR_795,     CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_795_PAYLOAD_BYTE     },
   { CODEC_GSMAMR_740,     CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_740_PAYLOAD_BYTE     },
   { CODEC_GSMAMR_670,     CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_670_PAYLOAD_BYTE     },
   { CODEC_GSMAMR_590,     CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_590_PAYLOAD_BYTE     },
   { CODEC_GSMAMR_515,     CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_515_PAYLOAD_BYTE     },
   { CODEC_GSMAMR_475,     CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_475_PAYLOAD_BYTE     },
   { CODEC_GSMAMR_OA_12K,  CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_OA_12K_PAYLOAD_BYTE  },
   { CODEC_GSMAMR_OA_10K,  CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_OA_10K_PAYLOAD_BYTE  },
   { CODEC_GSMAMR_OA_795,  CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_OA_795_PAYLOAD_BYTE  },
   { CODEC_GSMAMR_OA_740,  CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_OA_740_PAYLOAD_BYTE  },
   { CODEC_GSMAMR_OA_670,  CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_OA_670_PAYLOAD_BYTE  },
   { CODEC_GSMAMR_OA_590,  CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_OA_590_PAYLOAD_BYTE  },
   { CODEC_GSMAMR_OA_515,  CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_OA_515_PAYLOAD_BYTE  },
   { CODEC_GSMAMR_OA_475,  CODEC_GSMAMR_FRAMERATE,       CODEC_GSMAMR_OA_475_PAYLOAD_BYTE  },

   { CODEC_GSMEFR,         CODEC_GSMEFR_FRAMERATE,       CODEC_GSMEFR_PAYLOAD_BYTE         },
   { CODEC_GSMFR,          CODEC_GSMFR_FRAMERATE,        CODEC_GSMFR_PAYLOAD_BYTE          },
   { CODEC_GSMHR,          CODEC_GSMHR_FRAMERATE,        CODEC_GSMHR_PAYLOAD_BYTE          },

   { CODEC_AMRWB_2385,     CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_2385_PAYLOAD_BYTE     },
   { CODEC_AMRWB_2305,     CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_2305_PAYLOAD_BYTE     },
   { CODEC_AMRWB_1985,     CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_1985_PAYLOAD_BYTE     },
   { CODEC_AMRWB_1825,     CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_1825_PAYLOAD_BYTE     },
   { CODEC_AMRWB_1585,     CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_1585_PAYLOAD_BYTE     },
   { CODEC_AMRWB_1425,     CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_1425_PAYLOAD_BYTE     },
   { CODEC_AMRWB_1265,     CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_1265_PAYLOAD_BYTE     },
   { CODEC_AMRWB_885,      CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_885_PAYLOAD_BYTE      },
   { CODEC_AMRWB_66,       CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_66_PAYLOAD_BYTE       },
   { CODEC_AMRWB_OA_2385,  CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_OA_2385_PAYLOAD_BYTE  },
   { CODEC_AMRWB_OA_2305,  CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_OA_2305_PAYLOAD_BYTE  },
   { CODEC_AMRWB_OA_1985,  CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_OA_1985_PAYLOAD_BYTE  },
   { CODEC_AMRWB_OA_1825,  CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_OA_1825_PAYLOAD_BYTE  },
   { CODEC_AMRWB_OA_1585,  CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_OA_1585_PAYLOAD_BYTE  },
   { CODEC_AMRWB_OA_1425,  CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_OA_1425_PAYLOAD_BYTE  },
   { CODEC_AMRWB_OA_1265,  CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_OA_1265_PAYLOAD_BYTE  },
   { CODEC_AMRWB_OA_885,   CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_OA_885_PAYLOAD_BYTE   },
   { CODEC_AMRWB_OA_66,    CODEC_AMRWB_FRAMERATE,        CODEC_AMRWB_OA_66_PAYLOAD_BYTE    },

   { CODEC_T38,            CODEC_T38_FRAMERATE,          CODEC_T38_PAYLOAD_BYTE            },
   { CODEC_T38_MUTE,       CODEC_T38_FRAMERATE,          CODEC_T38_PAYLOAD_BYTE            },

   /* Redundancy codec.  Currently we would only support redundancy for PCM codecs
      and in the case of voice band data using V.152.  The base framerate and payload
      byte sizes for RED is therefore the same as for PCM (G.711) with a multiplicative
      factor and a small overhead depending on the redundancy level being used. */
   { CODEC_RED,            CODEC_G711_FRAMERATE,         CODEC_G711_PAYLOAD_BYTE           },

   { CODEC_NULL,           0,                            0                                 }
};


/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


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
unsigned int codecGetBaseFrameRate( CODEC_TYPE codec )
{
   const CODECMAP *codecMap = gCodecMap;

   /* Lookup the current codec map */
   while ( codecMap->codec != CODEC_NULL )
   {
      if ( codecMap->codec == codec )
      {
         return ( codecMap->pktperiod );
      }

      codecMap++;
   }


   /* Error. */
   return ( 0 );
}


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
unsigned int codecGetBasePayloadSize( CODEC_TYPE codec )
{
   const CODECMAP *codecMap = gCodecMap;

   /* Lookup the current codec map */
   while ( codecMap->codec != CODEC_NULL )
   {
      if ( codecMap->codec == codec )
      {
         return ( codecMap->pktsize );
      }

      codecMap++;
   }


   /* Error. */
   return ( 0 );
}


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
unsigned int codecGetPayloadSize( CODEC_TYPE codec, unsigned int ptime )
{
   unsigned int   numPkt;
   unsigned int   pktSize;
   unsigned int   baseFrameSize;
   unsigned int   baseFrameRate;

   baseFrameRate = codecGetBaseFrameRate( codec );
   baseFrameSize = codecGetBasePayloadSize( codec );

   if ( ( baseFrameRate == 0 ) || ( baseFrameSize == 0 ) )
   {
      return ( 0 );
   }

   numPkt = ptime / baseFrameRate;

   /* Special handling for T.38 */
   if ( codec == CODEC_T38 || codec == CODEC_T38_MUTE )
   {
      /* Need to manually calculate the T.38 packet size due to common pkt hdrs */

      /* Determine the number of raw packets */
      pktSize = CODEC_T38_RAW_PKT_SIZE * numPkt;

      /* Add IFP header */
      pktSize += 6;

      /* Add redundancy of 1 */
      pktSize *= 2;

      /* Add error recovery and redundant packet header */
      pktSize += 2;

      /* Add UDPTL header */
      pktSize += 2;
   }
   else
   {
      pktSize = numPkt * baseFrameSize;
   }

   return ( pktSize );
}

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
unsigned int codecCheckClass( CODEC_TYPE codec, CODEC_CLASS cclass )
{
   int ret = 0;

   switch( cclass )
   {
      case CODEC_G723x_53:
      {
         if( (codec == CODEC_G7231_53) ||
             (codec == CODEC_G7231A_53) )
         {
            ret = 1;
         }
      }
      break;

      case CODEC_G723x_63:
      {
         if( (codec == CODEC_G7231_63) ||
             (codec == CODEC_G7231A_63) )
         {
            ret = 1;
         }
      }
      break;

      case CODEC_G723x_VAR:
      {
         if( (codec == CODEC_G7231_VAR) ||
             (codec == CODEC_G7231_53_VAR) ||
             (codec == CODEC_G7231_63_VAR) )
         {
            ret = 1;
         }
      }
      break;

      case CODEC_G723x:
      {
         if( (codec == CODEC_G7231_53) ||
             (codec == CODEC_G7231A_53) ||
             (codec == CODEC_G7231_63) ||
             (codec == CODEC_G7231A_63) ||
             (codec == CODEC_G7231_VAR) ||
             (codec == CODEC_G7231_53_VAR) ||
             (codec == CODEC_G7231_63_VAR) )
         {
            ret = 1;
         }
      }
      break;

      case CODEC_G726x:
      {
         if( (codec == CODEC_G726_16) ||
             (codec == CODEC_G726_24) ||
             (codec == CODEC_G726_32) ||
             (codec == CODEC_G726_40) )
         {
            ret = 1;
         }
      }
      break;

      case CODEC_G729x:
      {
         if( (codec == CODEC_G729A) ||
             (codec == CODEC_G729B) ||
             (codec == CODEC_G729) ||
             (codec == CODEC_G729E) )
         {
            ret = 1;
         }
      }
      break;

      case CODEC_ILBCx:
      {
         if( (codec == CODEC_ILBC_20) ||
             (codec == CODEC_ILBC_30) )
         {
            ret = 1;
         }
      }
      break;

      case CODEC_T38x:
      {
         if( (codec == CODEC_T38) ||
             (codec == CODEC_T38_MUTE) )
         {
            ret = 1;
         }
      }
      break;

      case CODEC_PCMx:
      {
         if( (codec == CODEC_PCMU) ||
             (codec == CODEC_PCMA) )
         {
            ret = 1;
         }
      }
      break;

      case CODEC_VBDx:
      {
         if( (codec == CODEC_PCMU) ||
             (codec == CODEC_PCMA) )
         {
            ret = 1;
         }
      }
      break;

      case CODEC_AMRx:
      {
         if( (codec == CODEC_GSMAMR_12K) ||
             (codec == CODEC_GSMAMR_10K) ||
             (codec == CODEC_GSMAMR_795) ||
             (codec == CODEC_GSMAMR_740) ||
             (codec == CODEC_GSMAMR_670) ||
             (codec == CODEC_GSMAMR_590) ||
             (codec == CODEC_GSMAMR_515) ||
             (codec == CODEC_GSMAMR_475) ||
             (codec == CODEC_AMRWB_66) ||
             (codec == CODEC_AMRWB_885) ||
             (codec == CODEC_AMRWB_1265) ||
             (codec == CODEC_AMRWB_1425) ||
             (codec == CODEC_AMRWB_1585) ||
             (codec == CODEC_AMRWB_1825) ||
             (codec == CODEC_AMRWB_1985) ||
             (codec == CODEC_AMRWB_2305) ||
             (codec == CODEC_AMRWB_2385))
         {
            ret = 1;
         }
      }
      break;

      case CODEC_NON_VOICE:
      {
         if( (codec == CODEC_T38) ||
             (codec == CODEC_T38_MUTE) ||
             (codec == CODEC_NTE) ||
             (codec == CODEC_RED) )
         {
            ret = 1;
         }
      }
      break;

      default:
      break;
   }

   return ( ret );
}
