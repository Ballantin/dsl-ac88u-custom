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
#ifndef APM_SHARED_H
#define APM_SHARED_H

#include <xdrvTypes.h>
#include <dma3341.h>
#include "apm3341.h"

/* ---- Constants and Types ---------------------------------------------- */

#define SRAM3341_APM_SAMPLE_SIZE 96

typedef struct SRAM3341_APMDMA_SAMPLES
{
   XDRV_SINT16  samples[APM3341_NUM_CHANNELS][SRAM3341_APM_SAMPLE_SIZE];
} SRAM3341_APMDMA_SAMPLES;

typedef struct SRAM3341_APMDMA
{
   volatile SRAM3341_APMDMA_SAMPLES ingress;
   volatile SRAM3341_APMDMA_SAMPLES egress;
} SRAM3341_APMDMA;

#endif  /* APM_SHARED_H */

