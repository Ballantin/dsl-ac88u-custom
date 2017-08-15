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
#ifndef SRAMLAYOUT_HWAPP_HOSTONLY_H
#define SRAMLAYOUT_HWAPP_HOSTONLY_H

#include <xdrvTypes.h>
#include <apm3341.h>
#include "apm_shared.h"


/* ---- Constants and Types ---------------------------------------------- */

/*
** The entire shared sram is 0x1000 words in size.
** We want the monitor image to live at offset 0x800, therefore we set aside
** a scratch block after the apm block.
**
*/
#define APM_BLOCK_SIZEW       ( sizeof( SRAM3341_APMDMA )/ sizeof( XDRV_UINT16 ) )
#define SSRAM3341_DMAMEM_SIZEW   APM_BLOCK_SIZEW



typedef union
{
   SRAM3341_APMDMA   apmDma;
 
} SSRAM3341_DMAMEM;


/*---------------------------------------------------------------*/

typedef struct
{
   HAPI_UINT16 samples[2][8*5];
} SAMPLE_BUFFER;


typedef struct
{
   SAMPLE_BUFFER toHostBuf[2];
   SAMPLE_BUFFER fromHostBuf[2];
 
} ABCD;


/*
** Scratch SRAM memory usage on 3341
*/
typedef struct
{
   SSRAM3341_DMAMEM     ssramDmaMem;

} SSRAM3341_LAYOUT_HWAPP;


#endif  /* SRAMLAYOUT_HWAPP_HOSTONLY_H */
