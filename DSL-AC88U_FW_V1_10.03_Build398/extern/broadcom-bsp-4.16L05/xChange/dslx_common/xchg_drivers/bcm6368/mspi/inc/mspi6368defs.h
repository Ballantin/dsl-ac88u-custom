/****************************************************************************
*
*  Copyright (c) 2001-2008 Broadcom
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
*    Description:
*
****************************************************************************/
#ifndef MSPI6368DEFS_H 
#define MSPI6368DEFS_H

/* ---- Include Files ---------------------------------------- */

#include <bosSem.h>
#include <bcm6368.h>
#include <xdrvIntCtrl.h>
//#include <xdrvTypes.h>
#include <xdrvLog.h>

/* ---- Constants and Types ---------------------------------- */
#define MSPI_SUCCESS                           0
#define MSPI_ERROR                             1

#define INTERRUPT_ID_SPI_IRQ                 101

#define MSPI6368_MAX_RX_SIZE                1088
                                   
#define BCM6368_MSPI_TX_QUEUE_SIZE           544
#define BCM6368_MSPI_RX_QUEUE_SIZE           544

#define MMR6368_MSPI_INTSTATUS_CMNDDONE     0x01
#define MMR6368_MSPI_INTSTATUS_RXOVERFL     0x02
#define MMR6368_MSPI_INTSTATUS_TXUNDRFL     0x04
#define MMR6368_MSPI_INTSTATUS_TXOVERFL     0x08
#define MMR6368_MSPI_INTSTATUS_RXUNDRFl     0x10
#define MMR6368_MSPI_INTSTATUS_MASK         0x1F

#define MMR6368_MSPI_START_NOOP             0x00
#define MMR6368_MSPI_START_SOFT_RESET       0x01
#define MMR6368_MSPI_START_HARD_RESET       0x02
#define MMR6368_MSPI_START_IMMEDIATE        0x03
#define MMR6368_MSPI_START_NONBUF           0x0F

#define MMR6368_MSPI_STATUS_RXEMPTY         0x02
#define MMR6368_MSPI_STATUS_CMDBUSY         0x04
#define MMR6368_MSPI_STATUS_SERBUSY         0x08

typedef struct
{
   volatile SpiControl *p6368SpiRegs;

   int (*rxCallbackFnc)(XDRV_UINT8);
   int (*txCallbackFnc)(XDRV_UINT8);

   XDRV_UINT8  *pTxBuf;
   unsigned     txOffset;

   XDRV_UINT8   rxBuf[ MSPI6368_MAX_RX_SIZE ];
   unsigned     rxOffset;

   int          bytesRemaining;
   int          bytesQueued;

} BCM_MSPI6368_DRV;

#if MSPI6368_USE_INTERRUPTS
typedef void (*MSPI6368_ISR)(void *contextP);
#endif /* MSPI6368_USE_INTERRUPTS */

/* ---- Variable Externs ------------------------------------- */

extern BCM_MSPI6368_DRV  mspi6368Drv;
extern BCM_MSPI6368_REGS mspi6368Registers;

#if MSPI6368_USE_INTERRUPTS
extern BOS_SEM           mspi6368IsrSem;
#endif /* MSPI6368_USE_INTERRUPTS */

/* ---- Function Prototypes ---------------------------------- */

#if MSPI6368_USE_INTERRUPTS
unsigned int mspi6368Isr( XDRV_INT_CTRL_ISR_PARM contextp );
#endif /* MSPI6368_USE_INTERRUPTS */

void mspi6368intc_IrqDisable( void );
void mspi6368intc_IrqEnable( void );
void mspi6368intc_intVectIrqIsrInstall( XDRV_INT_CTRL_ISR isr );

#endif   /* MSPI6368DEFS_H */
