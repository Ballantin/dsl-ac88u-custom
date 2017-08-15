/****************************************************************************
*
*  Copyright (c) 2002-2010 Broadcom
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
*
*  dma.h -  DMA Definitions
*
*  PURPOSE: DMA defines used by DSP and PCM.
*
*  NOTES:
*
****************************************************************************/

#ifndef XCHG_DMA_H
#define XCHG_DMA_H

#ifdef __cplusplus
  extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>

/* ---- Constants and Types ---------------------------------------------- */

/*
** PCM / IUDMA Channel associations
*/
#define      BCM_IUDMA_PCM_RX      0
#define      BCM_IUDMA_PCM_TX      1

typedef struct IUDMA_ADDR16
{
   XDRV_UINT16 addr_h;                 /* high word */
   XDRV_UINT16 addr_l;                 /* low word */
} IUDMA_ADDR16;

/*
** MIPS core uses 32-bit access. However, certain modules (such as DSP if present)
** might need 16-bit addresses to ensure proper endian mode.
** Union the two so the descriptor definition can be shared */
typedef union
{
   IUDMA_ADDR16 addr16;
   XDRV_UINT32  addr32;
} IUDMA_ADDR;


typedef enum
{
   IUDMA_TYPE_PCM,
   IUDMA_TYPE_APM
} IUDMA_TYPE;

/****************************************************************************
** IUDMA Descriptors
****************************************************************************/
typedef struct BCM_IUDMA_DESC
{
   struct {
#if __arm__ // TODO: Turn this into little endian check
      XDRV_UINT32 status : 16;  /* buffer status */
      XDRV_UINT32 length : 16;  /* in bytes of data in buffer */
#else
      XDRV_UINT32 length : 16;  /* in bytes of data in buffer */
      XDRV_UINT32 status : 16;  /* buffer status */
#endif
   } flagsLengthStatus;
   IUDMA_ADDR addr;

} BCM_IUDMA_DESC;


/**********************************************************************************
** IUDMA descriptor and buffer addresses in both device and virtual memory space
**********************************************************************************/
typedef struct BCM_IUDMA_CTL_MAP
{
   BCM_IUDMA_DESC* descVirt;
   BCM_IUDMA_DESC* descPhy;
   IUDMA_ADDR bufPhy;
   IUDMA_ADDR bufVirt;
} BCM_IUDMA_ADDR_MAP;

typedef struct BCM_IUDMA_CTL
{
   IUDMA_TYPE  type;      /* Type of DMA channel */
   XDRV_UINT32 chanStart; /* Voice channel start index */
   XDRV_UINT32 chanCount; /* Voice channel count - number of voice channels associated with this DMA */
   XDRV_UINT32 dmaChanRx; /* IUDMA RX channel index */
   XDRV_UINT32 dmaChanTx; /* IUDMA TX channel index */
   volatile XDRV_UINT32 activeBufRx; /* Index of active DMA buffer (0 or 1) */
   volatile XDRV_UINT32 availableBufTx; /* Index of active DMA buffer (0 or 1) */
   BCM_IUDMA_ADDR_MAP rx[2]; /* RX buffer control map, double buffered */
   BCM_IUDMA_ADDR_MAP tx[2]; /* TX buffer control map, double buffered */
} BCM_IUDMA_ADDR_CTL;


/****************************************************************************
** IUDMA State RAM
****************************************************************************/
typedef struct BCM_IUDMA_STRAM
{
   XDRV_UINT32 baseDescPointer;                /* pointer to first buffer descriptor */

   XDRV_UINT32 stateBytesDoneRingOffset;       /* state info: how manu bytes done and the offset of the
                                             current descritor in process */
   XDRV_UINT32 flagsLengthStatus;              /* Length and status field of the current descriptor */

   XDRV_UINT32 currentBufferPointer;           /* pointer to the current descriptor */
} BCM_IUDMA_STRAM;

/****************************************************************************
** IUDMA Channel Control 
****************************************************************************/
typedef struct BCM_IUDMA_CTRL
{
    XDRV_UINT32 config;

    XDRV_UINT32 intStat;

    XDRV_UINT32 intMask;

    XDRV_UINT32 maxBurst;

} BCM_IUDMA_CTRL;

/****************************************************************************
** IUDMA Descriptor Ring
**
** Pointers to Tx and Rx DMA buffer descriptors. Each DMA channel consists
** of a descriptor ring. Each descriptor on the ring contains a pointer to the
** DMA buffer plus configuration data.
**
** The "start" arrays point to the first DMA descriptor in the ring.
** The "next" arrays point to the next DMA descriptor that will receive
** data for rx channels, and the next DMA descriptor that should be written to
** for tx channels.
****************************************************************************/
typedef struct BCM_IUDMA_DESC_RING
{
   volatile BCM_IUDMA_DESC *start;
   volatile BCM_IUDMA_DESC *next;
} BCM_IUDMA_DESC_RING;

#define BCM_IUDMA_DESC_STATUS_OWN       0x8000
#define BCM_IUDMA_DESC_STATUS_EOP       0x4000
#define BCM_IUDMA_DESC_STATUS_SOP       0x2000
#define BCM_IUDMA_DESC_STATUS_WRAP      0x1000
#define BCM_IUDMA_DESC_STATUS_PERIPH    0x0fff


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* DMA6368_H */
