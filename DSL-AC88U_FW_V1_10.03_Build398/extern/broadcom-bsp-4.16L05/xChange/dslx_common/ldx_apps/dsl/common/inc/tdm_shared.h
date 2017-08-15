/****************************************************************************
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
****************************************************************************/
#ifndef TDM_SHARED_H
#define TDM_SHARED_H

#include <xdrvTypes.h>
#include <dma3341.h>
#include <tdm3341.h>

/* ---- Constants and Types ---------------------------------------------- */
//LH#define SRAM3341_TDM_SAMPLE_SIZE (8*1)
#define SRAM3341_TDM_SAMPLE_SIZE (16*2)

typedef struct SRAM3341_TDMDMA_SAMPLES
{
  XDRV_SINT16  samples[SRAM3341_TDM_SAMPLE_SIZE];
} SRAM3341_TDMDMA_SAMPLES;

typedef struct SRAM3341_TDMADMA
{

   TDM3341_DMA_DESCRIPTORS descriptors;
   volatile SRAM3341_TDMDMA_SAMPLES ingress[TDM3341_NUM_DESCRIPTORS];
   volatile SRAM3341_TDMDMA_SAMPLES egress;
} SRAM3341_TDMDMA;

#endif  /* TDM_SHARED_H */

