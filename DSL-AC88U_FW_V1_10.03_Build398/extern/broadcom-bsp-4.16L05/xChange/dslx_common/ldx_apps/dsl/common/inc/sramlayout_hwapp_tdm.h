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
#ifndef SRAMLAYOUT_HWAPP_H
#define SRAMLAYOUT_HWAPP_H

#include <xdrvTypes.h>
#include <dbgCom.h>
#include <ipcSharedMem.h>
#include "apm_shared.h"
#include "tdm_shared.h"


/* ---- Constants and Types ---------------------------------------------- */

/*
** The entire shared sram is 0x1000 words in size.
** We want the monitor image to live at offset 0x800, therefore we set aside
** a scratch block after the apm block.
**
*/
#define APM_BLOCK_SIZEW       ( sizeof( SRAM3341_APMDMA )/ sizeof( XDRV_UINT16 ) )
#define TDM_BLOCK_SIZEW       ( sizeof( SRAM3341_TDMDMA )/ sizeof( XDRV_UINT16 ) )
#define DBGCOM_SIZEW          ( sizeof( DBGCOM ) / sizeof( XDRV_UINT16 ) )

#define SSRAM3341_DMAMEM_SIZEW  TDM_BLOCK_SIZEW

#define IPC_HEAP_SIZEW        ( MMR3341_SCRATCH_SRAM_SIZE - SSRAM3341_DMAMEM_SIZEW - DBGCOM_SIZEW )

#define IPC_HEAP1_SIZEW       ((MMR3341_SCRATCH_SRAM_SIZE / 2) - SSRAM3341_DMAMEM_SIZEW - DBGCOM_SIZEW)
#define SRAM3341_BOOT_SIZE    0x0200
#define IPC_HEAP2_SIZEW       ((MMR3341_SCRATCH_SRAM_SIZE / 2) - SRAM3341_BOOT_SIZE )


/*
** Two different memory layouts are provided for the shared memory heap -
** a contiguous heap and a non-contiguous heap. Normally, the contiguous heap
** should be used. If the monitor program is required to debug the DSP, the
** non-contiguous heap should be used. The monitor program must be located
** at the half way point of the shared memory. This results in two heap fragments,
** and may require the number of IPC channels supported to be decreased.
*/
typedef struct SSRAM3341_NONCONTIG_HEAP
{
   XDRV_UINT16    heap1[ IPC_HEAP1_SIZEW ];
   XDRV_UINT16    bootOrMonitorImage[ SRAM3341_BOOT_SIZE ];
   XDRV_UINT16    heap2[ IPC_HEAP2_SIZEW ];

} SSRAM3341_NONCONTIG_HEAP;

typedef union SSRAM3341_HEAP
{
   XDRV_UINT16                contiguousHeap[ IPC_HEAP_SIZEW ];

   SSRAM3341_NONCONTIG_HEAP   nonContiguousHeap;

} SSRAM3341_HEAP;

//DMAMEM should only include TDM and not APM for the TDM app. If both are here, there is not enough space for the heap.
typedef struct
{
   SRAM3341_TDMDMA   tdmDma;
} SSRAM3341_DMAMEM;

/*
** Scratch SRAM memory usage on 3341
*/
typedef struct
{
   SSRAM3341_DMAMEM     ssramDmaMem;
   volatile DBGCOM_MEM  dbgComPort;
   SSRAM3341_HEAP       heap;

   /*
   ** Do not put anything after the ipcHeap!!!
   */

} SSRAM3341_LAYOUT_HWAPP;


#endif  /* SRAMLAYOUT_HWAPP_H */
