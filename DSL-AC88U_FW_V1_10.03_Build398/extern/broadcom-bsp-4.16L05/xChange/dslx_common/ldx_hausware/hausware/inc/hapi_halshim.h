/***************************************************************************
*
* Copyright (c) 2003 Broadcom, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom
* company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*
*      HAL Shim API
*
****************************************************************************/

#ifndef HAPI_HALSHIM_H
#define HAPI_HALSHIM_H

/* ---- Include Files ---------------------------------------------------- */

#ifndef HAPI_TYPES_H
#include <hapi_types.h>                /* HAPI environment definitions     */
#endif

#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Hausware API object ids          */
#endif

#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>               /* Hausware generic cmds/events     */
#endif

/* ---- Constants and Types ---------------------------------------------- */
/* The following are statitics kept for each HAL Shim device
 */
typedef struct
{
   HAPI_UINT16 currIbdPackets;  /* Current number of IBD packets in queue  */
   HAPI_UINT16 maxIbdPackets;   /* Maximum bumber of IBD packets in queue  */
   HAPI_UINT16 underrunCount;   /* Underrun count in ms                    */

} HAPI_HALSHIMSTATS;

/* Mode of the HAL device */
typedef enum 
{
   HALSHIM_MODE_START,
   HALSHIM_NARROWBAND_MODE,
   HALSHIM_WIDEBAND_MODE,
   HALSHIM_MODE_MAX
   
} HALSHIM_MODE;

/* PCM Compression Modes */
typedef enum 
{
   HALSHIM_PCM_COMP_START = -1,
   HALSHIM_PCM_COMP_ULAW,
   HALSHIM_PCM_COMP_ALAW,
   HALSHIM_PCM_COMP_NONE,
   HALSHIM_PCM_COMP_MAX,
   
} HALSHIM_PCM_COMP_MODE;

typedef enum
{
   HAL_SHIM_CONFIG_IDLE = 0,
   HAL_SHIM_CONFIG_AUDIO = 1,
   HAL_SHIM_CONFIG_VIDEO = 2,
   HAL_SHIM_CONFIG_AUDIO_SPECIAL = 3,
   HAL_SHIM_CONFIG_AUDIO_IDECT = 4,
   HAL_SHIM_CONFIG_MAX

} HALSHIM_CONFIG_FLAG;

/* Sample size in bits */
typedef enum
{
   HALSHIM_SAMPLE_SIZE_START = -1,
   HALSHIM_SAMPLE_SIZE_8BITS = 1,   /* Do not change the values!! */
   HALSHIM_SAMPLE_SIZE_16BITS,
   HALSHIM_SAMPLE_SIZE_MAX   
   
} HALSHIM_SAMPLE_SIZE;

/* Number of DMA channels per HAL device */
typedef enum
{
   HALSHIM_PCM_CHANNELS_START = 0,
   HALSHIM_PCM_CHANNELS_ONE,        /* Do not change the values!! */
   HALSHIM_PCM_CHANNELS_TWO,
   HALSHIM_PCM_CHANNELS_NONE,
   HALSHIM_PCM_CHANNELS_MAX
   
} HALSHIM_PCM_CHANNELS; 

/* Channel mute options */
typedef enum
{
   HALSHIM_PCM_CHANNEL_MUTE_START = -1,
   HALSHIM_PCM_CHANNEL_MUTE_OFF,
   HALSHIM_PCM_CHANNEL_MUTE_ON,
   HALSHIM_PCM_CHANNEL_MUTE_MAX
   
} HALSHIM_PCM_CHANNEL_MUTE;

typedef enum
{
   HALSHIM_BYTE_ORDER_START = -1,
   HALSHIM_BYTE_ORDER_BIG_ENDIAN,   
   HALSHIM_BYTE_ORDER_LITTLE_ENDIAN,   
   HALSHIM_BYTE_ORDER_MAX
   
} HALSHIM_BYTE_ORDER;

/***************************************************************************
**        HAL SHIM COMMANDS
****************************************************************************/

/* Enable/disable a HAL Shim Device */
#define HAPI_HALSHIM_ENABLE_CMD          (HSF_MKHDR(HAPI_HALSHIM_SRV, HAPI_OPC_ENABLE_CMD))
#define HAPI_HALSHIM_ENABLE_RSP          (HSF_MKRSP_STD(HAPI_HALSHIM_ENABLE_CMD))
      /* OP1: TRUE to enable; FALSE to disable                          */
      /* OP2: Reserved                                                  */

/* Flush IBD Queue */
#define HAPI_HALSHIM_FLUSH_CMD           (HSF_MKCMD_STD(HAPI_HALSHIM_SRV, 1))
#define HAPI_HALSHIM_FLUSH_RSP           (HSF_MKRSP_STD(HAPI_HALSHIM_FLUSH_CMD))
      /* OP1: Reserved                                                  */
      /* OP2: Reserved                                                  */

/* Get Statistics */
#define HAPI_HALSHIM_GETSTATS_CMD        (HSF_MKCMD_EXT(HAPI_HALSHIM_SRV, 2))
#define HAPI_HALSHIM_GETSTATS_RSP        (HSF_MKRSP_EXT(HAPI_HALSHIM_GETSTATS_CMD))
      /* OP1: Length in bytes of HAPI_HALSHIMSTATS                               */
      /* OP2: Pointer to a buffer of size HAPI_HALSHIMSTATS                      */

/* Reset statistics */
#define HAPI_HALSHIM_RESET_STATS_CMD     (HSF_MKHDR(HAPI_HALSHIM_SRV, HAPI_OPC_RESET_STATS_CMD))
#define HAPI_HALSHIM_RESET_STATS_RSP     (HSF_MKRSP_STD(HAPI_HALSHIM_RESET_STATS_CMD))
      /* OP1: Reserved                                                  */
      /* OP2: Reserved                                                  */

/* Flush IBD Queue */
#define HAPI_HALSHIM_BUILDOUT_CMD        (HSF_MKCMD_STD(HAPI_HALSHIM_SRV, 3))
#define HAPI_HALSHIM_BUILDOUT_RSP        (HSF_MKRSP_STD(HAPI_HALSHIM_BUILDOUT_CMD))
      /* OP1: build out in number of samples (8Khz)                     */
      /* OP2: Reserved                                                  */
/* Configure the HAL device */
#define HAPI_HALSHIM_CONFIG_CMD          (HSF_MKCMD_STD(HAPI_HALSHIM_SRV, 4))
#define HAPI_HALSHIM_CONFIG_RSP          (HSF_MKRSP_STD(HAPI_HALSHIM_CONFIG_CMD))
      /* OP1: IBD mask              */
      /* OP2: Samples per HAL tick  */



#endif      /* HAPI_HALSHIM_H */
